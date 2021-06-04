#include "pwm.h"
#include "sys.h"
#include "stm32f10x.h"
void PWM_Init_TIM8(u16 Psc,u16 Per)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//定义一个GPIO结构体变量
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定义一个定时器结构体变量
	TIM_OCInitTypeDef TIM_OCInitStruct;//定义一个输出比较结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_TIM8 | RCC_APB2Periph_AFIO, ENABLE);//使能GPIOC时钟 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_9;		//初始化GPIO 配置PC6 9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//设置50MHz时钟
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//设置为复用推挽输出模式
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化

	//初始化定时器
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// 使用的采样频率之间的分频比例 不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式 向上计数
	TIM_TimeBaseStructure.TIM_Period = Per;	//重装载值
	TIM_TimeBaseStructure.TIM_Prescaler = Psc;	//分频系数 
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);	//初始化定时器8
	
	//初始化输出比较
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;  //PWM1模式
 	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;  //计数超过CCR后 电平为0 高极性
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;  //比较输出使能 状态
	TIM_OCInitStruct.TIM_Pulse=0; //占空比
	TIM_OC1Init(TIM8, &TIM_OCInitStruct); //通道1
	TIM_OC4Init(TIM8, &TIM_OCInitStruct); //通道4
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE); //高级定时器专属MOE 主输出使能 输出PWM波所需设置 与编码器类似
	
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable); //OC1预装载寄存器使能
	TIM_OC4PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM8,ENABLE); //定时器1在ARR上预装载寄存器使能
	
	TIM_Cmd(TIM8,ENABLE); //开定时器
/*	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO, ENABLE);//使能GPIOA时钟 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;		//初始化GPIO 配置PA8 11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//设置50MHz时钟
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//设置为复用推挽输出模式
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化

	//初始化定时器
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// 使用的采样频率之间的分频比例 不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式 向上计数
	TIM_TimeBaseStructure.TIM_Period = Per;	//重装载值
	TIM_TimeBaseStructure.TIM_Prescaler = Psc;	//分频系数 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);	//初始化定时器2
	
	//初始化输出比较
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;  //PWM1模式
 	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;  //计数超过CCR后 电平为0 高极性
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;  //比较输出使能 状态
	TIM_OCInitStruct.TIM_Pulse=0; //占空比
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE); //高级定时器专属MOE 主输出使能 输出PWM波所需设置 与编码器类似
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable); //OC1预装载寄存器使能
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1,ENABLE); //定时器1在ARR上预装载寄存器使能
	
	TIM_Cmd(TIM1,ENABLE); //开定时器*/
	
	
}





