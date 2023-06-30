#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"
#include "usart.h"
#include "motor.h"
#include "PWM.h"
#include "PID.h"
int16_t  machou, Speed;  //实际速度
int pwm = 0;
int PWM1 = 0;
int pwm_val_protect(int pwm_input);
int main(void)
{
	OLED_Init();
	Timer_Init();
	Encoder_Init();
     USART1_Init(9600);
     Motor_Init();
     PWM_Init();
    // PID_Init();
	
	OLED_ShowString(1, 1, "Speed:");
	printf("666\n\r");
	while (1)
	{

         
          

          
          
          printf("PID =%d\n\r",pwm);
          printf("PWM = %d\n\r",PWM1);
          printf("bianma = %d\n\r",Speed);
		OLED_ShowSignedNum(1, 7, Speed, 5);
	}
}

void TIM2_IRQHandler(void)
{

     
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Speed  = Encoder_Get();
          //Speed = (float)machou / TOTAL_RESOLUTION * 10 * 60;
          pwm =  PID_control(Speed);
          PWM1 = pwm_val_protect(pwm);
          if(PWM1<0)
          {
               PWM1 = -PWM1;
          }
          MotorControl(2,PWM1 ,PWM1);
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


