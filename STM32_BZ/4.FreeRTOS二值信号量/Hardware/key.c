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
	 
 	    
//按键初始化函数 
//PA0.15和PC5 设置成输入
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTC时
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;//PC8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC5
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;//PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0设置成上拉输入 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY0_PRES，KEY0按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下 
//注意此函数有响应优先级,KEY0>KEY1>WK_UP!!
//u8 KEY_Scan(u8 mode)
//{	 
//	static u8 key_up=1;//按键按松开标志
//	if(mode)key_up=1;  //支持连按		  
//	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
//	{
//		Delay_ms(10);//去抖动 
//		key_up=0;
//		if(KEY0==0)return KEY0_PRES;
//		else if(KEY1==0)return KEY1_PRES;
//		else if(WK_UP==1)return WKUP_PRES; 
//	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	     
//	return 0;// 无按键按下
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
