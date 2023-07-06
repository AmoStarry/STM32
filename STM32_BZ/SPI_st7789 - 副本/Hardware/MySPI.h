#ifndef __MYSPI_H
#define __MYSPI_H
#include "stm32f10x.h"                  // Device header


void initlcd(void);
void writeData(u8 data);
void writeCommand(u8 data);
void fillScreen(u16 color);
void SPI1_Init(void);	
void SPI1_SetSpeed(u8 SpeedSet);   
u8 SPI1_ReadWriteByte(u8 TxData);
void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);

#endif
