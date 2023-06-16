#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure; //����GPIO�ṹ��
     TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//���嶨ʱ���ṹ��
     TIM_OCInitTypeDef TIM_OCInitStructure;//�����������������ȽϽṹ��
     
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //��ʱ��1ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//PWM_GPIO���ʹ�ܣ�ֱ��ʹ�ܶ�Ӧ���Ų�Ҫ����

	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�������������ֻ������GPIO�Ų��ø��ã�Ƭ�����������Ҫ�ø��ù��ܣ�       
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;	   //GPIOA8 ch1 GPIOA11 ch4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //GPIO����ʱ��Ƶ��
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM1);                            //�Բ���Ҫ���õ����Ĭ��ֵ
	
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;      //ѡ���ⲿ����ϵͳʱ����Ϊ��Ƶ��Դ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		          //ARR ��������װֵ  PWMƵ�� = ʱ��Ƶ��/ʱ�ӷ�Ƶ+1/��װֵ+1  =72/(PSC+1)/(ARR+1)
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;		     //PSC ʱ�ӷ�Ƶ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);             
	
	
	TIM_OCStructInit(&TIM_OCInitStructure);                         //�ṹ��û�����������Խṹ�帳Ĭ��ֵ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //����Ƚϵ�ģʽ��PWM1ģʽ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //��������Ƚϵļ��ԣ��߼���
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //���ʹ��
     TIM_OCInitStructure.TIM_Pulse = 0;		//CCR                     ռ�ձȳ�ֵ ռ�ձ�=CCR/��ARR+1��������ͨ�������޸�ֵĬ��Ϊ0
     
     TIM_OC1Init(TIM1, &TIM_OCInitStructure);                        //�ýṹ������PWMCH1ͨ��
     TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);               //�����Զ���װ
     TIM_OC4Init(TIM1, &TIM_OCInitStructure);                        //�ýṹ������PWMCH4ͨ��
     TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
     
	TIM_Cmd(TIM1, ENABLE);           //��ʱ���ܿ���ʹ��
     TIM_CtrlPWMOutputs(TIM1, ENABLE);//�߼���ʱ���ؼ�    
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM1, Compare);  //�����������������ʱ�޸�ͨ��1ռ�ձȳ�ֵ CRR
}

void PWM_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM1, Compare);  //�����������������ʱ�޸�ͨ��2ռ�ձȳ�ֵ CRR
}
