#include "stm32f10x.h"

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_2);
} 

void LED1_ON(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}

void LED1_OFF(void)
{
    GPIO_SetBits(GPIOA,GPIO_Pin_8);
}

void LED1_Turn(void)
{
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) ==0)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_8);
    }
    else
    {
        GPIO_ResetBits(GPIOA,GPIO_Pin_8);
    }
}

void LED2_ON(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}

void LED2_OFF(void)
{
    GPIO_SetBits(GPIOA,GPIO_Pin_2);
}

void LED2_Turn(void)
{
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2) ==0)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_2);
    }
    else
    {
        GPIO_ResetBits(GPIOA,GPIO_Pin_2);
    }
}
