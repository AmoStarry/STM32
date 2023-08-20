#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Delay.h" 
#include "LED.h"
#include "APP.h"
int main()
{	
     LED_Init();
     
	while(1)
	{
          LED1_ON();
          LED2_ON();
          Delay_ms(100);
          LED1_OFF();
          LED2_OFF();
          Delay_ms(100);
          
        //APP_turn();  
	}
}
