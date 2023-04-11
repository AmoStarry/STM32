#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "Key.h"
int main()
{
	LED_Init();
	Key_Init();
	while(1)
	{
		LED1_ON();
		Delay_ms(500);
		LED1_OFF();
		Delay_ms(500);
		LED2_ON();
		Delay_ms(500);
		LED2_OFF();
		Delay_ms(500);
		
		
	}
}
