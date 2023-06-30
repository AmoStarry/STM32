#include "PID.h"

/**
  * @brief  PID算法实现
  * @param  actual_val:实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
  
 /*定义一个PID结构体型的全局变量*/
PID pid;

float PID_realize(float actual_val)
{
     
	/*计算目标值与实际值的误差*/
	pid.err = pid.target_val - actual_val;
	
	/*积分项*/
	pid.integral += pid.err;

	/*PID算法实现*/
	pid.output_val = pid.Kp * pid.err + 
				     pid.Ki * pid.integral + 
				     pid.Kd * (pid.err - pid.err_last);

	/*误差传递*/
	pid.err_last = pid.err;

	/*返回当前实际值*/
	return pid.output_val;
}

/*-----------------------外部链接部分----------------------*/
/**
  * @brief  PID参数初始化
  *	@note 	无
  * @retval 无
  */
void PID_Init()
{
	float pid_temp[3];

	/* 初始化参数(连接上位机后, 可实时修改PID参数) */
	pid.target_val=3000;	  //目标值			
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
  //set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid_temp, 3);     // 给通道1发送PID值
#endif
}

