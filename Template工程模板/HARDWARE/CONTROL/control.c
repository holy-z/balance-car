#include "control.h"
#include "sys.h"

#include "led.h"
#include "math.h" 
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define DEAD_VAL       2800	   //死区值0
#define SPEED_MAX		20
#define SPEED_MAX1		40
#define TURN_MAX		35
//-1.2
float Med_Angle1=-1.1;
float Med_Angle=-1.1; //机械中值 使小车自平衡的角度
float Target_Speed=0;  //期望速度
float Turn_Speed=0;  //转向速度
//{91.3, 0.21, 5.1, 0.10};
//550-450  520 8  312 5.1
//最大值乘0.6  630 3.5

float 
	Vertical_Kp=420,  //比例倾角
	Vertical_Kd=2.7; //微分倾角 得到倾角速度 直立环
float 
	Velocity_Kp=0.68,
	Velocity_Ki=0.0025; //速度环
float
	Turn_Kd=2,	//Turn_Kp 调大 右 调小 左
	Turn_Kp=50;  //转向速度调节

extern u8 Fore,Back,Left,Right,Fore1; //遥控
int MOTO1,MOTO2,MOTO;//电机装载
float Pitch_abs;
float Pitch_dec;
float Pitch,Roll,Yaw;	//角度信息 俯仰角 横滚角 偏航角
short gyrox,gyroy,gyroz; //陀螺仪 角速度
short aacx,aacy,aacz; //加速度

int Encoder_Left,Encoder_Right; //编码器读取到的转速
int Vertical_Out,Velocity_Out,Turn_Out; //控制环输出
int stop=0; //小车倒下时积分清零控制位

void EXTI9_5_IRQHandler(void) //外部中断PB5
{
	//USART2_Send_String("exit1");
	if(EXTI_GetITStatus(EXTI_Line5)!=0) //一级判断  触发中断
	{
		int PWM_out;
		if(PBin(5)==0) //二级判断
		{
			//USART2_Send_String("exit2");
			EXTI_ClearITPendingBit(EXTI_Line5);  //清除中断标志位
			
			//1.采集编码器数据&MPU6050角度信息
			Encoder_Left=-Read_Speed(2); //电机是相对安装 刚好相差180度
			Encoder_Right=Read_Speed(4);//为了编码器输出极性一致，就需要对其中一个取反
			
			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);//角度 俯仰角 横滚角 偏航角
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);//陀螺仪 角速度
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz); //加速度
			printf("Pitch=%f\r\n",Pitch);
			
			Pitch_dec=Pitch-Med_Angle;
			Pitch_abs=Pitch_dec>0?Pitch_dec:(-Pitch_dec);
			
			
			//2.将数据压入闭环控制中，计算出输出量 Fore,Back,Left,Right SPEED_MAX
			
			//前后
			if((Fore==0)&&(Back==0)&&(Fore1==0)) 
			{
				Med_Angle=Med_Angle1;
				if(Target_Speed>0)
					Target_Speed-=0.2;
				else if(Target_Speed<0)
					Target_Speed+=0.2;
			}
			else if(Fore==1) 
			{
				Target_Speed-=0.1;
				//Med_Angle=Med_Angle1+10;
			}
			else if(Back==1)
			{
				Target_Speed+=0.1;
				//Med_Angle=Med_Angle1-10;
			}
			else if(Fore1==1)
			{
				Target_Speed-=0.1;
				//Med_Angle=Med_Angle1+15;
			}
			//速度限幅
			if(Fore1==1)
			{
				Target_Speed=Target_Speed>SPEED_MAX1?SPEED_MAX1:(Target_Speed<-SPEED_MAX1?(-SPEED_MAX1):Target_Speed);
			}
			else
			{
				Target_Speed=Target_Speed>SPEED_MAX?SPEED_MAX:(Target_Speed<-SPEED_MAX?(-SPEED_MAX):Target_Speed);
			}
			
			
			
			//左右
			if((Right==0)&&(Left==0)) Turn_Speed=0;
			else if(Right==1) Turn_Speed+=2;
			else if(Left==1) Turn_Speed-=2;
			//转向限幅
			Turn_Speed=Turn_Speed>TURN_MAX?TURN_MAX:(Turn_Speed<-TURN_MAX?(-TURN_MAX):Turn_Speed);
			
			//转向约束
			if((Right==0)&&(Left==0)) Turn_Kd=2; //若无左右转向指令 则开启转向约束
			else if((Right==1)||(Left==1)) Turn_Kd=0; //若有左右转向指令 则关闭转向约束
			
			
			
			Velocity_Out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);  //速度环
			Vertical_Out=Vertical(Med_Angle+Velocity_Out,Pitch,gyroy); //直立环
			Turn_Out=Turn(gyroz,Turn_Speed); //角度环
			
			PWM_out=Vertical_Out;
			//3.把控制输出量加载到电机上，完成最终的控制
			MOTO=PWM_out-Turn_Out;
			MOTO1=MOTO>0?(MOTO*0.6)+DEAD_VAL:(MOTO*0.6)-DEAD_VAL; //左电机
			MOTO=PWM_out+Turn_Out;
			MOTO2=MOTO>0?(MOTO*0.6)+DEAD_VAL:(MOTO*0.6)-DEAD_VAL; //右电机
			
			Limit(&MOTO1,&MOTO2); //PWM限幅
			if(Pitch_abs>35)
			{
				stop=1;
				MOTO1=0,MOTO2=0;
			}

			//printf("MOTO1=%d\r\n",MOTO1);
			//printf("MOTO2=%d\r\n",MOTO2);
			Load(MOTO1,MOTO2); //输出PWM波到电机

		}
	}
}

