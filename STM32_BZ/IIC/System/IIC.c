#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void IIC_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
	Delay_us(10);  
}

void IIC_W_SDA(uint8_t BitValue)
{
     GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	Delay_us(10);
}

uint8_t IIC_R_SDA(void)
{
     uint8_t BitValue;
     BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
     Delay_us(10);
     return BitValue; 
}

void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

void IIC_Start(void) //开始
{
	IIC_W_SDA(1);
	IIC_W_SCL(1);
	IIC_W_SDA(0);
	IIC_W_SCL(0);
}

void IIC_Stop(void) //停止
{
	IIC_W_SDA(0);
	IIC_W_SCL(1);
	IIC_W_SDA(1);
}

void IIC_SendByte(uint8_t Byte) //发送数据
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		IIC_W_SDA(Byte & (0x80 >> i));
		IIC_W_SCL(1);
		IIC_W_SCL(0);
	}
}

uint8_t IIC_ReceiveByte(void)  //接收数据
{
	uint8_t i, Byte = 0x00;
	IIC_W_SDA(1);
	for (i = 0; i < 8; i ++)
	{
		IIC_W_SCL(1);
		if (IIC_R_SDA() == 1){Byte |= (0x80 >> i);}
		IIC_W_SCL(0);
	}
	return Byte;
}


void IIC_SendAck(uint8_t AckBit) //发送应答位
{
	IIC_W_SDA(AckBit);
	IIC_W_SCL(1);
	IIC_W_SCL(0);
}


uint8_t IIC_ReceiveAck(void) //接收应答位
{
	uint8_t AckBit;
	IIC_W_SDA(1);
	IIC_W_SCL(1);
	AckBit = IIC_R_SDA();
	IIC_W_SCL(0);
	return AckBit;
}

