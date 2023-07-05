#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"
#include "usart.h"
#include "motor.h"
#include "PWM.h"
#include "PID.h"
#include "protocol.h"
#include "USAR_PID.h"

#define ENCODER_RESOLUTION 11    /*������һȦ������������*/
#define ENCODER_MULTIPLE 4       /*��������Ƶ��ͨ����ʱ���ı�����ģʽ����*/
#define MOTOR_REDUCTION_RATIO 46 /*����ļ��ٱ�*/
/*���תһȦ�ܵ�������(��ʱ���ܶ�����������) = ����������������*��������Ƶ*������ٱ� */
/* 11*4*34= 1496*/
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 
#define ENCODER_TIM_PERIOD  65535   /*���������ֵ*/
int16_t EncoderOverflowCnt = 0;

int16_t  machou, Speed;  //ʵ���ٶ�
int pwm = 0;
int PWM1 = 0;

//static uint32_t location_timer = 0;    // λ�û�����
	
static int32_t encoderNow = 0;    /*��ǰʱ���ܼ���ֵ*/
static int32_t encoderNum = 0;   /*��һʱ���ܼ���ֵ*/
int encoderDelta = 0; /*��ǰʱ������һʱ�̱������ı仯��*/
float actual_speed = 0;  /*ʵ�ʲ���ٶ�*/
int actual_speed_int = 0;

int pwm_val_protect(int pwm_input);

int32_t target_speed = 0;
int main(void)
{
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	OLED_Init();
     Motor_Init();
     PWM_Init();
	Timer_Init();
	Encoder_Init();
     protocol_init();
     USART1_Init(115200);
     PID_Init();
     
     #if defined(PID_ASSISTANT_EN)
	/*��ʼ��ʱ���Ϸ�stop��ͬ����λ����������ť״̬*/
	set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);  

	/*��ȡĬ�ϵ�Ŀ��ֵ*/
	target_speed = (int32_t)get_pid_target();
	/*��ͨ��1����Ŀ��ֵ*/
	set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &target_speed, 1);     
#endif
	
	
	OLED_ShowString(1, 1, "Speed:");
//	printf("666\n\r");
	while (1)
	{

         receiving_process();
          

          
          
//          printf("PID =%d\n\r",pwm);
//          printf("PWM = %d\n\r",PWM1);
//          printf("bianma = %d\n\r",encoderNow);
		//OLED_ShowSignedNum(1, 7, Speed, 5);
	}
}



void TIM2_IRQHandler(void)
{

     
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
          encoderNow = Encoder_Get();             //10us��CNT������10us����һ��
          encoderNum += encoderNow;                /*���м���������ۼӵ��ۼ�ֵ*/
          actual_speed = (float)encoderNow*6000 / TOTAL_RESOLUTION ;
          
          Speed =(int)actual_speed;
          printf("speed =%d\n\r",encoderNow);
          
          pwm = PID_realize(actual_speed);
        //  printf("PID =%d\n\r",pwm);
          PWM1 = pwm_val_protect((int)pwm);
        //  printf("PWM = %d\n\r",PWM1);
          if(PWM1>0)
          {
            PWM1 =PWM1;
            MotorControl(1,PWM1 ,PWM1);
          }
          else if(PWM1<0)
          {
               PWM1 = -PWM1;
               MotorControl(2,PWM1 ,PWM1);
          }
          else if(PWM1 == 0)
          {
               MotorControl(3,0,0);
          }
          
static int i=0;          
#if (PID_ASSISTANT_EN)
	i++;
	if(i==12)
	{
		i=0;
		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &Speed, 1); /*��ͨ��1����ʵ�ʵĵ�����ٶȡ�ֵ*/
		//set_computer_value(SEND_FACT_CMD, CURVES_CH1, &encoderNow, 1); /*��ͨ��1����ʵ�ʵĵ����λ�á�ֵ*/
	}
#else
	i++;
	if(i==100)
	{
		i=0;
		printf("sum:%d set_pwm:%d\r\n",sum,res_pwm);
		
	}
#endif
          
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
int pwm_val_protect(int pwm_input)
{
	int pwm_output = 0;
	
	//ռ�ձ����ֵ�ж�
	if(pwm_input>999) 
	{
		pwm_output = 999;
	}
	else if(pwm_input<-999) 
	{
		pwm_output = -999;
	}
	//ռ�ձ�̫С�����յ����̫С�����
	else if((pwm_input>-100)&&(pwm_input<100)) 
	{
		pwm_output = 0;
	}
	else
	{
		pwm_output = pwm_input;
	}
	
	return pwm_output;
}


