#include "usart.h"

char Serial_RxPacket[100];				//"@MSG\r\n"
uint8_t Serial_RxFlag;

/*************************************************
*函数名：     USART1_Init
*函数功能：   串口1的初始化
*输入：       bound：波特率   
*返回值：     无
**************************************************/

void USART1_Init(uint32_t bound)
{
     GPIO_InitTypeDef GPIO_InitStruct;
     USART_InitTypeDef USART_InitStruct;
     NVIC_InitTypeDef NVIC_InitStruct;
     
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //时钟使能
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
     
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
     GPIO_InitStruct.GPIO_Pin = USART1_TX_Pin;    //GPIOB 9 发送引脚
     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //发送速度
     GPIO_Init(GPIOA,&GPIO_InitStruct);            //引脚初始化
     
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //浮空输入
     GPIO_InitStruct.GPIO_Pin = USART1_RX_Pin;          //GPIOB 10接收引脚
     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOA,&GPIO_InitStruct); //引脚初始化
     
     
     USART_InitStruct.USART_BaudRate = bound; //波特率
     USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
     USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx; //收发模式
     USART_InitStruct.USART_Parity = USART_Parity_No; //没有校验位
     USART_InitStruct.USART_StopBits = USART_StopBits_1; //一位停止位
     USART_InitStruct.USART_WordLength = USART_WordLength_8b; //8位一个字节
     USART_Init(USART1,&USART_InitStruct); //初始化串口
     
     USART_Cmd(USART1,ENABLE); //串口使能
     
     USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //接收中断使能   (有查询和中断两种方法实现接收)
     NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;  //要打开的中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2; //抢占式优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;   //相应式优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //NVIC通道使能
	NVIC_Init(&NVIC_InitStruct);
	
	USART_ClearFlag(USART1, USART_FLAG_TC); //TC位初始值位1，要先清0


     
}

/*************************************************
*函数名：         USART1_IRQHandler
*函数功能：       串口中断函数-将从电脑发送的数据发回给电脑
*输入：           无
*返回值：         无
**************************************************/
void USART1_IRQHandler(void)
{
     int on,off;
     static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)  //判断中断标志
	{
		
          uint8_t RxData = USART_ReceiveData(USART1);
		
     }
}

/*************************************************
*函数名：       USART_ReceiveBit
*函数功能：     串口接收函数
*输入：         USARTx:串口
*返回值：       接收到的数据
**************************************************/
uint16_t USART_ReceiveBit(USART_TypeDef* USARTx)
{
	while(!USART_GetFlagStatus(USARTx, USART_FLAG_RXNE)); //等待接收的数据全部接收
	USART_ClearFlag(USARTx, USART_FLAG_RXNE);
	return USART_ReceiveData(USARTx);
}


//直接使用串口发送接收函数会出现内容覆盖的问题，所以需要我们重写函数
/*************************************************
*函数名：      USART_SendBit
*函数功能：    串口发送函数
*输入：        USARTx:串口  Data：发送的数据
*返回值：      无
**************************************************/
void USART_SendBit(USART_TypeDef* USARTx,uint16_t Data)   
{
	USART_SendData(USARTx, Data);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
//	while(!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));  //要等待数据全部转到移位寄存器
//	USART_ClearFlag(USARTx, USART_FLAG_TXE); //清空标志位
	
//    while(!USART_GetFlagStatus(USARTx, USART_FLAG_TC));//要等待数据全部发出
//    USART_ClearFlag(USARTx, USART_FLAG_TC); //清空标志位
}




//void USART_SenndPaket(void)
//{
//     USART_SendBit(USART1,0xFF);
//     USART_SendArray(USART1,USART_TxPacket,4);
//     USART_SendBit(USART1,0xFE);
//     
//}



/*************************************************
*函数名：      USART_SendArray
*函数功能：    串口发送函数
*输入：        USARTx:串口  Array:数组名  Length:长度
*返回值：      无
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
*函数名：        USART_SendString
*函数功能：      串口发送字符串函数
*输入：          USARTx:串口，string：字符型指针
*返回值：        无
**************************************************/
void USART_SendString(USART_TypeDef* USARTx,char *string)
{
	while(*string)
	{
		USART_SendBit(USARTx,*string++);
	}
}

/*************************************************
*函数名：        USART_SendString
*函数功能：      串口发送字符串函数
*输入：          USARTx:串口，Number:数字  Length：长度
*返回值：        无
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
*函数名：        fputc
*函数功能：      转换printf让在电脑输出
*输入：          无
*返回值：        无
**************************************************/
int fputc(int ch, FILE *f)
{
	USART_SendBit(USART1,ch);
	return ch;
}

/*************************************************
*函数名：        USART_Printf
*函数功能：      自封装串口打印
*输入：          固定使用USART1  
*返回值：        无
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
