#ifndef __PID_H
#define __PID_H
#include "stm32f10x.h"                  // Device header

#define PID_ASSISTANT_EN (1) // 1:使用PID调试助手显示波形，0：使用串口直接打印数据
typedef struct
{
	float target_val;   //目标值
	float err;          //偏差值
	float err_last;     //上一个偏差值
	float Kp,Ki,Kd;     //比例、积分、微分系数
	float integral;     //积分值
	float output_val;   //输出值
}PID;

float PID_realize(float actual_val);
void PID_Init(void);
#endif
