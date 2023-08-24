#ifndef USART_H_
#define USART_H_
#include <stdio.h>
#include <stdarg.h>
#include "stm32f10x.h"

//extern uint8_t USART_TxPacket[4];
//extern uint8_t USART_RxPacket[4];
///************串口引脚************/
//#define USART1_GPIO_Port  GPIOA
//#define USART1_RX_Pin     GPIO_Pin_3
//#define USART1_TX_Pin     GPIO_Pin_2

///************串口函数************/
//void USART1_Init(u32 bound);          //串口初始化
//void USART_SendBit(USART_TypeDef* USARTx,u16 Data);   //发送单个数据
//uint16_t USART_ReceiveBit(USART_TypeDef* USARTx);     //接收单个数据
//void USART_SendString(USART_TypeDef* USARTx,char *string); //发送字符串
//void USART_SenndPaket(void);
//void USART1_IRQHandler(void);
//void USART_SendArray(USART_TypeDef* USARTx,uint8_t *Array, uint16_t Length);//发送数组

//uint32_t Serial_Pow(uint32_t X, uint32_t Y);
//void USART_SendNumber(USART_TypeDef* USARTx,uint32_t Number, uint8_t Length);//发送数字
//int fputc(int ch, FILE *f);
//void USART_Printf(char *format, ...);//打印
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
#endif


