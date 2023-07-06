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
	
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;  //����ģʽ
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫��ȫ˫��
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  //8bitһ֡����
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                //��λ�ȷ�
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;  //128��Ƶ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                       //ģʽ2  cpol 2 cpha 0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                      //ss���������ģ��
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
	while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);  //�жϱ�־λ���������Ƿ�Ϊ��
     SPI_I2S_SendData(SPI1,ByteSend);                              //��������
     while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET); //�жϷ�����
     return SPI_I2S_ReceiveData(SPI1);                            //��������
} 

u16 DMA1_MEM_LEN;
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
     DMA_InitTypeDef DMA_InitStructure;
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//��1?��DMA��?��?
	
     DMA_DeInit(DMA_CHx);   //��dma1��ĳͨ��
	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //dma Ҫ���˵��������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //dmaҪ���˵��ڴ�ĵ�ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���˷��� ���ڴ浽����
	DMA_InitStructure.DMA_BufferSize = cndtr; //Ҫ���˵��ڴ�Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //  �������������Ļ���ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //����������ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ��λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//���ݿ��Ϊ��λ 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //���ȼ�����
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //û���ڴ浽�ڴ�Ĵ���
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //
	  	
} 
//ʹ��dma1��ͨ��3����Ϊspi�����Ӧ���Ǵ�ͨ��
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );    
 	DMA_SetCurrDataCounter(DMA1_Channel3,DMA1_MEM_LEN);
 	DMA_Cmd(DMA_CHx, ENABLE);  
}	  

 

//��ʼ����ʾ��
void initlcd(){
 	RCC->APB2ENR|=1<<3;//ʱ��ʹ��							 
	GPIOB->CRH&=0XFFFF0FFF;//����Ϊ���ģʽ
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
