#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "Delay.h"

#define DC1    GPIO_WriteBit(GPIOB, GPIO_Pin_11,Bit_SET);
#define DC0    GPIO_WriteBit(GPIOB, GPIO_Pin_11,Bit_RESET);
u8 SendBuff[480];
DMA_InitTypeDef DMA_InitStructure;
u16 DMA1_MEM_LEN;    
//配置dma
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
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

 


SPI_InitTypeDef  SPI_InitStructure;
//spi1的初始化
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;	
	SPI_Init(SPI1, &SPI_InitStructure); 
 
	SPI_Cmd(SPI1, ENABLE); 
	
	SPI1_ReadWriteByte(0xff);	 
}   
  
void SPI1_SetSpeed(u8 SpeedSet)
{
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
} 


u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); 
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); 				    
}


void initlcd(){

 	RCC->APB2ENR|=1<<3;//?????IO PORTC?? 							 
	GPIOB->CRH&=0XFFFF0FFF;//PC11/12 
	GPIOB->CRH|=0X00003000;
	GPIOB->ODR|=1<11;     //PC11,12 ???
  
     SPI1_Init();
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);	
	//配置dma
	MYDMA_Config(DMA1_Channel3,(u32)&SPI1->DR,(u32)SendBuff,480);
	
  writeCommand(0x01);
	Delay_us(150);
	writeCommand(0x11);
	Delay_us(120);
	writeCommand(0x3A);
	writeData(0x55);
	writeCommand(0x36);
	writeData(0x00);
	writeCommand(0x21);
	writeCommand(0x13);
	writeCommand(0x29);
	
}
void writeData(u8 data){
   DC1;
   SPI1_ReadWriteByte(data);
   
}

void writeCommand(u8 cmd){
	DC0;
   SPI1_ReadWriteByte(cmd);
}

void fillScreen(u16 color){
	u16 i ,j;
	//DC = 0;
  writeCommand(0x2A);
  writeData(0);
  writeData(0);
	writeData(0);
	writeData(240);
  writeCommand(0X2B);
  writeData(0);
  writeData(0);
	writeData(0X01);
	writeData(0X40);
  writeCommand(0X2C);
	DC1;
	for(j=0 ;j<480;){
      SendBuff[j] = color>>8;
		  SendBuff[j+1] = color;
		  j += 2;
	}

  for(i = 0 ; i<320 ; i++){
		
		  SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE); //????1?DMA?? 
	      MYDMA_Enable(DMA1_Channel3);
		
  while(1){
		if(DMA_GetFlagStatus(DMA1_FLAG_TC3)!=RESET)//μè′yí¨μà4′?ê?íê3é
				{
					DMA_ClearFlag(DMA1_FLAG_TC3);//??3yí¨μà4′?ê?íê3é±ê??
					break; 
		        }
					}			
	
	}

}

