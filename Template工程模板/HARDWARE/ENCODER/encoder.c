#include "sys.h"
#include "encoder.h"
/*************************定时器TIM2初始化****************************/
void TIM2_Encoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//定义一个GPIO结构体变量
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定义一个定时器结构体变量
	TIM_ICInitTypeDef TIM_ICInitStructure;	//定义一个定时器编码器结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能定时器2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;		//初始化GPIO 配置PA0->TIM2_CH1,PA1->TIM2_CH2
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//输入模式 不设置速度 设置50MHz时钟
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//设置为浮空输入模式
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化

	//初始化定时器
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	//分频系数 时钟不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// 使用的采样频率之间的分频比例
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式 向上计数
	TIM_TimeBaseStructure.TIM_Period = 0xffff;	//计数器最大值	 重装载值
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	//初始化定时器2

	//编码器配置 TIM_EncoderMode_TI12  设置为 TI1 TI2的所有边沿计数
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//模式3 不反相 上升沿触发
	
	TIM_ICStructInit(&TIM_ICInitStructure); //初始化输入捕获
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
 
//  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//清除定时器2中断标志位
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);		//配置 溢出更新 中断标志位
	
	TIM_SetCounter(TIM2,0);//初始值设0
	
	TIM_Cmd(TIM2, ENABLE);  //计数器使能，开始计数
}


/*************************定时器TIM4初始化****************************/

void TIM4_Encoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//定义一个GPIO结构体变量
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定义一个定时器结构体变量
	TIM_ICInitTypeDef TIM_ICInitStructure;	//定义一个定时器编码器结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;		//配置PA0->TIM2_CH1,PA1->TIM2_CH2
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//设置50MHz时钟
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//设置为浮空输入模式
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	//分频系数 时钟不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// 使用的采样频率之间的分频比例
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式 向上计数
	TIM_TimeBaseStructure.TIM_Period = 0xffff;	//计数器最大值	 重装载值
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	//初始化定时器4

	//编码器配置
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//模式3 上升沿
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
	TIM_SetCounter(TIM4, 0x7fff);
 
//  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//清除定时器4中断标志位
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);		//打开定时器4中断 溢出更新
	
	TIM_SetCounter(TIM4,0);//初始值设0
	
	TIM_Cmd(TIM4, ENABLE);  //计数器使能，开始计数
}

//编码器速度读取函数
int Read_Speed(int TIMx)
{
	int value_1;
	switch(TIMx)
	{
		case 2:
			value_1=(short)TIM_GetCounter(TIM2);
			TIM_SetCounter(TIM2,0);//初始值设0
			break;
		case 4:
			value_1=(short)TIM_GetCounter(TIM4);
			TIM_SetCounter(TIM4,0);//初始值设0
			break;
		default:value_1=0;
	}
	return value_1;
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM2,TIM_IT_Update)!=0) //中断标志位等于1 进入中断
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除
	}
}

//中断服务函数
void TIM4_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM4,TIM_IT_Update)!=0) //中断标志位等于1 进入中断
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清除
	}
}

void Encoder_Init(void)
{
	TIM2_Encoder_Init();
	TIM4_Encoder_Init();
}



