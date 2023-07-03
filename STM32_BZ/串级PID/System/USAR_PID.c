#include "USAR_PID.h"

#include "Timer.h"

extern PID pid;
void set_p_i_d(float p, float i, float d)
{
	pid.Kp = p;    // ���ñ���ϵ�� P
	pid.Ki = i;    // ���û���ϵ�� I
	pid.Kd = d;    // ����΢��ϵ�� D
}

/**
  * @brief  ����Ŀ��ֵ
  * @param  val		Ŀ��ֵ
  *	@note 	��
  * @retval ��
  */
void set_pid_target(float temp_val)
{  
	pid.target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
}

float get_pid_target(void)
{
	return pid.target_val;    // ���õ�ǰ��Ŀ��ֵ
}


