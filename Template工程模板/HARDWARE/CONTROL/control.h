#ifndef _CONTROL_H_
#define _CONTROL_H_
//void EXTI9_5_IRQHandler(void); //外部中断PB5
int Vertical(float Med,float Angle,float gyro_Y); //直立环
int Velocity(int Target,int encode_left,int encode_right); //速度环
int Turn(int gyro_z,int RC); //转向环

#endif

