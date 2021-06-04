#ifndef __USART3_H
#define __USART3_H
#include "string.h"
#include "sys.h" 



void USART2_Send_String(char *String);
void USART2_Send_Stringbylen(char *String,int len);
void uart2_init(u32 bound);					//串口3初始化函数
//void USART3_IRQHandler(void);     	//串口3中断服务程序
#endif

