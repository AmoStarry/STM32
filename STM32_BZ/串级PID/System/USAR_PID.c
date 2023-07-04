#include "USAR_PID.h"

#include "Timer.h"

/**
  * @brief  ���ñ��������֡�΢��ϵ��
  * @param  p������ϵ�� P
  * @param  i������ϵ�� i
  * @param  d��΢��ϵ�� d
  *	@note 	��
  * @retval ��
  */
void set_p_i_d(PID *pid, float p, float i, float d)
{
	pid->Kp = p;    // ���ñ���ϵ�� P
	pid->Ki = i;    // ���û���ϵ�� I
	pid->Kd = d;    // ����΢��ϵ�� D
}


/**
  * @brief  ����Ŀ��ֵ
  * @param  val		Ŀ��ֵ
  *	@note 	��
  * @retval ��
  */
void set_pid_target(PID *pid, float temp_val)
{  
	pid->target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
}



/**
  * @brief  ��ȡĿ��ֵ
  * @param  ��
  *	@note 	��
  * @retval Ŀ��ֵ
  */
float get_pid_target(PID *pid)
{
  return pid->target_val;    // ��ȡ��ǰ��Ŀ��ֵ
}


//Ŀ���ٶ�ֵ����
//#define TARGET_SPEED_MAX   60 // Ŀ���ٶȵ����ֵ r/m
static float TARGET_SPEED_MAX = 100;
void speed_val_protect(float *speed_val)
{
	/*Ŀ���ٶ����޴���*/
	if (*speed_val > TARGET_SPEED_MAX)
	{
		*speed_val = TARGET_SPEED_MAX;
	}
	else if (*speed_val < -TARGET_SPEED_MAX)
	{
		*speed_val = -TARGET_SPEED_MAX;
	}	
}

/*��λ��Ŀ��ֵ��ȡ������*/
void SetTargetMaxSpeed(int speed)
{
	TARGET_SPEED_MAX = (float)speed;
}
int GetTargetMaxSpeed(void)
{
	return (int)TARGET_SPEED_MAX;
}
