#include "stm32f10x.h"                  // Device header

#define ENCODER_TIM_PSC  0          /*��������Ƶ*/
#define ENCODER_TIM_PERIOD  65535   /*���������ֵ*/
#define CNT_INIT 0                  /*��������ֵ*/

#define ENCODER_RESOLUTION 11    /*������һȦ������������*/
#define ENCODER_MULTIPLE 4       /*��������Ƶ��ͨ����ʱ���ı�����ģʽ����*/
#define MOTOR_REDUCTION_RATIO 34 /*����ļ��ٱ�*/
/*���תһȦ�ܵ�������(��ʱ���ܶ�����������) = ����������������*��������Ƶ*������ٱ� */
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 

/**
* @brief TIM3 ͨ��1ͨ��2 ����������
* @param none
*/

void Encoder_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
     TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
     TIM_ICInitTypeDef TIM_ICInitStructure;
     
     
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //��ʼ����ʱ��3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//��ʼ��GPIOA
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        //��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //ʱ����Դ��ϵͳʱ��
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = ENCODER_TIM_PERIOD;		//ARR ��װ��
	TIM_TimeBaseInitStructure.TIM_Prescaler = ENCODER_TIM_PSC;		//PSC ����Ƶ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	
	TIM_ICStructInit(&TIM_ICInitStructure);               //�ṹ�����ò���������ֵ
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;     //ͨ��1
	TIM_ICInitStructure.TIM_ICFilter = 0;              //����ͨ�����˲�����
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
     TIM_SetCounter(TIM3, CNT_INIT);      /*CNT���ֵ*/
	TIM_ClearFlag(TIM3,TIM_IT_Update);   /*�жϱ�־��0*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); /*�ж�ʹ��*/
	TIM_Cmd(TIM3, ENABLE);
}

int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3); //���ؼ���ֵ
	TIM_SetCounter(TIM3, 0);
	return Temp;
}

//������ת�٣�����һ����ʱ��100ms����1�Σ�
void motor_speed(void)
{
	int encoderNum = 0;
	float rotateSpeed = 0;
	
	/*��ȡ��������ֵ������������ת����*/
	encoderNum = Encoder_Get();
	/* ת��(1����ת����Ȧ)=��λʱ���ڵļ���ֵ/�ֱܷ���*ʱ��ϵ�� */
	rotateSpeed = (float)encoderNum/TOTAL_RESOLUTION*10;
     printf("encoder: %d\t speed:%.2f rps\r\n",encoderNum,rotateSpeed);
}

