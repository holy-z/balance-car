#ifndef _CONTROL_H_
#define _CONTROL_H_
//void EXTI9_5_IRQHandler(void); //�ⲿ�ж�PB5
int Vertical(float Med,float Angle,float gyro_Y); //ֱ����
int Velocity(int Target,int encode_left,int encode_right); //�ٶȻ�
int Turn(int gyro_z,int RC); //ת��

#endif

