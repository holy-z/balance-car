#include "stm32f10x.h"
#include "sys.h"
#include "usart3.h"	 
#include "led.h"

//STM32F103���İ�����
//�⺯���汾����
/********** mcudev.taobao.com ��Ʒ  ********/

float gyz;
int acc;
int acc1;
int PWM_MAX=7200,PWM_MIN=-7200;

int main(void)
{
	int i=0;
	//short temp;					//�¶�
	u16 times=0;
	//char str[20]={0};
	
	delay_init();          //��ʱ������ʼ��	 
	NVIC_Configuration();    //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200); //
	uart2_init(9600);
	MPU_Init();							//MPU6050��ʼ��
	mpu_dmp_init();
	MPU6050_EXTI_Init();			//�ⲿ�ж�*/
	
	Encoder_Init();                   //��������ʼ�� ��ʱ��2 4��������ȡ������ PA0 1 PB6 7
	
	MOTOR_Init();				  //���IO�ڳ�ʼ��
	//��Ϊ��ʱ��1 ͨ��1��pwm��������Ž���LED�� �г�ͻ ��˸ĳɶ�ʱ��8
	PWM_Init_TIM8(1,7199);		//����1 ��Ƶ  ����2 ��װ��ֵ 
	
	LED_Init();			     //LED�˿ڳ�ʼ��

	printf("\r\n��ӢSTM32������ ����ʵ��\r\n");
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
			/*mpu_dmp_get_data(&Pitch,&Roll,&Yaw);//�Ƕ� ������ ����� ƫ����
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);//������ ���ٶ�
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz); //���ٶ�*/
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
		
		//if(times%3==0)LED=!LED;//��˸LED,��ʾϵͳ��������.
		//delay_ms(500);   
		
	}
/*	 
	float Pitch,Roll,Yaw;	//�Ƕ���Ϣ ������ ����� ƫ����
short gyrox,gyroy,gyroz; //������ ���ٶ�
short aacx,aacy,aacz; //���ٶ�
	
	 u16 t;
	 u16 len;
	 u16 times=0;
 	 delay_init();	    	     //��ʱ������ʼ��	  
	 NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	 uart_init(9600);	       //���ڳ�ʼ��Ϊ9600
	 LED_Init();			     //LED�˿ڳ�ʼ��
	 while(1)
		{
			if(USART_RX_STA&0x8000)
			{					   
				len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
				printf("\r\n�����͵���ϢΪ:\r\n\r\n");
				for(t=0;t<len;t++)
				{
					USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				}
				printf("\r\n\r\n");//���뻻��
				USART_RX_STA=0;
			}else
			{
				times++;
				if(times%5000==0)
				{
					printf("\r\n��ӢSTM32������ ����ʵ��\r\n");
					printf("����ԭ��@ALIENTEK\r\n\r\n");
				}
				if(times%200==0)printf("����������,�Իس�������\n");  
				if(times%30==0)LED=!LED;//��˸LED,��ʾϵͳ��������.
				delay_ms(10);   
			}
		}*/
}

