#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Delay.h" 
#include "LED.h"
#include "APP.h"
int main()
{
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
     LED_Init();
     Delay_init();

     
	while(1)
	{
        APP_turn();  
	}
}
