#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "usart.h"

uint8_t add[5] ={0x41,0x11,0x12,0x13,0x14};
uint8_t RxData;
int main()
{
     USART1_Init(9600);
	
	OLED_Init();
	USART_SendBit(USART1,0x41);
     USART_SendArray(USART1,add,5);
     USART_SendString(USART1,"HellowWorld!\n\r");
     USART_SendNumber(USART1,28,2);
     printf("\n\rNum=%d\n\r",666);
     
	while(1)
	{
		
	}
}
