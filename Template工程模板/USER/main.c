#include "stm32f10x.h"
#include "sys.h"
#include "usart3.h"	 
#include "led.h"

//STM32F103核心板例程
//库函数版本例程
/********** mcudev.taobao.com 出品  ********/

float gyz;
int acc;
int acc1;
int PWM_MAX=7200,PWM_MIN=-7200;

int main(void)
{
	int i=0;
	//short temp;					//温度
	u16 times=0;
	//char str[20]={0};
	
	delay_init();          //延时函数初始化	 
	NVIC_Configuration();    //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200); //
	uart2_init(9600);
	MPU_Init();							//MPU6050初始化
	mpu_dmp_init();
	MPU6050_EXTI_Init();			//外部中断*/
	
	Encoder_Init();                   //编码器初始化 定时器2 4编码器获取脉冲数 PA0 1 PB6 7
	
	MOTOR_Init();				  //电机IO口初始化
	//因为定时器1 通道1的pwm输出的引脚接在LED上 有冲突 因此改成定时器8
	PWM_Init_TIM8(1,7199);		//参数1 分频  参数2 重装载值 
	
	LED_Init();			     //LED端口初始化

	printf("\r\n精英STM32开发板 串口实验\r\n");
/*	for(i=0;i<10;i++)
	{
		delay_ms(1000);
	}*/
	
	printf("speed=%d\r\n",Read_Speed(4));
	for(i=0;i<3;i++)
	{
		delay_ms(1000);
	}
	while(1)
	{
		
		times++;

		if(times%6==0) 
		{
			/*memset(str,0,20);
			sprintf(str,"%d",send_Angle1);
			USART2_Send_Stringbylen(str,20);*/
			/*mpu_dmp_get_data(&Pitch,&Roll,&Yaw);//角度 俯仰角 横滚角 偏航角
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);//陀螺仪 角速度
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz); //加速度*/
			//printf("times=%d\r\n",times);
			//printf("left  speed=%d\r\n",Read_Speed(2));
			//printf("right speed=%d\r\n",Read_Speed(4));
			//sprintf(str,"%d",(int)Pitch);
			//sprintf(str,"%d",times);
			//USART2_Send_String(str);
			//USART2_Send_String(str);
			//memset(str,0x0,sizeof(str));
			//printf("x:%f  y:%f  z:%f \r\n",Pitch,Roll,Yaw);
			//printf("y:%d  z:%d \r\n",gyroy,gyroz);
		}
		/*printf("x:%f  y:%f  z:%f \r\n",Pitch,Roll,Yaw);
		printf("x %d  y:%d  z:%d \r\n",gyrox,gyroy,gyroz);
		printf("x %d  y:%d  z:%d \r\n",gyrox,gyroy,gyroz);*/
		
		//if(times%3==0)LED=!LED;//闪烁LED,提示系统正在运行.
		//delay_ms(500);   
		
	}
/*	 
	float Pitch,Roll,Yaw;	//角度信息 俯仰角 横滚角 偏航角
short gyrox,gyroy,gyroz; //陀螺仪 角速度
short aacx,aacy,aacz; //加速度
	
	 u16 t;
	 u16 len;
	 u16 times=0;
 	 delay_init();	    	     //延时函数初始化	  
	 NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	 uart_init(9600);	       //串口初始化为9600
	 LED_Init();			     //LED端口初始化
	 while(1)
		{
			if(USART_RX_STA&0x8000)
			{					   
				len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
				printf("\r\n您发送的消息为:\r\n\r\n");
				for(t=0;t<len;t++)
				{
					USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
				}
				printf("\r\n\r\n");//插入换行
				USART_RX_STA=0;
			}else
			{
				times++;
				if(times%5000==0)
				{
					printf("\r\n精英STM32开发板 串口实验\r\n");
					printf("正点原子@ALIENTEK\r\n\r\n");
				}
				if(times%200==0)printf("请输入数据,以回车键结束\n");  
				if(times%30==0)LED=!LED;//闪烁LED,提示系统正在运行.
				delay_ms(10);   
			}
		}*/
}

