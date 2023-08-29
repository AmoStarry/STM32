#include "usart.h"
 u16 r;
uint8_t USART_TxPacket[4];
uint8_t USART_RxPacket[4];

/*************************************************
*��������     USART1_Init
*�������ܣ�   ����1�ĳ�ʼ��
*���룺       bound��������   
*����ֵ��     ��
**************************************************/

//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOSʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                  
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fput��c���� 
int fputc(int ch, FILE *f)
{      
    while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void uart_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
			{
			if(USART_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
} 
#endif



//void USART1_Init(uint32_t bound)
//{
//     GPIO_InitTypeDef GPIO_InitStruct;
//     USART_InitTypeDef USART_InitStruct;
//     NVIC_InitTypeDef NVIC_InitStruct;
//     
//     RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ʱ��ʹ��
//     RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//     
//     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
//     GPIO_InitStruct.GPIO_Pin = USART1_TX_Pin;    //GPIOB 9 ��������
//     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�����ٶ�
//     GPIO_Init(GPIOA,&GPIO_InitStruct);            //���ų�ʼ��
//     
//     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //��������
//     GPIO_InitStruct.GPIO_Pin = USART1_RX_Pin;          //GPIOB 10��������
//     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(GPIOA,&GPIO_InitStruct); //���ų�ʼ��
//     
//     
//     USART_InitStruct.USART_BaudRate = bound; //������
//     USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
//     USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx; //�շ�ģʽ
//     USART_InitStruct.USART_Parity = USART_Parity_No; //û��У��λ
//     USART_InitStruct.USART_StopBits = USART_StopBits_1; //һλֹͣλ
//     USART_InitStruct.USART_WordLength = USART_WordLength_8b; //8λһ���ֽ�
//     USART_Init(USART1,&USART_InitStruct); //��ʼ������
//     
//     USART_Cmd(USART1,ENABLE); //����ʹ��
//     
//     USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //�����ж�ʹ��   (�в�ѯ���ж����ַ���ʵ�ֽ���)
//     NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;  //Ҫ�򿪵��ж�ͨ��
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2; //��ռʽ���ȼ�
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;   //��Ӧʽ���ȼ�
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //NVICͨ��ʹ��
//	NVIC_Init(&NVIC_InitStruct);
//	
//	USART_ClearFlag(USART1, USART_FLAG_TC); //TCλ��ʼֵλ1��Ҫ����0


//     
//}
////ֱ��ʹ�ô��ڷ��ͽ��պ�����������ݸ��ǵ����⣬������Ҫ������д����
///*************************************************
//*��������      USART_SendBit
//*�������ܣ�    ���ڷ��ͺ���
//*���룺        USARTx:����  Data�����͵�����
//*����ֵ��      ��
//**************************************************/
//void USART_SendBit(USART_TypeDef* USARTx,uint16_t Data)   
//{
//	USART_SendData(USARTx, Data);
//	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
////	while(!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));  //Ҫ�ȴ�����ȫ��ת����λ�Ĵ���
////	USART_ClearFlag(USARTx, USART_FLAG_TXE); //��ձ�־λ
//	
////    while(!USART_GetFlagStatus(USARTx, USART_FLAG_TC));//Ҫ�ȴ�����ȫ������
////    USART_ClearFlag(USARTx, USART_FLAG_TC); //��ձ�־λ
//}


///*************************************************
//*��������         USART1_IRQHandler
//*�������ܣ�       �����жϺ���-���ӵ��Է��͵����ݷ��ظ�����
//*���룺           ��
//*����ֵ��         ��
//**************************************************/
//void USART1_IRQHandler(void)
//{
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)  //�ж��жϱ�־
//	{
//		r = USART_ReceiveData(USART1);
//		USART_SendBit(USART1,r);
//	}
//}

///*************************************************
//*��������       USART_ReceiveBit
//*�������ܣ�     ���ڽ��պ���
//*���룺         USARTx:����
//*����ֵ��       ���յ�������
//**************************************************/
//uint16_t USART_ReceiveBit(USART_TypeDef* USARTx)
//{
//	while(!USART_GetFlagStatus(USARTx, USART_FLAG_RXNE)); //�ȴ����յ�����ȫ������
//	USART_ClearFlag(USARTx, USART_FLAG_RXNE);
//	return USART_ReceiveData(USARTx);
//}


//void USART_SenndPaket(void)
//{
//     USART_SendBit(USART1,0xFF);
//     USART_SendArray(USART1,USART_TxPacket,4);
//     USART_SendBit(USART1,0xFE);
//     
//}



///*************************************************
//*��������      USART_SendArray
//*�������ܣ�    ���ڷ��ͺ���
//*���룺        USARTx:����  Array:������  Length:����
//*����ֵ��      ��
//**************************************************/

//void USART_SendArray(USART_TypeDef* USARTx,uint8_t *Array, uint16_t Length)
//{
//	uint16_t i;
//	for (i = 0; i < Length; i ++)
//	{
//		USART_SendBit(USARTx,Array[i]);
//	}
//}

///*************************************************
//*��������        USART_SendString
//*�������ܣ�      ���ڷ����ַ�������
//*���룺          USARTx:���ڣ�string���ַ���ָ��
//*����ֵ��        ��
//**************************************************/
//void USART_SendString(USART_TypeDef* USARTx,char *string)
//{
//	while(*string)
//	{
//		USART_SendBit(USARTx,*string++);
//	}
//}

///*************************************************
//*��������        USART_SendString
//*�������ܣ�      ���ڷ����ַ�������
//*���룺          USARTx:���ڣ�Number:����  Length������
//*����ֵ��        ��
//**************************************************/

//uint32_t Serial_Pow(uint32_t X, uint32_t Y)
//{
//	uint32_t Result = 1;
//	while (Y --)
//	{
//		Result *= X;
//	}
//	return Result;
//}

//void USART_SendNumber(USART_TypeDef* USARTx,uint32_t Number, uint8_t Length)
//{
//	uint16_t i,Num;
//	for (i = 0; i < Length; i ++)
//	{
//          Num = Number / Serial_Pow(10, Length - i - 1) % 10 + '0';
//		USART_SendBit(USARTx,Num);
//	}
//}

///*************************************************
//*��������        fputc
//*�������ܣ�      ת��printf���ڵ������
//*���룺          ��
//*����ֵ��        ��
//**************************************************/
//int fputc(int ch, FILE *f)
//{
//	USART_SendBit(USART1,ch);
//	return ch;
//}

///*************************************************
//*��������        USART_Printf
//*�������ܣ�      �Է�װ���ڴ�ӡ
//*���룺          �̶�ʹ��USART1  
//*����ֵ��        ��
//**************************************************/
//void USART_Printf(char *format, ...)
//{
//	char String[100];
//	va_list arg;
//	va_start(arg, format);
//	vsprintf(String, format, arg);
//	va_end(arg);
//	USART_SendString(USART1,String);
//}
