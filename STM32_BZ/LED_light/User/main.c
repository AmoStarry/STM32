#include "stm32f10x.h" 
#include "Delay.h"
int main()
{
	GPIO_InitTypeDef  GPIO_Initstructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	//GPIO_SetBits(GPIOA,GPIO_Pin_8);
	//GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	while(1)
	{
		GPIO_Write(GPIOA,~0x00001);//对应GPIOA的16个引脚
		Delay_ms(500); 
		GPIO_Write(GPIOA,~0x00002);//对应GPIOA的16个引脚
		Delay_ms(500); 
		GPIO_Write(GPIOA,~0x00004);//对应GPIOA的16个引脚
		Delay_ms(500); 
		GPIO_Write(GPIOA,~0x00008);//对应GPIOA的16个引脚
		Delay_ms(500); 
		GPIO_Write(GPIOA,~0x00010);//对应GPIOA的16个引脚
		Delay_ms(500); 
		GPIO_Write(GPIOA,~0x00020);//对应GPIOA的16个引脚
		Delay_ms(500); 
		GPIO_Write(GPIOA,~0x00040);//对应GPIOA的16个引脚
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x00080);//对应GPIOA的16个引脚
		Delay_ms(500); 
	
	}
}

