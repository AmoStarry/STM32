


/*定义位置PID与速度PID结构体型的全局变量*/
PID pid_location;
PID pid_speed;

/**
  * @brief  PID参数初始化
  *	@note 	无
  * @retval 无
  */
void PID_param_init()
{
	/* 位置相关初始化参数 */
	pid_location.target_val = TOTAL_RESOLUTION*10;				
	pid_location.output_val = 0.0;
	pid_location.err = 0.0;
	pid_location.err_last = 0.0;
	pid_location.integral = 0.0;

	pid_location.Kp = 0.05;
	pid_location.Ki = 0;
	pid_location.Kd = 0;

	/* 速度相关初始化参数 */
	pid_speed.target_val=10.0;				
	pid_speed.output_val=0.0;
	pid_speed.err=0.0;
	pid_speed.err_last=0.0;
	pid_speed.integral=0.0;

	pid_speed.Kp = 80.0;
	pid_speed.Ki = 2.0;
	pid_speed.Kd = 100.0;

#if defined(PID_ASSISTANT_EN)
	{
		float pid_temp[3] = {0};
		
		/*给通道1发送位置PID值*/
		pid_temp[0] = pid_location.Kp;
		pid_temp[1] = pid_location.Ki;
		pid_temp[2] = pid_location.Kd;
		set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid_temp, 3);     
		
		/*给通道2发送速度PID值*/
		pid_temp[0] = pid_speed.Kp;
		pid_temp[1] = pid_speed.Ki;
		pid_temp[2] = pid_speed.Kd;
		set_computer_value(SEND_P_I_D_CMD, CURVES_CH2, pid_temp, 3);  
	}	
#endif
}