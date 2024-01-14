//#include <stm32f10x.h>
//#include "Delay.h"

//void Key_Init(void)
//{
//    GPIO_InitTypeDef  GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_11;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//    GPIO_Init(GPIOB,&GPIO_InitStructure);
//}

//uint8_t Key_GetNum(void)
//{
//    uint8_t KeyNum =0;
//    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
//    {
//        Delay_ms(20);
//        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);
//        Delay_ms(20);
//        KeyNum = 1;
//    }
//	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
//    {
//        Delay_ms(20);
//        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0);
//        Delay_ms(20);
//        KeyNum = 2;
//    }

//    return KeyNum;

//}

#include "Key.h"
#include "Delay.h"
	 
 	    
//������ʼ������ 
//PA0.15��PC5 ���ó�����
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTCʱ
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;//PC8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC5
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;//PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0���ó��������� 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY0_PRES��KEY0����
//KEY1_PRES��KEY1����
//WKUP_PRES��WK_UP���� 
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
//u8 KEY_Scan(u8 mode)
//{	 
//	static u8 key_up=1;//�������ɿ���־
//	if(mode)key_up=1;  //֧������		  
//	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
//	{
//		Delay_ms(10);//ȥ���� 
//		key_up=0;
//		if(KEY0==0)return KEY0_PRES;
//		else if(KEY1==0)return KEY1_PRES;
//		else if(WK_UP==1)return WKUP_PRES; 
//	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	     
//	return 0;// �ް�������
//}

uint8_t Key_Num(void)
{
     if(KEY0 == 0 ||KEY1 == 0 || WK_UP == 0)
     {
          Delay_ms(100);
          if(KEY0 == 0) {return KEY0_PRES;}
          else if(KEY1 == 0) {return KEY1_PRES;}
          else if(WK_UP == 0) {return WKUP_PRES;}
     }
     return 0;
}
