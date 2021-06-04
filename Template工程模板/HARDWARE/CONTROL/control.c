#include "control.h"
#include "sys.h"

#include "led.h"
#include "math.h" 
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define DEAD_VAL       2800	   //����ֵ0
#define SPEED_MAX		20
#define SPEED_MAX1		40
#define TURN_MAX		35
//-1.2
float Med_Angle1=-1.1;
float Med_Angle=-1.1; //��е��ֵ ʹС����ƽ��ĽǶ�
float Target_Speed=0;  //�����ٶ�
float Turn_Speed=0;  //ת���ٶ�
//{91.3, 0.21, 5.1, 0.10};
//550-450  520 8  312 5.1
//���ֵ��0.6  630 3.5

float 
	Vertical_Kp=420,  //�������
	Vertical_Kd=2.7; //΢����� �õ�����ٶ� ֱ����
float 
	Velocity_Kp=0.68,
	Velocity_Ki=0.0025; //�ٶȻ�
float
	Turn_Kd=2,	//Turn_Kp ���� �� ��С ��
	Turn_Kp=50;  //ת���ٶȵ���

extern u8 Fore,Back,Left,Right,Fore1; //ң��
int MOTO1,MOTO2,MOTO;//���װ��
float Pitch_abs;
float Pitch_dec;
float Pitch,Roll,Yaw;	//�Ƕ���Ϣ ������ ����� ƫ����
short gyrox,gyroy,gyroz; //������ ���ٶ�
short aacx,aacy,aacz; //���ٶ�

int Encoder_Left,Encoder_Right; //��������ȡ����ת��
int Vertical_Out,Velocity_Out,Turn_Out; //���ƻ����
int stop=0; //С������ʱ�����������λ

void EXTI9_5_IRQHandler(void) //�ⲿ�ж�PB5
{
	//USART2_Send_String("exit1");
	if(EXTI_GetITStatus(EXTI_Line5)!=0) //һ���ж�  �����ж�
	{
		int PWM_out;
		if(PBin(5)==0) //�����ж�
		{
			//USART2_Send_String("exit2");
			EXTI_ClearITPendingBit(EXTI_Line5);  //����жϱ�־λ
			
			//1.�ɼ�����������&MPU6050�Ƕ���Ϣ
			Encoder_Left=-Read_Speed(2); //�������԰�װ �պ����180��
			Encoder_Right=Read_Speed(4);//Ϊ�˱������������һ�£�����Ҫ������һ��ȡ��
			
			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);//�Ƕ� ������ ����� ƫ����
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);//������ ���ٶ�
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz); //���ٶ�
			printf("Pitch=%f\r\n",Pitch);
			
			Pitch_dec=Pitch-Med_Angle;
			Pitch_abs=Pitch_dec>0?Pitch_dec:(-Pitch_dec);
			
			
			//2.������ѹ��ջ������У����������� Fore,Back,Left,Right SPEED_MAX
			
			//ǰ��
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
			//�ٶ��޷�
			if(Fore1==1)
			{
				Target_Speed=Target_Speed>SPEED_MAX1?SPEED_MAX1:(Target_Speed<-SPEED_MAX1?(-SPEED_MAX1):Target_Speed);
			}
			else
			{
				Target_Speed=Target_Speed>SPEED_MAX?SPEED_MAX:(Target_Speed<-SPEED_MAX?(-SPEED_MAX):Target_Speed);
			}
			
			
			
			//����
			if((Right==0)&&(Left==0)) Turn_Speed=0;
			else if(Right==1) Turn_Speed+=2;
			else if(Left==1) Turn_Speed-=2;
			//ת���޷�
			Turn_Speed=Turn_Speed>TURN_MAX?TURN_MAX:(Turn_Speed<-TURN_MAX?(-TURN_MAX):Turn_Speed);
			
			//ת��Լ��
			if((Right==0)&&(Left==0)) Turn_Kd=2; //��������ת��ָ�� ����ת��Լ��
			else if((Right==1)||(Left==1)) Turn_Kd=0; //��������ת��ָ�� ��ر�ת��Լ��
			
			
			
			Velocity_Out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);  //�ٶȻ�
			Vertical_Out=Vertical(Med_Angle+Velocity_Out,Pitch,gyroy); //ֱ����
			Turn_Out=Turn(gyroz,Turn_Speed); //�ǶȻ�
			
			PWM_out=Vertical_Out;
			//3.�ѿ�����������ص�����ϣ�������յĿ���
			MOTO=PWM_out-Turn_Out;
			MOTO1=MOTO>0?(MOTO*0.6)+DEAD_VAL:(MOTO*0.6)-DEAD_VAL; //����
			MOTO=PWM_out+Turn_Out;
			MOTO2=MOTO>0?(MOTO*0.6)+DEAD_VAL:(MOTO*0.6)-DEAD_VAL; //�ҵ��
			
			Limit(&MOTO1,&MOTO2); //PWM�޷�
			if(Pitch_abs>35)
			{
				stop=1;
				MOTO1=0,MOTO2=0;
			}

			//printf("MOTO1=%d\r\n",MOTO1);
			//printf("MOTO2=%d\r\n",MOTO2);
			Load(MOTO1,MOTO2); //���PWM�������

		}
	}
}

