#ifndef _ENCODER_H_
#define _ENCODER_H_
#include "sys.h"
void TIM2_Encoder_Init(void);
void TIM4_Encoder_Init(void);
void Encoder_Init(void);
int Read_Speed(int TIMx);
#endif
