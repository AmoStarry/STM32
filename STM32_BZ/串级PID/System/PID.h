#ifndef __PID_H
#define __PID_H
#include "stm32f10x.h"                  // Device header

#define PID_ASSISTANT_EN (1) // 1:使用PID调试助手显示波形，0：使用串口直接打印数据
#define ENCODER_RESOLUTION 11    /*编码器一圈的物理脉冲数*/
#define ENCODER_MULTIPLE 4       /*编码器倍频，通过定时器的编码器模式设置*/
#define MOTOR_REDUCTION_RATIO 46 /*电机的减速比*/
/*电机转一圈总的脉冲数(定时器能读到的脉冲数) = 编码器物理脉冲数*编码器倍频*电机减速比 */
/* 11*4*34= 1496*/
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 


typedef struct
{
	float target_val;   //目标值
	float err;          //偏差值
	float err_last;     //上一个偏差值
	float Kp,Ki,Kd;     //比例、积分、微分系数
	float integral;     //积分值
	float output_val;   //输出值
}PID;

float location_pid_realize(PID *pid, float actual_val);
float speed_pid_realize(PID *pid, float actual_val);

void PID_Init(void);
#endif