/***********************
�ڻ� PD������
ֱ����  kp*ek+kd*ek_d
��ڲ����� Med�����Ƕ�  Angle��ʵ�����Ƕ�  gyro_Y Y����ٶ�
            �����Ƕ� �ٶȻ���� �Ƕȷ�����6050��
�����ֱ������� ֱ�ӿ��Ƶ���ٶ�

= Kp1* ��_r+ Kd*(��_r-��_e)��-	Kp1* [Kp2*(E_r- E_e)+ Ki *��(E_r- E_e)]
=Kp1*��ʵ�Ƕ�+ Kd*�Ƕ�ƫ���΢��-Kp1* [Kp2*������ƫ��- Ki *������ƫ��Ļ���]
***********************/
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_out; //ֱ�������
	
	PWM_out=Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0); //0:�����ĽǶ�
	
	return PWM_out;
}



/***********************
�⻷  PI ����ٶ�
�ٶȻ�  kp*ek+ki*ek_s
���� Ŀ���ٶ� ���������ٶ� �ҵ�������ٶ�
��� �Ƕ�ֵ �� ֱ�����������Ƕ�����

***********************/
int Velocity(int Target,int encode_left,int encode_right)
{
	static int Encoder_S,EnC_Err_Lowout_Last=0,PWM_out,Encoder_Err,EnC_Err_Lowout; //ֱ�������
	float a=0.7;
	
	//1.�����ٶ�ƫ��
	//��ȥ���ܹ����ٶ�Ϊ 0 �ĽǶ� ���ǻ�е��ֵ
	Encoder_Err=(encode_left+encode_right)-Target; //��ʵ�ٶ�-����
	
	//2.���ٶ�ƫ����е�ͨ�˲�
	//low_out=(1-a)*ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_Last; //ʹ�ò��θ���ƽ�� �˳���Ƶ���� ��ֹ�ٶȹ���								
	EnC_Err_Lowout_Last=EnC_Err_Lowout;						//��ֹ�ٶȹ����Ӱ��ֱ��������������
	
	//3.���ٶ�ƫ����֣����ֳ�λ��
	Encoder_S+=EnC_Err_Lowout;
	
	//4.�����޷�
	Encoder_S=Encoder_S>8000?8000:(Encoder_S<(-8000)?(-8000):Encoder_S);
	if(stop==1)
		Encoder_S=0,stop=0;
	
	
	//5.���ٶȻ������������
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
	
	return PWM_out;
}



/***********************
ת��  ϵ��*Z����ٶ�+ϵ��*ң������
***********************/
int Turn(int gyro_z,int RC)
{
	int PWM_out;
	
	//�����ϸ��PD��������KD��Ե���ת�򻷵�Լ����kp��Ե���ң�ص�ת��
	PWM_out=Turn_Kd*gyro_z+Turn_Kp*RC;
	return PWM_out;
}










