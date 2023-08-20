#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Delay.h" 
#include "LED.h"
#include "APP.h"
#include "stm32f10x_it.h"
int main()
{	
     SysTick_Init();
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
     LED_Init();
     
	while(1)
	{      
        APP_turn();  
	}
}
