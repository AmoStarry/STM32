#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "usart.h"
#include "key.h"
#include "string.h"
#include "LED.h"

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
		if (Serial_RxFlag == 1)
		{
			OLED_ShowString(4, 1, "                ");
			OLED_ShowString(4, 1, Serial_RxPacket);
			
			if (strcmp(Serial_RxPacket, "LED_ON") == 0)
			{
				LED1_ON(); 
				USART_SendString(USART1,"LED_ON_OK\r\n");
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "LED_ON_OK");
			}
			else if (strcmp(Serial_RxPacket, "LED_OFF") == 0)
			{
				LED1_OFF();
				USART_SendString(USART1,"LED_OFF_OK\r\n");
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "LED_OFF_OK");
			}
			else
			{
				USART_SendString(USART1,"ERROR_COMMAND\r\n");
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "ERROR_COMMAND");
			}
			
			Serial_RxFlag = 0;
		}
	}
	
}
