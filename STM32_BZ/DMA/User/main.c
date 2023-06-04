#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "DMA.h"

uint8_t DataA[] = {0x01,0x02,0x03,0x04};
uint8_t DataB[] = {0,0,0,0};

uint8_t KeyNum;
int main()
{
	
	OLED_Init();
     OLED_ShowString(1,1,"DataA");
     OLED_ShowString(3,1,"DataB");
     OLED_ShowHexNum(1,8,(uint32_t)DataA,8);
     OLED_ShowHexNum(3,8,(uint32_t)DataB,8);
     
     
	while(1)
	{
		
		
	}
}
