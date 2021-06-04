#include "usart3.h"	  
#include "led.h"

extern float Target_Speed;
void uart2_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//时钟GPIOA、USART2
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
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART2, ENABLE);                    //使能串口 
}

u8 Fore,Back,Left,Right,Fore1;
void USART2_IRQHandler(void) 
{
	static char buf[10]={0};
	int Bluetooth_data;
//	int i=0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//接收中断标志位拉高
	{
		Bluetooth_data=USART_ReceiveData(USART2);//保存接收的数据
		sprintf(buf,"%d",Bluetooth_data);
		USART2_Send_String(buf);
		memset(buf,0x0,sizeof(buf));
		if(Bluetooth_data==48)
		{
			USART2_Send_String("0");
			Fore=0,Back=0,Left=0,Right=0,Fore1=0;//刹
			//Target_Speed=0;
			/*delay_ms(1000);
			for(i=0;i<2;i++)
			{
				LED=!LED;//闪烁LED,提示系统正在运行.
				delay_ms(500);
			}*/
			 
		}
		else if(Bluetooth_data==49)
		{
			USART2_Send_String("1");
			Fore=1,Back=0,Left=0,Right=0,Fore1=0;//前

		}
		else if(Bluetooth_data==50)
		{
			USART2_Send_String("2");
			Fore=0,Back=1,Left=0,Right=0,Fore1=0;//后
		}
		else if(Bluetooth_data==51)
		{
			USART2_Send_String("3");
			Fore=0,Back=0,Left=1,Right=0,Fore1=0;//左
			
		}
		else if(Bluetooth_data==52)
		{
			USART2_Send_String("4");
			Fore=0,Back=0,Left=0,Right=1,Fore1=0;//右
		}
		else if(Bluetooth_data==53)
		{
			USART2_Send_String("5");
			Fore=0,Back=0,Left=0,Right=0,Fore1=1;//右
		}
		else
		{
			USART2_Send_String("0");
			Fore=0,Back=0,Left=0,Right=0;//刹
		}			
	}
}

//一个
void USART2_Send_Data(char data)
{
	USART_SendData(USART2,data);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
}

//一串
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



