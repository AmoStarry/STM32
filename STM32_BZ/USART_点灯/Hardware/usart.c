#include "usart.h"

char Serial_RxPacket[100];				//"@MSG\r\n"
uint8_t Serial_RxFlag;

/*************************************************
*��������     USART1_Init
*�������ܣ�   ����1�ĳ�ʼ��
*���룺       bound��������   
*����ֵ��     ��
**************************************************/

void USART1_Init(uint32_t bound)
{
     GPIO_InitTypeDef GPIO_InitStruct;
     USART_InitTypeDef USART_InitStruct;
     NVIC_InitTypeDef NVIC_InitStruct;
     
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ʱ��ʹ��
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
     
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
     GPIO_InitStruct.GPIO_Pin = USART1_TX_Pin;    //GPIOB 9 ��������
     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�����ٶ�
     GPIO_Init(GPIOA,&GPIO_InitStruct);            //���ų�ʼ��
     
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //��������
     GPIO_InitStruct.GPIO_Pin = USART1_RX_Pin;          //GPIOB 10��������
     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOA,&GPIO_InitStruct); //���ų�ʼ��
     
     
     USART_InitStruct.USART_BaudRate = bound; //������
     USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
     USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx; //�շ�ģʽ
     USART_InitStruct.USART_Parity = USART_Parity_No; //û��У��λ
     USART_InitStruct.USART_StopBits = USART_StopBits_1; //һλֹͣλ
     USART_InitStruct.USART_WordLength = USART_WordLength_8b; //8λһ���ֽ�
     USART_Init(USART1,&USART_InitStruct); //��ʼ������
     
     USART_Cmd(USART1,ENABLE); //����ʹ��
     
     USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //�����ж�ʹ��   (�в�ѯ���ж����ַ���ʵ�ֽ���)
     NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;  //Ҫ�򿪵��ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2; //��ռʽ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;   //��Ӧʽ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //NVICͨ��ʹ��
	NVIC_Init(&NVIC_InitStruct);
	
	USART_ClearFlag(USART1, USART_FLAG_TC); //TCλ��ʼֵλ1��Ҫ����0


     
}

/*************************************************
*��������         USART1_IRQHandler
*�������ܣ�       �����жϺ���-���ӵ��Է��͵����ݷ��ظ�����
*���룺           ��
*����ֵ��         ��
**************************************************/
void USART1_IRQHandler(void)
{
     int on,off;
     static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)  //�ж��жϱ�־
	{
		
          uint8_t RxData = USART_ReceiveData(USART1);
		
     }
}

/*************************************************
*��������       USART_ReceiveBit
*�������ܣ�     ���ڽ��պ���
*���룺         USARTx:����
*����ֵ��       ���յ�������
**************************************************/
uint16_t USART_ReceiveBit(USART_TypeDef* USARTx)
{
	while(!USART_GetFlagStatus(USARTx, USART_FLAG_RXNE)); //�ȴ����յ�����ȫ������
	USART_ClearFlag(USARTx, USART_FLAG_RXNE);
	return USART_ReceiveData(USARTx);
}


//ֱ��ʹ�ô��ڷ��ͽ��պ�����������ݸ��ǵ����⣬������Ҫ������д����
/*************************************************
*��������      USART_SendBit
*�������ܣ�    ���ڷ��ͺ���
*���룺        USARTx:����  Data�����͵�����
*����ֵ��      ��
**************************************************/
void USART_SendBit(USART_TypeDef* USARTx,uint16_t Data)   
{
	USART_SendData(USARTx, Data);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
//	while(!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));  //Ҫ�ȴ�����ȫ��ת����λ�Ĵ���
//	USART_ClearFlag(USARTx, USART_FLAG_TXE); //��ձ�־λ
	
//    while(!USART_GetFlagStatus(USARTx, USART_FLAG_TC));//Ҫ�ȴ�����ȫ������
//    USART_ClearFlag(USARTx, USART_FLAG_TC); //��ձ�־λ
}




//void USART_SenndPaket(void)
//{
//     USART_SendBit(USART1,0xFF);
//     USART_SendArray(USART1,USART_TxPacket,4);
//     USART_SendBit(USART1,0xFE);
//     
//}



/*************************************************
*��������      USART_SendArray
*�������ܣ�    ���ڷ��ͺ���
*���룺        USARTx:����  Array:������  Length:����
*����ֵ��      ��
**************************************************/

void USART_SendArray(USART_TypeDef* USARTx,uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		USART_SendBit(USARTx,Array[i]);
	}
}

/*************************************************
*��������        USART_SendString
*�������ܣ�      ���ڷ����ַ�������
*���룺          USARTx:���ڣ�string���ַ���ָ��
*����ֵ��        ��
**************************************************/
void USART_SendString(USART_TypeDef* USARTx,char *string)
{
	while(*string)
	{
		USART_SendBit(USARTx,*string++);
	}
}

/*************************************************
*��������        USART_SendString
*�������ܣ�      ���ڷ����ַ�������
*���룺          USARTx:���ڣ�Number:����  Length������
*����ֵ��        ��
**************************************************/

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void USART_SendNumber(USART_TypeDef* USARTx,uint32_t Number, uint8_t Length)
{
	uint16_t i,Num;
	for (i = 0; i < Length; i ++)
	{
          Num = Number / Serial_Pow(10, Length - i - 1) % 10 + '0';
		USART_SendBit(USARTx,Num);
	}
}

/*************************************************
*��������        fputc
*�������ܣ�      ת��printf���ڵ������
*���룺          ��
*����ֵ��        ��
**************************************************/
int fputc(int ch, FILE *f)
{
	USART_SendBit(USART1,ch);
	return ch;
}

/*************************************************
*��������        USART_Printf
*�������ܣ�      �Է�װ���ڴ�ӡ
*���룺          �̶�ʹ��USART1  
*����ֵ��        ��
**************************************************/
void USART_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	USART_SendString(USART1,String);
}
