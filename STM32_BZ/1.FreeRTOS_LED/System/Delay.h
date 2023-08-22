#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f10x.h"                  // Device header

#define SYSTEM_SUPPORT_OS		1	
void Delay_init(void);
void Delay_ms(u32 nms);
void Delay_us(u32 nus);
void Delay_xms(u32 nms);

#endif
