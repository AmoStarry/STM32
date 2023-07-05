#ifndef __USAR_PID_H
#define __USAR_PID_H
#include "PID.h"
#include "stm32f10x.h"                  // Device header

#define t 3.14
#define r 3.3
#define zhouchang  (2*t*r)
void set_p_i_d(PID *pid, float p, float i, float d);
void set_pid_target(PID *pid, float temp_val);
float get_pid_target(PID *pid);
void speed_val_protect(float *speed_val);
void SetTargetMaxSpeed(int speed);
int GetTargetMaxSpeed(void);
#endif
