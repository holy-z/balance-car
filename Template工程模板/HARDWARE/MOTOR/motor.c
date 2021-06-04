#include "sys.h"
#include "motor.h"

/*
  控制GPIO PB12/PB13/PB14/PB15
  电机MOTOR  TIM2_CH3/TIM2_CH4
  光电编码器 TIM3_CH1/TIM3_CH2
  光电编码器 TIM4_CH1/TIM4_CH2
*/
//电机初始化函数
void MOTOR_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个GPIO_InitTypeDef类型的结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //开启GPIOB的外设时钟

															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //选择要控制的GPIOB引脚	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //设置引脚模式为通用推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置引脚速率为50MHz
  	GPIO_Init(GPIOB, &GPIO_InitStructure);	//调用库函数，初始化GPIOB
}

//限幅函数
void Limit(int *motoA,int *motoB) //限制
{
	if(*motoA>PWM_MAX) *motoA=PWM_MAX;
	if(*motoA<PWM_MIN) *motoA=PWM_MIN;
	
	if(*motoB>PWM_MAX) *motoB=PWM_MAX;
	if(*motoB<PWM_MIN) *motoB=PWM_MIN;
}

//绝对值
int ZHJ_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

//将PID算出的输出量加载到电机
void Load(int moto1,int moto2)
{
	if(moto1>0) Ain1=1,Ain2=0;
	else		Ain1=0,Ain2=1;
	TIM_SetCompare1(TIM8, ZHJ_abs(moto1));
		
	if(moto2>0) Bin1=1,Bin2=0;
	else		Bin1=0,Bin2=1;
	TIM_SetCompare4(TIM8, ZHJ_abs(moto2));
}






// /*       TIM3_CH2 为脉冲输入口
// 1.  配置GPIO_GPIOA_PIN7 输入
// 2.  配置TIM3 计数器在TI2 端的上升沿计数:
//     1). TIMx_CCMR1: CC2S =01;      配置通道2检测TI2输入的上升沿
//     2). TIMx_CCMR1:IC2F  =000;     选择输入滤波器带宽
//     3). TIMx_CCER:  CC2P =0;         配置上升沿极性                                 √
//     4). TIMx_SMCR:  SMS  =111;     选择定时器外部时钟模式1 
//     5). TIMx_SMCR:  TS    =110;     选择TI2作为触发输入源                   √
//     6). TIMx_CR1:    CEN  =1;        启动计数器
// */		

// /*       TIM4_CH2 为脉冲输入口
// 1.  配置GPIO_GPIOB_PIN7 输入
// 2.  配置TIM4 计数器在TI2 端的上升沿计数:
//     1). TIMx_CCMR1: CC2S =01;      配置通道2检测TI2输入的上升沿
//     2). TIMx_CCMR1:IC2F  =000;     选择输入滤波器带宽
//     3). TIMx_CCER:  CC2P =0;         配置上升沿极性                                 √
//     4). TIMx_SMCR:  SMS  =111;     选择定时器外部时钟模式1 
//     5). TIMx_SMCR:  TS    =110;     选择TI2作为触发输入源                   √
//     6). TIMx_CR1:    CEN  =1;        启动计数器
// */




