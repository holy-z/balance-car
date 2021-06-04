#include "sys.h"


/**
@code  
 The table below gives the allowed values of the pre-emption priority and subpriority according
 to the Priority Grouping configuration performed by NVIC_PriorityGroupConfig function
  ============================================================================================================================
    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
  ============================================================================================================================
   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
                         |                                   |                             |   4 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
                         |                                   |                             |   3 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
                         |                                   |                             |   2 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
                         |                                   |                             |   1 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_4  |                0-15               |            0                |   4 bits for pre-emption priority
                         |                                   |                             |   0 bits for subpriority                       
  ============================================================================================================================
@endcode
*/
//STM32������
//ϵͳ�жϷ������û�		   

//********************************************************************************  
void NVIC_Configuration(void) //����NVIC ����ִ���ж�
{
	NVIC_InitTypeDef NVIC_InitStruct;
	//�ж����ȼ����� ������ 4����ռ 4����Ӧ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	//�ⲿ�ж�
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;  //ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //ͨ��ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;  //��ռ���ȼ� 0�� ������ȼ� 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;  //��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStruct);
	
	//����
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;  //ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //ͨ��ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;  //��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStruct);
	
	//����
	NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;  //ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //ͨ��ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;  //��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStruct);

}
