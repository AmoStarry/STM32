#include "stm32f10x.h"                  // Device header

uint16_t Encode_count;

void Encode_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
     EXTI_InitTypeDef EXTI_InitStructure;
     NVIC_InitTypeDef NVIC_InitStructure;
     
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
     
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOB,&GPIO_InitStructure);
     
     GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
     GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
     
     EXTI_InitStructure.EXTI_Line = EXTI_Line0|EXTI_Line1;
     EXTI_InitStructure.EXTI_LineCmd = ENABLE;
     EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
     EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
     EXTI_Init(&EXTI_InitStructure);
     
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
     
     NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
     
     NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
     NVIC_Init(&NVIC_InitStructure);
}

void EXTI0_IRQHandler(void)
{
     if(EXTI_GetFlagStatus(EXTI_Line0) == SET)
     {
          if(GPIO_ReadInputDataBit (GPIOB,GPIO_Pin_0) ==0)
          { 
               Encode_count --;
          }
          EXTI_ClearITPendingBit(EXTI_Line0);
          
     }
}

void EXTI1_IRQHandler(void)
{
     if(EXTI_GetFlagStatus(EXTI_Line1) == SET)
     {
          if(GPIO_ReadInputDataBit (GPIOB,GPIO_Pin_1) ==0)
          { 
               Encode_count ++;
          }
          EXTI_ClearITPendingBit(EXTI_Line1);
          
     }
}

int16_t Encode_Get(void)
{
     int16_t Temp;
     Temp = Encode_count;
     Encode_count =0;
     return Temp;
}

