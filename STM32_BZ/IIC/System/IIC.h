#ifndef __IIC_H
#define __IIC_H
#include "stm32f10x.h"                  // Device header

void IIC_W_SCL(uint8_t BitValue);
void IIC_W_SDA(uint8_t BitValue);
uint8_t IIC_R_SDA(void);
void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendByte(uint8_t Byte);
uint8_t IIC_ReceiveByte(void);
void IIC_SendAck(uint8_t AckBit);
uint8_t IIC_ReceiveAck(void);

#endif
