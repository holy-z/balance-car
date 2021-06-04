#include "usart3.h"	  
#include "led.h"

extern float Target_Speed;
void uart2_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʱ��GPIOA��USART2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//USART1_TX   PA2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//USART1_RX	  PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
}

u8 Fore,Back,Left,Right,Fore1;
void USART2_IRQHandler(void) 
{
	static char buf[10]={0};
	int Bluetooth_data;
//	int i=0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//�����жϱ�־λ����
	{
		Bluetooth_data=USART_ReceiveData(USART2);//������յ�����
		sprintf(buf,"%d",Bluetooth_data);
		USART2_Send_String(buf);
		memset(buf,0x0,sizeof(buf));
		if(Bluetooth_data==48)
		{
			USART2_Send_String("0");
			Fore=0,Back=0,Left=0,Right=0,Fore1=0;//ɲ
			//Target_Speed=0;
			/*delay_ms(1000);
			for(i=0;i<2;i++)
			{
				LED=!LED;//��˸LED,��ʾϵͳ��������.
				delay_ms(500);
			}*/
			 
		}
		else if(Bluetooth_data==49)
		{
			USART2_Send_String("1");
			Fore=1,Back=0,Left=0,Right=0,Fore1=0;//ǰ

		}
		else if(Bluetooth_data==50)
		{
			USART2_Send_String("2");
			Fore=0,Back=1,Left=0,Right=0,Fore1=0;//��
		}
		else if(Bluetooth_data==51)
		{
			USART2_Send_String("3");
			Fore=0,Back=0,Left=1,Right=0,Fore1=0;//��
			
		}
		else if(Bluetooth_data==52)
		{
			USART2_Send_String("4");
			Fore=0,Back=0,Left=0,Right=1,Fore1=0;//��
		}
		else if(Bluetooth_data==53)
		{
			USART2_Send_String("5");
			Fore=0,Back=0,Left=0,Right=0,Fore1=1;//��
		}
		else
		{
			USART2_Send_String("0");
			Fore=0,Back=0,Left=0,Right=0;//ɲ
		}			
	}
}

//һ��
void USART2_Send_Data(char data)
{
	USART_SendData(USART2,data);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
}

//һ��
void USART2_Send_String(char *String)
{
	u16 len,j;
	
	len=strlen(String);
	for(j=0;j<len;j++)
	{
		USART2_Send_Data(*String++);
	}
}

void USART2_Send_Stringbylen(char *String,int len)
{
	u16 l,j;
	
	l=len;
	for(j=0;j<l;j++)
	{
		USART2_Send_Data(*String++);
	}
}



