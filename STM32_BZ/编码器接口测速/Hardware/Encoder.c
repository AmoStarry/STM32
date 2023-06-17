#include "stm32f10x.h"                  // Device header

#define ENCODER_TIM_PSC  0          /*计数器分频*/
#define ENCODER_TIM_PERIOD  65535   /*计数器最大值*/
#define CNT_INIT 0                  /*计数器初值*/

#define ENCODER_RESOLUTION 11    /*编码器一圈的物理脉冲数*/
#define ENCODER_MULTIPLE 4       /*编码器倍频，通过定时器的编码器模式设置*/
#define MOTOR_REDUCTION_RATIO 34 /*电机的减速比*/
/*电机转一圈总的脉冲数(定时器能读到的脉冲数) = 编码器物理脉冲数*编码器倍频*电机减速比 */
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 

/**
* @brief TIM3 通道1通道2 正交编码器
* @param none
*/

void Encoder_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
     TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
     TIM_ICInitTypeDef TIM_ICInitStructure;
     
     
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //初始化定时器3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//初始化GPIOA
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        //上拉输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //时钟来源于系统时钟
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInitStructure.TIM_Period = ENCODER_TIM_PERIOD;		//ARR 重装载
	TIM_TimeBaseInitStructure.TIM_Prescaler = ENCODER_TIM_PSC;		//PSC 不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	
	TIM_ICStructInit(&TIM_ICInitStructure);               //结构体配置不完整赋初值
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;     //通道1
	TIM_ICInitStructure.TIM_ICFilter = 0;              //输入通道的滤波参数
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
     TIM_SetCounter(TIM3, CNT_INIT);      /*CNT设初值*/
	TIM_ClearFlag(TIM3,TIM_IT_Update);   /*中断标志清0*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); /*中断使能*/
	TIM_Cmd(TIM3, ENABLE);
}

int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3); //返回计数值
	TIM_SetCounter(TIM3, 0);
	return Temp;
}

//计算电机转速（被另一个定时器100ms调用1次）
void motor_speed(void)
{
	int encoderNum = 0;
	float rotateSpeed = 0;
	
	/*读取编码器的值，正负代表旋转方向*/
	encoderNum = Encoder_Get();
	/* 转速(1秒钟转多少圈)=单位时间内的计数值/总分辨率*时间系数 */
	rotateSpeed = (float)encoderNum/TOTAL_RESOLUTION*10;
     printf("encoder: %d\t speed:%.2f rps\r\n",encoderNum,rotateSpeed);
}

