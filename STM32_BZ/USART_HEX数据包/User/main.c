#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "usart.h"
#include "key.h"
#include "string.h"
#include "LED.h"

uint8_t KeyNum;
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
     
     OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");
	
	Serial_TxPacket[0] = 0x01;
	Serial_TxPacket[1] = 0x02;
	Serial_TxPacket[2] = 0x03;
	Serial_TxPacket[3] = 0x04;
     
	while(1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			Serial_TxPacket[0] ++;
			Serial_TxPacket[1] ++;
			Serial_TxPacket[2] ++;
			Serial_TxPacket[3] ++;
			
			USART_SendPacket();
			
			OLED_ShowHexNum(2, 1, Serial_TxPacket[0], 2);
			OLED_ShowHexNum(2, 4, Serial_TxPacket[1], 2);
			OLED_ShowHexNum(2, 7, Serial_TxPacket[2], 2);
			OLED_ShowHexNum(2, 10, Serial_TxPacket[3], 2);
		}
		
		if (Serial_GetRxFlag() == 1)
		{
			OLED_ShowHexNum(4, 1, Serial_RxPacket[0], 2);
			OLED_ShowHexNum(4, 4, Serial_RxPacket[1], 2);
			OLED_ShowHexNum(4, 7, Serial_RxPacket[2], 2);
			OLED_ShowHexNum(4, 10, Serial_RxPacket[3], 2);
		}
	}
}
