 #include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "usart.h"
#include "Encoder.h"
#include "PWM.h"
#include "motor.h"

int16_t Speed;

int main(void)
{
	OLED_Init();
	Timer_Init();
	Encoder_Init();
     PWM_Init();
     Motor_Init();
	USART1_Init(9600);	
	while (1)
	{
          //TIM_SetCompare4(TIM1,50);
          //printf("666/n/r");
          MotorControl(1,1000,1000);
		
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Speed = Encoder_Get();
          printf("speed = %d/n/r",Speed);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

