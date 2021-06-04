#include "exti.h"
#include "sys.h"
#include "stm32f10x_exti.h"

void MPU6050_EXTI_Init(void) //MPU6050中断 
{
	EXTI_InitTypeDef EXTI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;	//定义一个GPIO结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);//使能GPIOB时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		//初始化GPIO 配置PB5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//设置50MHz时钟
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//设置为上拉输入模式
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5); //外部中断线配置
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line5;   //中断线
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;  //使能
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;  //模式 中断触发
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling; //边沿 下降沿触发
	EXTI_Init(&EXTI_InitStruct);
}






