/*
 * motor.h
 *
 *  Created on: Apr 2023/5/11
 *      Author: cope csdn ZRob
 */

#ifndef MOTOR_MOTOR_H_
#define MOTOR_MOTOR_H_

#include "main.h"
#include "tim.h"


void MotorControl(char motorDirection,int leftMotorPWM, int rightMotorPWM);

#endif /* MOTOR_MOTOR_H_ */

