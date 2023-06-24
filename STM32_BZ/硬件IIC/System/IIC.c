#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void IIC_Init()
{
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);  //iic2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;   //����ģʽ
	I2C_InitStructure.I2C_ClockSpeed = 50000;   //50KHz ,��Χ��100KHz --400KHz
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; //�ߵ͵�ƽռ�ձȵ���2:1������16:9
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;        //Ĭ�ϸ�Ӧ��
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  //7λ��ַģʽ
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;  //��Ϊ�ӻ�ʱ��ַ
	I2C_Init(I2C2, &I2C_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);  //ʹ��I2C2
   
}