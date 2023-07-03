#ifndef __USAR_PID_H
#define __USAR_PID_H
#include "PID.h"
#include "stm32f10x.h"                  // Device header


void set_p_i_d(float p, float i, float d);
void set_pid_target(float temp_val);
float get_pid_target(void);
#endif
