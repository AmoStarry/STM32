#include "PID.h"

/**
  * @brief  PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
  *	@note 	��
  * @retval ͨ��PID���������
  */
  
 /*����һ��PID�ṹ���͵�ȫ�ֱ���*/
PID pid;

float PID_realize(float actual_val)
{
     
	/*����Ŀ��ֵ��ʵ��ֵ�����*/
	pid.err = pid.target_val - actual_val;
	
	/*������*/
	pid.integral += pid.err;

	/*PID�㷨ʵ��*/
	pid.output_val = pid.Kp * pid.err + 
				     pid.Ki * pid.integral + 
				     pid.Kd * (pid.err - pid.err_last);

	/*����*/
	pid.err_last = pid.err;

	/*���ص�ǰʵ��ֵ*/
	return pid.output_val;
}

/*-----------------------�ⲿ���Ӳ���----------------------*/
/**
  * @brief  PID������ʼ��
  *	@note 	��
  * @retval ��
  */
void PID_Init()
{
	float pid_temp[3];

	/* ��ʼ������(������λ����, ��ʵʱ�޸�PID����) */
	pid.target_val=3000;	  //Ŀ��ֵ			
	pid.output_val=0.0;
	pid.err=0.0;
	pid.err_last=0.0;
	pid.integral=0.0;

	pid.Kp = 0.01;
	pid.Ki = 2;
	pid.Kd = 0;

#if defined(PID_ASSISTANT_EN)
	pid_temp[0] = pid.Kp;
	pid_temp[1] = pid.Ki;
	pid_temp[2] = pid.Kd;
  //set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid_temp, 3);     // ��ͨ��1����PIDֵ
#endif
}

