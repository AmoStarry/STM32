/*
 * motor.c
 *
 *  Created on: Apr 2023/5/11
 *      Author: cope csdn ZRob
 */
#include "motor.h"
/**
*    @brief ���Ƶ����ת����ת��ֹͣ //PB12��PB13��PB14��PB15 Pwm:PA8��PA11
 *    @param None
 *    @retval None
 */
void LeftMotor_Go() // ������ת AIN1 1 ��AIN 0  //
{
	HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
}

void LeftMotor_Back()  //��ת��ת AIN1 0 ��AIN2 1
{
	HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
}

void LeftMotor_Stop()  //����ͣת AIN1 0  AIN2 0
{
	HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
}

void RightMotor_Go()   //�ҵ����ת AIN1 1 ��AIN 0
{
	HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
}
void RightMotor_Back()  //��ת��ת AIN1 0 ��AIN2 1
{
	HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
}
void RightMotor_Stop()  //�ҵ��ͣת AIN1 0  AIN2 0
{
	HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
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
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, leftMotorPWM);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, rightMotorPWM);
		break;
	case 2:
		LeftMotor_Back();
		RightMotor_Back();
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,leftMotorPWM);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, rightMotorPWM);
		break;
	case 3:
		LeftMotor_Stop();
		RightMotor_Stop();
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		break;
	default:
		break;
	}
}
