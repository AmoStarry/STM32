#include "USAR_PID.h"

#include "Timer.h"

extern PID pid;
void set_p_i_d(float p, float i, float d)
{
	pid.Kp = p;    // 设置比例系数 P
	pid.Ki = i;    // 设置积分系数 I
	pid.Kd = d;    // 设置微分系数 D
}

/**
  * @brief  设置目标值
  * @param  val		目标值
  *	@note 	无
  * @retval 无
  */
void set_pid_target(float temp_val)
{  
	pid.target_val = temp_val;    // 设置当前的目标值
}

float get_pid_target(void)
{
	return pid.target_val;    // 设置当前的目标值
}


