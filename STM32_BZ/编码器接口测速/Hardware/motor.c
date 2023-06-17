#include "motor.h"


/**
*    @brief ���Ƶ����ת����ת��ֹͣ //PB12��PB13��PB14��PB15 Pwm:PA8��PA11
 *    @param None
 *    @retval None
 */
void Motor_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStruct;
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //ʱ��ʹ��
     
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //�������
     GPIO_InitStruct.GPIO_Pin =  AIN1_Pin|AIN2_Pin|BIN1_Pin|BIN2_Pin;    //GPIOB 9 ��������
     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�����ٶ�
     GPIO_Init(GPIOB,&GPIO_InitStruct);            //���ų�ʼ��
}     

void LeftMotor_Go() // ������ת AIN1 1 ��AIN 0  //
{
	GPIO_WriteBit(AIN1_GPIO_Port, AIN1_Pin, Bit_SET);
	GPIO_WriteBit(AIN2_GPIO_Port, AIN2_Pin, Bit_RESET);
}

void LeftMotor_Back()  //��ת��ת AIN1 0 ��AIN2 1
{
	GPIO_WriteBit(AIN1_GPIO_Port, AIN1_Pin, Bit_RESET);
	GPIO_WriteBit(AIN2_GPIO_Port, AIN2_Pin, Bit_SET);
}

void LeftMotor_Stop()  //����ͣת AIN1 0  AIN2 0
{
	GPIO_WriteBit(AIN1_GPIO_Port, AIN1_Pin, Bit_RESET);
	GPIO_WriteBit(AIN2_GPIO_Port, AIN2_Pin, Bit_RESET);
}

void RightMotor_Go()   //�ҵ����ת AIN1 1 ��AIN 0
{
	GPIO_WriteBit(BIN1_GPIO_Port, BIN1_Pin, Bit_SET);
	GPIO_WriteBit(BIN2_GPIO_Port, BIN2_Pin, Bit_RESET);
}
void RightMotor_Back()  //��ת��ת AIN1 0 ��AIN2 1
{
	GPIO_WriteBit(BIN1_GPIO_Port, BIN1_Pin, Bit_RESET);
	GPIO_WriteBit(BIN2_GPIO_Port, BIN2_Pin, Bit_SET);
}
void RightMotor_Stop()  //�ҵ��ͣת AIN1 0  AIN2 0
{
	GPIO_WriteBit(BIN1_GPIO_Port, BIN1_Pin, Bit_RESET);
	GPIO_WriteBit(BIN2_GPIO_Port, BIN2_Pin, Bit_RESET);
}

/**
 *    @brief ���Ƶ�������ٶȿ���
 *    @param �˶��������ҵ����PWMֵ
 *    @retval None
 */
void MotorControl(char motorDirection, int leftMotorPWM, int rightMotorPWM) {
	switch (motorDirection) {
	case 1:
		LeftMotor_Go();
		RightMotor_Go();
		TIM_SetCompare1(TIM1, leftMotorPWM);
		TIM_SetCompare1(TIM1, rightMotorPWM); 
		break;
	case 2:
		LeftMotor_Back();
		RightMotor_Back();
		TIM_SetCompare1(TIM1, leftMotorPWM);
		TIM_SetCompare1(TIM1, rightMotorPWM); 
		break;
	case 3:
		LeftMotor_Stop();
		RightMotor_Stop();
		TIM_SetCompare1(TIM1, 0);
		TIM_SetCompare1(TIM1, 0); 
		break;
	default:
		break;
	}
}
