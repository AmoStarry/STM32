#ifndef __PID_H
#define __PID_H
#include "stm32f10x.h"                  // Device header

#define PID_ASSISTANT_EN (1) // 1:ʹ��PID����������ʾ���Σ�0��ʹ�ô���ֱ�Ӵ�ӡ����
typedef struct
{
	float target_val;   //Ŀ��ֵ
	float err;          //ƫ��ֵ
	float err_last;     //��һ��ƫ��ֵ
	float Kp,Ki,Kd;     //���������֡�΢��ϵ��
	float integral;     //����ֵ
	float output_val;   //���ֵ
}PID;

float PID_realize(float actual_val);
void PID_Init(void);
#endif
