#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Delay.h" 
#include "LED.h"
#include "APP.h"
#include "Key.h"
#include "USART.h"
int main()
{
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
     LED_Init();
     Delay_init();
     //Key_Init();
     //USART1_Init(9600);
	while(1)
	{
        APP_turn();  
	}
}
