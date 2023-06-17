#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Encode.h"


int16_t Num;
uint8_t KeyNum;
int main()
{
	
	OLED_Init();
     Encode_Init();
	OLED_ShowString(1,3,"Num");
	OLED_Clear();
	while(1)
	{
		Num +=Encode_Get();
          OLED_ShowHexNum(1,5,Num,5);
		
	}
}
