#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Sensor.h"


uint8_t KeyNum;
int main()
{
	
	OLED_Init();
	OLED_ShowString(1,1,"Count");
     Sen_Init();
	while(1)
	{
		
		OLED_ShowNum(1,7,CountSensor_Get(),5);
	}
}
