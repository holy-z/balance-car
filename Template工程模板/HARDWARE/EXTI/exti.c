#include "exti.h"
#include "sys.h"
#include "stm32f10x_exti.h"

void MPU6050_EXTI_Init(void) //MPU6050�ж� 
{
	EXTI_InitTypeDef EXTI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;	//����һ��GPIO�ṹ�����

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);//ʹ��GPIOBʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		//��ʼ��GPIO ����PB5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//����50MHzʱ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//����Ϊ��������ģʽ
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5); //�ⲿ�ж�������
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line5;   //�ж���
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;  //ʹ��
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;  //ģʽ �жϴ���
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling; //���� �½��ش���
	EXTI_Init(&EXTI_InitStruct);
}






