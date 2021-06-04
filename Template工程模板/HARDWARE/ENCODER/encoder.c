#include "sys.h"
#include "encoder.h"
/*************************��ʱ��TIM2��ʼ��****************************/
void TIM2_Encoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����һ��GPIO�ṹ�����
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//����һ����ʱ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStructure;	//����һ����ʱ���������ṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ�� 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ�ܶ�ʱ��2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;		//��ʼ��GPIO ����PA0->TIM2_CH1,PA1->TIM2_CH2
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//����ģʽ �������ٶ� ����50MHzʱ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//����Ϊ��������ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��

	//��ʼ����ʱ��
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	//��Ƶϵ�� ʱ�Ӳ���Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ ���ϼ���
	TIM_TimeBaseStructure.TIM_Period = 0xffff;	//���������ֵ	 ��װ��ֵ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	//��ʼ����ʱ��2

	//���������� TIM_EncoderMode_TI12  ����Ϊ TI1 TI2�����б��ؼ���
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ģʽ3 ������ �����ش���
	
	TIM_ICStructInit(&TIM_ICInitStructure); //��ʼ�����벶��
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
 
//  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//�����ʱ��2�жϱ�־λ
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);		//���� ������� �жϱ�־λ
	
	TIM_SetCounter(TIM2,0);//��ʼֵ��0
	
	TIM_Cmd(TIM2, ENABLE);  //������ʹ�ܣ���ʼ����
}


/*************************��ʱ��TIM4��ʼ��****************************/

void TIM4_Encoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����һ��GPIO�ṹ�����
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//����һ����ʱ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStructure;	//����һ����ʱ���������ṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ�ܶ�ʱ��4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;		//����PA0->TIM2_CH1,PA1->TIM2_CH2
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//����50MHzʱ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//����Ϊ��������ģʽ
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	//��Ƶϵ�� ʱ�Ӳ���Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ ���ϼ���
	TIM_TimeBaseStructure.TIM_Period = 0xffff;	//���������ֵ	 ��װ��ֵ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	//��ʼ����ʱ��4

	//����������
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ģʽ3 ������
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
	TIM_SetCounter(TIM4, 0x7fff);
 
//  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//�����ʱ��4�жϱ�־λ
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);		//�򿪶�ʱ��4�ж� �������
	
	TIM_SetCounter(TIM4,0);//��ʼֵ��0
	
	TIM_Cmd(TIM4, ENABLE);  //������ʹ�ܣ���ʼ����
}

//�������ٶȶ�ȡ����
int Read_Speed(int TIMx)
{
	int value_1;
	switch(TIMx)
	{
		case 2:
			value_1=(short)TIM_GetCounter(TIM2);
			TIM_SetCounter(TIM2,0);//��ʼֵ��0
			break;
		case 4:
			value_1=(short)TIM_GetCounter(TIM4);
			TIM_SetCounter(TIM4,0);//��ʼֵ��0
			break;
		default:value_1=0;
	}
	return value_1;
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM2,TIM_IT_Update)!=0) //�жϱ�־λ����1 �����ж�
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//���
	}
}

//�жϷ�����
void TIM4_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM4,TIM_IT_Update)!=0) //�жϱ�־λ����1 �����ж�
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//���
	}
}

void Encoder_Init(void)
{
	TIM2_Encoder_Init();
	TIM4_Encoder_Init();
}



