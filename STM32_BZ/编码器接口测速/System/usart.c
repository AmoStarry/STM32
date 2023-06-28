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
void usart1_send(u8*data, u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
		USART_SendData(USART1,data[i]);   
	}
}

//=======================================
//串口1中断服务程序
//=======================================
uint8_t Recv1[128]={0};//串口接收缓存
u8 rx_cnt=0;//接收数据个数计数变量

int sizecopy=128;

void USART1_IRQHandler(void)                	
{
	uint8_t data;//接收数据暂存变量
	uint8_t bufcopy[128];//最多只取前64个数据

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		data = USART_ReceiveData(USART1);   			
		Recv1[rx_cnt++]=data;//接收的数据存入接收数组 
		//protocol_data_recv(&data, 1);
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	} 
	
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//空闲中断
	{
		data = USART1->SR;//串口空闲中断的中断标志只能通过先读SR寄存器，再读DR寄存器清除！
		data = USART1->DR;
		
		//清空本地接收数组
		memset(bufcopy,0,sizecopy);  /* void *memset(void *s,int c,unsigned long n);
                                         指针变量s所指的前n个字节的内存单元填充给定的int型数值c，它可以为任何数据进行初始化
                                         是将数值c以单个字节逐个拷贝的方式放到指针变量s所指的内存中去*/
		
		memcpy(bufcopy,Recv1,rx_cnt);// 有几个复制几个, 以source指向的地址为起点，将连续的n个字节数据，复制到以destin指向的地址为起点的内存中。
                                       //函数有三个参数，第一个是目标地址，第二个是源地址，第三个是数据长度
		
		protocol_data_recv(bufcopy, rx_cnt);

		memset(Recv1,0,sizecopy);
		rx_cnt=0;
	}
} 

#endif	

