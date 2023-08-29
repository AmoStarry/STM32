#ifndef USART_H_
#define USART_H_
#include <stdio.h>
#include <stdarg.h>
#include "stm32f10x.h"

//extern uint8_t USART_TxPacket[4];
//extern uint8_t USART_RxPacket[4];
///************��������************/
//#define USART1_GPIO_Port  GPIOA
//#define USART1_RX_Pin     GPIO_Pin_3
//#define USART1_TX_Pin     GPIO_Pin_2

///************���ں���************/
//void USART1_Init(u32 bound);          //���ڳ�ʼ��
//void USART_SendBit(USART_TypeDef* USARTx,u16 Data);   //���͵�������
//uint16_t USART_ReceiveBit(USART_TypeDef* USARTx);     //���յ�������
//void USART_SendString(USART_TypeDef* USARTx,char *string); //�����ַ���
//void USART_SenndPaket(void);
//void USART1_IRQHandler(void);
//void USART_SendArray(USART_TypeDef* USARTx,uint8_t *Array, uint16_t Length);//��������

//uint32_t Serial_Pow(uint32_t X, uint32_t Y);
//void USART_SendNumber(USART_TypeDef* USARTx,uint32_t Number, uint8_t Length);//��������
//int fputc(int ch, FILE *f);
//void USART_Printf(char *format, ...);//��ӡ
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
#endif


