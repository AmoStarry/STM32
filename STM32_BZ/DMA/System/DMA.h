#ifndef __DMA_H
#define __DMA_H
#include "stm32f10x.h"                  // Device header

void MyDMA_Init(uint32_t AddrA,uint32_t AddrB,uint16_t Size);
void DMA_Transfer(void);
#endif