/***********************
内环 PD控制器
直立环  kp*ek+kd*ek_d
入口参数： Med期望角度  Angle真实俯仰角度  gyro_Y Y轴角速度
            给定角度 速度环输出 角度反馈（6050）
输出：直立环输出 直接控制电机速度

= Kp1* θ_r+ Kd*(θ_r-θ_e)＇-	Kp1* [Kp2*(E_r- E_e)+ Ki *Σ(E_r- E_e)]
=Kp1*真实角度+ Kd*角度偏差的微分-Kp1* [Kp2*编码器偏差- Ki *编码器偏差的积分]
***********************/
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_out; //直立环输出
	
	PWM_out=Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0); //0:期望的角度
	
	return PWM_out;
}



/***********************
外环  PI 电机速度
速度环  kp*ek+ki*ek_s
输入 目标速度 左电机反馈速度 右电机反馈速度
输出 角度值 即 直立环的期望角度输入

***********************/
int Velocity(int Target,int encode_left,int encode_right)
{
	static int Encoder_S,EnC_Err_Lowout_Last=0,PWM_out,Encoder_Err,EnC_Err_Lowout; //直立环输出
	float a=0.7;
	
	//1.计算速度偏差
	//舍去误差，能够让速度为 0 的角度 就是机械中值
	Encoder_Err=(encode_left+encode_right)-Target; //真实速度-期望
	
	//2.对速度偏差进行低通滤波
	//low_out=(1-a)*ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_Last; //使得波形更加平滑 滤除高频干扰 防止速度过大								
	EnC_Err_Lowout_Last=EnC_Err_Lowout;						//防止速度过大的影响直立环的正常工作
	
	//3.对速度偏差积分，积分出位移
	Encoder_S+=EnC_Err_Lowout;
	
	//4.积分限幅
	Encoder_S=Encoder_S>8000?8000:(Encoder_S<(-8000)?(-8000):Encoder_S);
	if(stop==1)
		Encoder_S=0,stop=0;
	
	
	//5.对速度环控制输出计算
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
	
	return PWM_out;
}



/***********************
转向环  系数*Z轴角速度+系数*遥控数据
***********************/
int Turn(int gyro_z,int RC)
{
	int PWM_out;
	
	//不是严格的PD控制器，KD针对的是转向环的约束，kp针对的是遥控的转向
	PWM_out=Turn_Kd*gyro_z+Turn_Kp*RC;
	return PWM_out;
}










