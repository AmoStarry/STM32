#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

uint8_t KeyNum;
uint16_t Num;
int main()
{
	
	OLED_Init();
     Timer_Iint();
     
	OLED_ShowChar(1,1,'A');
	OLED_ShowSignedNum(2,7,-66,2);
	
	OLED_ShowBinNum(4,1,0xAA66,16);
	OLED_ShowString(1,3,"HwlloWorld");
	OLED_Clear();
	while(1)
	{
		OLED_ShowHexNum(1,5,Num,5);
		
	}
}
