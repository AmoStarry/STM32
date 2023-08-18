#ifndef __task_H__
#define __task_H__

#include "stm32f1xx.h"
#ifdef __cplusplus
 extern "C" {
#endif

typedef struct timer_parameter {
    void (*timeout_cb)(void); 
	uint32_t timeout;
	uint32_t repeat;
}task_timer_parameter;


int task_init(void);
int task_run(void);


#ifdef __cplusplus
}
#endif


#endif
/*****************************END OF FILE*********************************/
