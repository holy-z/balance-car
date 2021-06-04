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
//STM32开发板
//系统中断分组设置化		   

//********************************************************************************  
void NVIC_Configuration(void) //配置NVIC 才能执行中断
{
	NVIC_InitTypeDef NVIC_InitStruct;
	//中断优先级分组 分两组 4级抢占 4级响应
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	//外部中断
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;  //通道
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //通道使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;  //抢占优先级 0级 最高优先级 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;  //响应优先级
	NVIC_Init(&NVIC_InitStruct);
	
	//串口
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;  //通道
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //通道使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;  //响应优先级
	NVIC_Init(&NVIC_InitStruct);
	
	//串口
	NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;  //通道
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //通道使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;  //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;  //响应优先级
	NVIC_Init(&NVIC_InitStruct);

}
