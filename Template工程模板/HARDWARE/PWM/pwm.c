#include "pwm.h"
#include "sys.h"
#include "stm32f10x.h"
void PWM_Init_TIM8(u16 Psc,u16 Per)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����һ��GPIO�ṹ�����
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//����һ����ʱ���ṹ�����
	TIM_OCInitTypeDef TIM_OCInitStruct;//����һ������ȽϽṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_TIM8 | RCC_APB2Periph_AFIO, ENABLE);//ʹ��GPIOCʱ�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_9;		//��ʼ��GPIO ����PC6 9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//����50MHzʱ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//����Ϊ�����������ģʽ
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��

	//��ʼ����ʱ��
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// ʹ�õĲ���Ƶ��֮��ķ�Ƶ���� ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ ���ϼ���
	TIM_TimeBaseStructure.TIM_Period = Per;	//��װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = Psc;	//��Ƶϵ�� 
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);	//��ʼ����ʱ��8
	
	//��ʼ������Ƚ�
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;  //PWM1ģʽ
 	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;  //��������CCR�� ��ƽΪ0 �߼���
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;  //�Ƚ����ʹ�� ״̬
	TIM_OCInitStruct.TIM_Pulse=0; //ռ�ձ�
	TIM_OC1Init(TIM8, &TIM_OCInitStruct); //ͨ��1
	TIM_OC4Init(TIM8, &TIM_OCInitStruct); //ͨ��4
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE); //�߼���ʱ��ר��MOE �����ʹ�� ���PWM���������� �����������
	
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable); //OC1Ԥװ�ؼĴ���ʹ��
	TIM_OC4PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM8,ENABLE); //��ʱ��1��ARR��Ԥװ�ؼĴ���ʹ��
	
	TIM_Cmd(TIM8,ENABLE); //����ʱ��
/*	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO, ENABLE);//ʹ��GPIOAʱ�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;		//��ʼ��GPIO ����PA8 11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//����50MHzʱ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//����Ϊ�����������ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��

	//��ʼ����ʱ��
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// ʹ�õĲ���Ƶ��֮��ķ�Ƶ���� ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ ���ϼ���
	TIM_TimeBaseStructure.TIM_Period = Per;	//��װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = Psc;	//��Ƶϵ�� 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);	//��ʼ����ʱ��2
	
	//��ʼ������Ƚ�
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;  //PWM1ģʽ
 	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;  //��������CCR�� ��ƽΪ0 �߼���
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;  //�Ƚ����ʹ�� ״̬
	TIM_OCInitStruct.TIM_Pulse=0; //ռ�ձ�
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE); //�߼���ʱ��ר��MOE �����ʹ�� ���PWM���������� �����������
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable); //OC1Ԥװ�ؼĴ���ʹ��
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1,ENABLE); //��ʱ��1��ARR��Ԥװ�ؼĴ���ʹ��
	
	TIM_Cmd(TIM1,ENABLE); //����ʱ��*/
	
	
}





