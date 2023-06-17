
#ifndef __ENCODE_H
#define __ENCODE_H

#include "stm32f10x.h"                  // Device header
void Encode_Init(void);
void EXTI1_IRQHandler(void);
void EXTI0_IRQHandler(void);
int16_t Encode_Get(void);

#endif
