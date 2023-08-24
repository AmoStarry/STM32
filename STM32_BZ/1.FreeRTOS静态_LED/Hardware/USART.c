#include "usart.h"
 u16 r;
uint8_t USART_TxPacket[4];
uint8_t USART_RxPacket[4];

/*************************************************
*函数名：     USART1_Init
*函数功能：   串口1的初始化
*输入：       bound：波特率   
*返回值：     无
**************************************************/

//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOS使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                  
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fput·c函数 
int fputc(int ch, FILE *f)
{      
    while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void uart_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
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
//     RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //时钟使能
//     RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//     
//     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
//     GPIO_InitStruct.GPIO_Pin = USART1_TX_Pin;    //GPIOB 9 发送引脚
//     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //发送速度
//     GPIO_Init(GPIOA,&GPIO_InitStruct);            //引脚初始化
//     
//     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //浮空输入
//     GPIO_InitStruct.GPIO_Pin = USART1_RX_Pin;          //GPIOB 10接收引脚
//     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(GPIOA,&GPIO_InitStruct); //引脚初始化
//     
//     
//     USART_InitStruct.USART_BaudRate = bound; //波特率
//     USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
//     USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx; //收发模式
//     USART_InitStruct.USART_Parity = USART_Parity_No; //没有校验位
//     USART_InitStruct.USART_StopBits = USART_StopBits_1; //一位停止位
//     USART_InitStruct.USART_WordLength = USART_WordLength_8b; //8位一个字节
//     USART_Init(USART1,&USART_InitStruct); //初始化串口
//     
//     USART_Cmd(USART1,ENABLE); //串口使能
//     
//     USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //接收中断使能   (有查询和中断两种方法实现接收)
//     NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;  //要打开的中断通道
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2; //抢占式优先级
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;   //相应式优先级
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //NVIC通道使能
//	NVIC_Init(&NVIC_InitStruct);
//	
//	USART_ClearFlag(USART1, USART_FLAG_TC); //TC位初始值位1，要先清0


//     
//}
////直接使用串口发送接收函数会出现内容覆盖的问题，所以需要我们重写函数
///*************************************************
//*函数名：      USART_SendBit
//*函数功能：    串口发送函数
//*输入：        USARTx:串口  Data：发送的数据
//*返回值：      无
//**************************************************/
//void USART_SendBit(USART_TypeDef* USARTx,uint16_t Data)   
//{
//	USART_SendData(USARTx, Data);
//	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
////	while(!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));  //要等待数据全部转到移位寄存器
////	USART_ClearFlag(USARTx, USART_FLAG_TXE); //清空标志位
//	
////    while(!USART_GetFlagStatus(USARTx, USART_FLAG_TC));//要等待数据全部发出
////    USART_ClearFlag(USARTx, USART_FLAG_TC); //清空标志位
//}


///*************************************************
//*函数名：         USART1_IRQHandler
//*函数功能：       串口中断函数-将从电脑发送的数据发回给电脑
//*输入：           无
//*返回值：         无
//**************************************************/
//void USART1_IRQHandler(void)
//{
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)  //判断中断标志
//	{
//		r = USART_ReceiveData(USART1);
//		USART_SendBit(USART1,r);
//	}
//}

///*************************************************
//*函数名：       USART_ReceiveBit
//*函数功能：     串口接收函数
//*输入：         USARTx:串口
//*返回值：       接收到的数据
//**************************************************/
//uint16_t USART_ReceiveBit(USART_TypeDef* USARTx)
//{
//	while(!USART_GetFlagStatus(USARTx, USART_FLAG_RXNE)); //等待接收的数据全部接收
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
//*函数名：      USART_SendArray
//*函数功能：    串口发送函数
//*输入：        USARTx:串口  Array:数组名  Length:长度
//*返回值：      无
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
//*函数名：        USART_SendString
//*函数功能：      串口发送字符串函数
//*输入：          USARTx:串口，string：字符型指针
//*返回值：        无
//**************************************************/
//void USART_SendString(USART_TypeDef* USARTx,char *string)
//{
//	while(*string)
//	{
//		USART_SendBit(USARTx,*string++);
//	}
//}

///*************************************************
//*函数名：        USART_SendString
//*函数功能：      串口发送字符串函数
//*输入：          USARTx:串口，Number:数字  Length：长度
//*返回值：        无
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
//*函数名：        fputc
//*函数功能：      转换printf让在电脑输出
//*输入：          无
//*返回值：        无
//**************************************************/
//int fputc(int ch, FILE *f)
//{
//	USART_SendBit(USART1,ch);
//	return ch;
//}

///*************************************************
//*函数名：        USART_Printf
//*函数功能：      自封装串口打印
//*输入：          固定使用USART1  
//*返回值：        无
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
