#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Delay.h" 
#include "LED.h"
#include "APP.h"
#include "USART.h"
#include "Key.h"
int main()
{
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
     LED_Init();
     Delay_init();
     uart_init(9600);
     KEY_Init();
	while(1)
	{
        APP_turn();  
	}
}
