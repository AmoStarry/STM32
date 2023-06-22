#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "usart.h"
#include "led.h"
extern u16 r;
uint8_t add[5] ={0x41,0x11,0x12,0x13,0x14};
uint8_t RxData;
int main()
{
     USART1_Init(9600);
	LED_Init();
	OLED_Init();
     printf("\n\rNum=%d\n\r",666);
     
	while(1)
	{
		if(r== '1')
          {
               //printf("\n\rNum=%d\n\r",777);
               LED1_ON();
               
               
          }
          else if(r == '0')
          {
               //printf("\n\rNum=%d\n\r",888);
               LED1_OFF();
              
          }
	}
}
