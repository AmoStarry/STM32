/*
 * motor.h
 *
 *  Created on: Apr 2023/5/11
 *      Author: cope csdn ZRob
 */

#ifndef MOTOR_MOTOR_H_
#define MOTOR_MOTOR_H_
#include "stm32f10x.h"                  // Device header

#define AIN1_Pin        GPIO_Pin_12
#define AIN1_GPIO_Port  GPIOB
#define AIN2_Pin        GPIO_Pin_13
#define AIN2_GPIO_Port  GPIOB
#define BIN1_Pin        GPIO_Pin_14
#define BIN1_GPIO_Port  GPIOB
#define BIN2_Pin        GPIO_Pin_15
#define BIN2_GPIO_Port  GPIOB
#define PWMA_Pin        GPIO_Pin_8
#define PWMA_GPIO_Port  GPIOA
#define PWMB_Pin        GPIO_Pin_11
#define PWMB_GPIO_Port  GPIOA


void Motor_Init(void);
void LeftMotor_Go(void);
void LeftMotor_Back(void);
void LeftMotor_Stop(void);
void RightMotor_Go(void);
void RightMotor_Go(void);
void RightMotor_Back(void);
void RightMotor_Stop(void) ;

void MotorControl(char motorDirection,int leftMotorPWM, int rightMotorPWM);

#endif /* MOTOR_MOTOR_H_ */

