#include "stm32f10x.h" 
#include "Delay.h"
int main()
{
	GPIO_InitTypeDef  GPIO_Initstructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	//GPIO_SetBits(GPIOA,GPIO_Pin_8);
	//GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	while(1)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);
		Delay_ms(100);
		GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);
		Delay_ms(100);
	}
}
