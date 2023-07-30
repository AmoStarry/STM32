#ifndef __UARTDATA_H
#define __UARTDATA_H

#include "system.h"

typedef struct
{
	
	uint8_t	head1;							// 帧头1
	uint8_t	head2;							// 帧头2
	uint8_t length;							// 有效数据长度
	uint8_t data[40];						// 有效数据组成的数组
	uint8_t transmit_data[50];				// 实际发送的数组 附带上帧头1 帧头2 有效数据长度位 校验位
	
}DataTransmit;

void Data_Init_Transmit(DataTransmit *data, uint8_t head1, uint8_t head2, uint8_t length);
void Data_Pack(DataTransmit *data);
void Data_Transmit(UART_HandleTypeDef *huart, DataTransmit *data);
void Data_Pack_Transmit(UART_HandleTypeDef *huart, DataTransmit *data);

typedef struct
{
	
	uint8_t	head1;							// 帧头1
	uint8_t	head2;							// 帧头2
	uint8_t length;							// 有效数据长度
	uint8_t i;								// 有效数据下标
	uint8_t cnt;							// 总数据长度
	uint8_t state;							// 接收状态
	uint8_t receive_data[50];				// 实际接收的数组 附带上帧头1 帧头2 有效数据长度位 校验位
	uint8_t data;							// 接收数据
	
}DataReceive;

void Data_Init_Receive(DataReceive *data, uint8_t head1, uint8_t head2);
uint8_t Buffer_Receive(UART_HandleTypeDef *huart, DataReceive *data);
void Data_Receive(DataReceive *data, uint8_t buf);

typedef struct
{
	
	uint16_t	x;							// 目标x轴坐标
	uint16_t	y;							// 目标y轴坐标
	uint8_t color;							// 目标颜色标志位
	uint8_t shape;							// 目标形状标志位
	uint8_t flag;							// 目标标志位
	
}TargetAttribute;

void Target_Init(TargetAttribute *target);
void Target_Parse(DataReceive *data,TargetAttribute *target);

#endif

