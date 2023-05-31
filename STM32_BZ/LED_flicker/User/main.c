#include "stm32f10x.h"
#include "Delay.h" 
int main()
{	
	GPIO_InitTypeDef  GPIO_Initstructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Initstructure);
	while(1)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		Delay_ms(500);
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
		Delay_ms(500);
		/*方法二：
		GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);//((BitAction)0)强制类型转换
		Delay_ms(500);
		GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);//((BitAction)1)
		Delay_ms(500);*/
	}
}