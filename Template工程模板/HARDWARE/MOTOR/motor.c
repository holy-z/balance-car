#include "sys.h"
#include "motor.h"

/*
  ����GPIO PB12/PB13/PB14/PB15
  ���MOTOR  TIM2_CH3/TIM2_CH4
  �������� TIM3_CH1/TIM3_CH2
  �������� TIM4_CH1/TIM4_CH2
*/
//�����ʼ������
void MOTOR_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //����һ��GPIO_InitTypeDef���͵Ľṹ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //����GPIOB������ʱ��

															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //ѡ��Ҫ���Ƶ�GPIOB����	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //��������ģʽΪͨ���������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //������������Ϊ50MHz
  	GPIO_Init(GPIOB, &GPIO_InitStructure);	//���ÿ⺯������ʼ��GPIOB
}

//�޷�����
void Limit(int *motoA,int *motoB) //����
{
	if(*motoA>PWM_MAX) *motoA=PWM_MAX;
	if(*motoA<PWM_MIN) *motoA=PWM_MIN;
	
	if(*motoB>PWM_MAX) *motoB=PWM_MAX;
	if(*motoB<PWM_MIN) *motoB=PWM_MIN;
}

//����ֵ
int ZHJ_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

//��PID�������������ص����
void Load(int moto1,int moto2)
{
	if(moto1>0) Ain1=1,Ain2=0;
	else		Ain1=0,Ain2=1;
	TIM_SetCompare1(TIM8, ZHJ_abs(moto1));
		
	if(moto2>0) Bin1=1,Bin2=0;
	else		Bin1=0,Bin2=1;
	TIM_SetCompare4(TIM8, ZHJ_abs(moto2));
}






// /*       TIM3_CH2 Ϊ���������
// 1.  ����GPIO_GPIOA_PIN7 ����
// 2.  ����TIM3 ��������TI2 �˵������ؼ���:
//     1). TIMx_CCMR1: CC2S =01;      ����ͨ��2���TI2�����������
//     2). TIMx_CCMR1:IC2F  =000;     ѡ�������˲�������
//     3). TIMx_CCER:  CC2P =0;         ���������ؼ���                                 ��
//     4). TIMx_SMCR:  SMS  =111;     ѡ��ʱ���ⲿʱ��ģʽ1 
//     5). TIMx_SMCR:  TS    =110;     ѡ��TI2��Ϊ��������Դ                   ��
//     6). TIMx_CR1:    CEN  =1;        ����������
// */		

// /*       TIM4_CH2 Ϊ���������
// 1.  ����GPIO_GPIOB_PIN7 ����
// 2.  ����TIM4 ��������TI2 �˵������ؼ���:
//     1). TIMx_CCMR1: CC2S =01;      ����ͨ��2���TI2�����������
//     2). TIMx_CCMR1:IC2F  =000;     ѡ�������˲�������
//     3). TIMx_CCER:  CC2P =0;         ���������ؼ���                                 ��
//     4). TIMx_SMCR:  SMS  =111;     ѡ��ʱ���ⲿʱ��ģʽ1 
//     5). TIMx_SMCR:  TS    =110;     ѡ��TI2��Ϊ��������Դ                   ��
//     6). TIMx_CR1:    CEN  =1;        ����������
// */




