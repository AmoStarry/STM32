#ifndef __PID_H
#define __PID_H
#include "stm32f10x.h"                  // Device header

#define PID_ASSISTANT_EN (1) // 1:ʹ��PID����������ʾ���Σ�0��ʹ�ô���ֱ�Ӵ�ӡ����
#define ENCODER_RESOLUTION 11    /*������һȦ������������*/
#define ENCODER_MULTIPLE 4       /*��������Ƶ��ͨ����ʱ���ı�����ģʽ����*/
#define MOTOR_REDUCTION_RATIO 46 /*����ļ��ٱ�*/
/*���תһȦ�ܵ�������(��ʱ���ܶ�����������) = ����������������*��������Ƶ*������ٱ� */
/* 11*4*34= 1496*/
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 


typedef struct
{
	float target_val;   //Ŀ��ֵ
	float err;          //ƫ��ֵ
	float err_last;     //��һ��ƫ��ֵ
	float Kp,Ki,Kd;     //���������֡�΢��ϵ��
	float integral;     //����ֵ
	float output_val;   //���ֵ
}PID;

float location_pid_realize(PID *pid, float actual_val);
float speed_pid_realize(PID *pid, float actual_val);

void PID_Init(void);
#endif
