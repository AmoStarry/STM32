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
#include "Key.h"


/*����λ��PID���ٶ�PID�ṹ���͵�ȫ�ֱ���*/
PID pid_location;
PID pid_speed;


//#define ENCODER_RESOLUTION 11    /*������һȦ������������*/
//#define ENCODER_MULTIPLE 4       /*��������Ƶ��ͨ����ʱ���ı�����ģʽ����*/
//#define MOTOR_REDUCTION_RATIO 46 /*����ļ��ٱ�*/
///*���תһȦ�ܵ�������(��ʱ���ܶ�����������) = ����������������*��������Ƶ*������ٱ� */
///* 11*4*34= 1496*/
//#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 
#define ENCODER_TIM_PERIOD  65535   /*���������ֵ*/
int16_t EncoderOverflowCnt = 0;

int16_t  machou, Speed;  //ʵ���ٶ�
int pwm = 0;
int PWM1 = 0;

static uint32_t location_timer = 0;    // λ�û�����
	
static int32_t encoderNow = 0;    /*��ǰʱ���ܼ���ֵ*/
static int32_t encoderNum = 0;   /*��һʱ���ܼ���ֵ*/
int Distance  = 0; 
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
     protocol_RX_init();
     USART1_Init(115200);
     PID_Init();
     Key_Init();
     protocol_TX_init();
     
	
	
	OLED_ShowString(1, 1, "Speed:");
     //	printf("666\n\r");
	while (1)
	{
         receiving_process();
          

      //          printf("PWM = %d\n\r",PWM1);
     //          printf("bianma = %d\n\r",encoderNow);
		//OLED_ShowSignedNum(1, 7, Speed, 5);
	}
}



void TIM2_IRQHandler(void)
{

     int temp=0;
     static int i=0;
     
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
          encoderNow = Encoder_Get();              //10ms��CNT������10us����һ��
          encoderNum += encoderNow;                /*���м���������ۼӵ��ۼ�ֵ*/
          actual_speed = (float)encoderNow*6000 / TOTAL_RESOLUTION ; //ʵ���ٶ� תÿ��
          printf("actual_speed = %.f\n\r",actual_speed);
          Distance = encoderNum/(TOTAL_RESOLUTION*zhouchang);       //ʵ��λ�õ�λ����
	
          /*��2��λ��PID���㣬�õ�PWM����ֵ*/
          if ((location_timer++ % 2) == 0)
          {
               float control_val = 0;   /*��ǰ����ֵ*/
               
               /*λ��PID����*/
               control_val = location_pid_realize(&pid_location, encoderNum);   //�õ��ٶȻ����ٶ�Ŀ��ֵ
               
               /*Ŀ���ٶ�ֵ����*/
     		speed_val_protect(&control_val);   //�����Ժ�������Ƶ�����ٶȿ��ܣ���ı��ٶȿ���������
               //printf("Speed = %f\n\r",control_val);
     		/*�趨�ٶ�PID��Ŀ��ֵ*/
     		set_pid_target(&pid_speed, control_val);    

#if defined(PID_ASSISTANT_EN)
     		if ((location_timer % 16) == 8)
     		{
     			temp = (int)control_val;
     			set_computer_value(SEND_TARGET_CMD, CURVES_CH2, &temp, 1);     // ��ͨ�� 2 ����Ŀ��ֵ
     		}
#endif
     	   /*��3���ٶ�PID���㣬�õ�PWM����ֵ*/
     	    actual_speed_int = actual_speed;
     	    PWM1 = pwm_val_protect((int)speed_pid_realize(&pid_speed, actual_speed)); //�ٶȻ�����������ʵ���ٶȣ��õ�PWM������ 
              PWM1 = pwm_val_protect((int)control_val);
          /*��4��PWM���Ƶ��*/
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
          
           /*��5�������ϴ�����λ����ʾ*/

               i++; 
               if(i%12 == 5)
               {
                    
                    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &Distance, 1);   /*��ͨ��1����ʵ�ʵĵ����λ�á�ֵ*/
               }
          //	else if(i%12 == 10)
          //	{
          //		set_computer_value(SEND_FACT_CMD, CURVES_CH2, &actual_speed_int, 1); /*��ͨ��2����ʵ�ʵĵ�����ٶȡ�ֵ*/
          //	}     
               TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
          }
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


