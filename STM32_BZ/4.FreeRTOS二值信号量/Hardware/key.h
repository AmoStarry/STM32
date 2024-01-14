//#ifndef __KEY_H__
//#define __KEY_H__

//void Key_Init(void);
//uint8_t Key_GetNum(void);

//#endif


#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f10x.h"                  // Device header
 
#define KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)//读取按键1
#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 
 

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define WKUP_PRES	3		//WK_UP  

void KEY_Init(void);//IO初始化
uint8_t Key_Num(void);					    
#endif
