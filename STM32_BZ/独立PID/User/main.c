#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"
#include "usart.h"
#include "motor.h"
#include "PWM.h"
#include "PID.h"
#include "protocol.h"
#include "USAR_PID.h"

#define ENCODER_RESOLUTION 11    /*编码器一圈的物理脉冲数*/
#define ENCODER_MULTIPLE 4       /*编码器倍频，通过定时器的编码器模式设置*/
#define MOTOR_REDUCTION_RATIO 46 /*电机的减速比*/
/*电机转一圈总的脉冲数(定时器能读到的脉冲数) = 编码器物理脉冲数*编码器倍频*电机减速比 */
/* 11*4*34= 1496*/
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 
#define ENCODER_TIM_PERIOD  65535   /*计数器最大值*/
int16_t EncoderOverflowCnt = 0;

int16_t  machou, Speed;  //实际速度
int pwm = 0;
int PWM1 = 0;

//static uint32_t location_timer = 0;    // 位置环周期
	
static int32_t encoderNow = 0;    /*当前时刻总计数值*/
static int32_t encoderNum = 0;   /*上一时刻总计数值*/
int encoderDelta = 0; /*当前时刻与上一时刻编码器的变化量*/
float actual_speed = 0;  /*实际测得速度*/
int actual_speed_int = 0;

int pwm_val_protect(int pwm_input);

int32_t target_speed = 0;
int main(void)
{
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	OLED_Init();
     Motor_Init();
     PWM_Init();
	Timer_Init();
	Encoder_Init();
     protocol_init();
     USART1_Init(115200);
     PID_Init();
     
     #if defined(PID_ASSISTANT_EN)
	/*初始化时，上发stop，同步上位机的启动按钮状态*/
	set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);  

	/*获取默认的目标值*/
	target_speed = (int32_t)get_pid_target();
	/*给通道1发送目标值*/
	set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &target_speed, 1);     
#endif
	
	
	OLED_ShowString(1, 1, "Speed:");
//	printf("666\n\r");
	while (1)
	{

         receiving_process();
          

          
          
//          printf("PID =%d\n\r",pwm);
//          printf("PWM = %d\n\r",PWM1);
//          printf("bianma = %d\n\r",encoderNow);
		//OLED_ShowSignedNum(1, 7, Speed, 5);
	}
}



void TIM2_IRQHandler(void)
{

     
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
          encoderNow = Encoder_Get();             //10us的CNT计数，10us清零一次
          encoderNum += encoderNow;                /*所有计数脉冲的累加的累计值*/
          actual_speed = (float)encoderNow*6000 / TOTAL_RESOLUTION ;
          
          Speed =(int)actual_speed;
          printf("speed =%d\n\r",encoderNow);
          
          pwm = PID_realize(actual_speed);
        //  printf("PID =%d\n\r",pwm);
          PWM1 = pwm_val_protect((int)pwm);
        //  printf("PWM = %d\n\r",PWM1);
          if(PWM1>0)
          {
            PWM1 =PWM1;
            MotorControl(1,PWM1 ,PWM1);
          }
          else if(PWM1<0)
          {
               PWM1 = -PWM1;
               MotorControl(2,PWM1 ,PWM1);
          }
          else if(PWM1 == 0)
          {
               MotorControl(3,0,0);
          }
          
static int i=0;          
#if (PID_ASSISTANT_EN)
	i++;
	if(i==12)
	{
		i=0;
		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &Speed, 1); /*给通道1发送实际的电机【速度】值*/
		//set_computer_value(SEND_FACT_CMD, CURVES_CH1, &encoderNow, 1); /*给通道1发送实际的电机【位置】值*/
	}
#else
	i++;
	if(i==100)
	{
		i=0;
		printf("sum:%d set_pwm:%d\r\n",sum,res_pwm);
		
	}
#endif
          
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
int pwm_val_protect(int pwm_input)
{
	int pwm_output = 0;
	
	//占空比最大值判断
	if(pwm_input>999) 
	{
		pwm_output = 999;
	}
	else if(pwm_input<-999) 
	{
		pwm_output = -999;
	}
	//占空比太小可能烧电机，太小则不输出
	else if((pwm_input>-100)&&(pwm_input<100)) 
	{
		pwm_output = 0;
	}
	else
	{
		pwm_output = pwm_input;
	}
	
	return pwm_output;
}


