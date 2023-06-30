#include "PID.h"

#define KP 1.0
#define KI 1.0
#define KD 1.0

float target_speed = 1000.0;
float current_speed = 0.0;
float last_error = 0.0;
float integral_value = 0.0;

float PID_control(float  current_speed)
{
    float error = target_speed - current_speed;
    float proportional = KP * error;
    integral_value += KI * error;
    float derivative = KD * (error - last_error);
    last_error = error;

    float control_output = proportional + integral_value + derivative;
    return control_output;
}

