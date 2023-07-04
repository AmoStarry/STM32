#include "USAR_PID.h"

#include "Timer.h"

/**
  * @brief  设置比例、积分、微分系数
  * @param  p：比例系数 P
  * @param  i：积分系数 i
  * @param  d：微分系数 d
  *	@note 	无
  * @retval 无
  */
void set_p_i_d(PID *pid, float p, float i, float d)
{
	pid->Kp = p;    // 设置比例系数 P
	pid->Ki = i;    // 设置积分系数 I
	pid->Kd = d;    // 设置微分系数 D
}


/**
  * @brief  设置目标值
  * @param  val		目标值
  *	@note 	无
  * @retval 无
  */
void set_pid_target(PID *pid, float temp_val)
{  
	pid->target_val = temp_val;    // 设置当前的目标值
}



/**
  * @brief  获取目标值
  * @param  无
  *	@note 	无
  * @retval 目标值
  */
float get_pid_target(PID *pid)
{
  return pid->target_val;    // 获取当前的目标值
}


//目标速度值限制
//#define TARGET_SPEED_MAX   60 // 目标速度的最大值 r/m
static float TARGET_SPEED_MAX = 100;
void speed_val_protect(float *speed_val)
{
	/*目标速度上限处理*/
	if (*speed_val > TARGET_SPEED_MAX)
	{
		*speed_val = TARGET_SPEED_MAX;
	}
	else if (*speed_val < -TARGET_SPEED_MAX)
	{
		*speed_val = -TARGET_SPEED_MAX;
	}	
}

/*上位机目标值获取与设置*/
void SetTargetMaxSpeed(int speed)
{
	TARGET_SPEED_MAX = (float)speed;
}
int GetTargetMaxSpeed(void)
{
	return (int)TARGET_SPEED_MAX;
}
