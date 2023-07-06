#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

SPI_InitTypeDef SPI_InitStructure;
void MySPI_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
     
     
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;  //主机模式
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线全双工
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  //8bit一帧发送
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                //高位先发
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;  //128分频
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                       //模式2  cpol 2 cpha 0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                      //ss配置由软件模拟
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
	
	MySPI_W_SS(1);
     MySPI_SwapByte(0xff);
}

void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}
void SPI1_SetSpeed(u8 SpeedSet)
{
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
} 


uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);  //判断标志位，看发送是否为空
     SPI_I2S_SendData(SPI1,ByteSend);                              //发送数据
     while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET); //判断发送完
     return SPI_I2S_ReceiveData(SPI1);                            //接收数据
} 

u16 DMA1_MEM_LEN;
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
     DMA_InitTypeDef DMA_InitStructure;
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ê1?üDMA′?ê?
	
     DMA_DeInit(DMA_CHx);   //将dma1的某通道
	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //dma 要搬运到的外设地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //dma要搬运的内存的地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //搬运方向， 从内存到外设
	DMA_InitStructure.DMA_BufferSize = cndtr; //要搬运的内存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //  传输过程中外设的基地址不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //传输过程中内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为八位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//数据宽度为八位 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //正常传输模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //优先级设置
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //没有内存到内存的传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //
	  	
} 
//使能dma1的通道3，因为spi输出对应的是此通道
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );    
 	DMA_SetCurrDataCounter(DMA1_Channel3,DMA1_MEM_LEN);
 	DMA_Cmd(DMA_CHx, ENABLE);  
}	  

 

//初始化显示屏
void initlcd(){
 	RCC->APB2ENR|=1<<3;//时钟使能							 
	GPIOB->CRH&=0XFFFF0FFF;//设置为输出模式
	GPIOB->CRH|=0X00003000;
	GPIOB->ODR|=1<11;     //dc high

     SPI1_Init();
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);	
	
	
    writeCommand(0x01);
	delay_us(150);
	writeCommand(0x11);
	delay_us(120);
	writeCommand(0x3A);
	writeData(0x55);
	writeCommand(0x36);
	writeData(0x00);
	writeCommand(0x21);
	writeCommand(0x13);
	writeCommand(0x29);
	
}
void writeData(u8 data){
   DC = 1;
   SPI1_ReadWriteByte(data);
   
}
