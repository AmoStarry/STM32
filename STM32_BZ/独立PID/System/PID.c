#include "PID.h"
#include "protocol.h"
/**
  * @brief  PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
  *	@note 	��
  * @retval ͨ��PID���������
  */
  
 /*����һ��PID�ṹ���͵�ȫ�ֱ���*/
PID pid;


#define SPE_DEAD_ZONE 5.0f /*�ٶȻ�����*/
#define SPE_INTEGRAL_START_ERR 100 /*���ַ���ʱ��Ӧ����Χ*/
#define SPE_INTEGRAL_MAX_VAL 260   /*���ַ�Χ�޶�����ֹ���ֱ���*/
float PID_realize(float actual_val)
{
	/*����Ŀ��ֵ��ʵ��ֵ�����*/
	pid.err = pid.target_val - actual_val;

	/* �趨�ջ����� */
	if( (pid.err>-SPE_DEAD_ZONE) && (pid.err<SPE_DEAD_ZONE ) )
	{
		pid.err = 0;
		pid.integral = 0;
		pid.err_last = 0;
	}

#if 0
	/*������*/
	pid->integral += pid->err;
#else	
	/*��������ַ��룬ƫ��ϴ�ʱȥ����������*/
	if(pid.err > -SPE_INTEGRAL_START_ERR && pid.err < SPE_INTEGRAL_START_ERR)
	{
		pid.integral += pid.err;  
        /*���ַ�Χ�޶�����ֹ���ֱ���*/
		if(pid.integral > SPE_INTEGRAL_MAX_VAL)
		{
			pid.integral = SPE_INTEGRAL_MAX_VAL;
		}
		else if(pid.integral < -SPE_INTEGRAL_MAX_VAL)
		{
			pid.integral = -SPE_INTEGRAL_MAX_VAL;
		}
	}	
#endif
	
	/*PID�㷨ʵ��*/
	pid.output_val = pid.Kp * pid.err +
	                  pid.Ki * pid.integral +
	                  pid.Kd *(pid.err - pid.err_last);

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
	pid.target_val=0.0;	  //Ŀ��ֵ			
	pid.output_val=0.0;
	pid.err=0.0;
	pid.err_last=0.0;
	pid.integral=0.0;

	pid.Kp = 1;
	pid.Ki = 0;
	pid.Kd = 0;

#if defined(PID_ASSISTANT_EN)
	pid_temp[0] = pid.Kp;
	pid_temp[1] = pid.Ki;
	pid_temp[2] = pid.Kd;
     set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid_temp, 3);     // ��ͨ��1����PIDֵ
#endif
}

