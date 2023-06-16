#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure; //定义GPIO结构体
     TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//定义定时器结构体
     TIM_OCInitTypeDef TIM_OCInitStructure;//定义计数器输入输出比较结构体
     
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //定时器1使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//PWM_GPIO输出使能，直接使能对应引脚不要复用

	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出（只有用作GPIO才不用复用，片上外设过来都要用复用功能）       
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;	   //GPIOA8 ch1 GPIOA11 ch4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //GPIO高速时钟频率
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM1);                            //对不需要配置的项给默认值
	
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;      //选择外部高速系统时钟作为分频来源
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		          //ARR 计数器重装值  PWM频率 = 时钟频率/时钟分频+1/重装值+1  =72/(PSC+1)/(ARR+1)
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;		     //PSC 时钟分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);             
	
	
	TIM_OCStructInit(&TIM_OCInitStructure);                         //结构体没配置完整，对结构体赋默认值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //输出比较的模式：PWM1模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //设置输出比较的极性：高极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
     TIM_OCInitStructure.TIM_Pulse = 0;		//CCR                     占空比初值 占空比=CCR/（ARR+1），后面通过函数修改值默认为0
     
     TIM_OC1Init(TIM1, &TIM_OCInitStructure);                        //用结构体配置PWMCH1通道
     TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);               //启动自动重装
     TIM_OC4Init(TIM1, &TIM_OCInitStructure);                        //用结构体配置PWMCH4通道
     TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
     
	TIM_Cmd(TIM1, ENABLE);           //定时器总开关使能
     TIM_CtrlPWMOutputs(TIM1, ENABLE);//高级定时器必加    
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM1, Compare);  //调用这个函数就能随时修改通道1占空比初值 CRR
}

void PWM_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM1, Compare);  //调用这个函数就能随时修改通道2占空比初值 CRR
}
