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
    
	OLED_ShowString(1,1,"Num");
     OLED_ShowString(2,1,"Cnt");
	OLED_Clear();
	while(1)
	{
		OLED_ShowHexNum(1,5,Num,5);
		OLED_ShowHexNum(2,5,Timer_GetCounter(),5);
	}
}
