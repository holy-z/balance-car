#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"
#include "sys.h"
#define Ain1 PBout(14)
#define Ain2 PBout(15)

#define Bin1 PBout(13)
#define Bin2 PBout(12)

void MOTOR_Init(void);
void Limit(int *motoA,int *motoB); //ÏÞÖÆ
int ZHJ_abs(int p);
void Load(int moto1,int moto2);
   

#endif

