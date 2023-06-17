#ifndef __ENCODER_H
#define __ENCODER_H
#include "stm32f10x.h"                  // Device header
#include "usart.h"
void Encoder_Init(void);
int16_t Encoder_Get(void);
void motor_speed(void);

#endif
