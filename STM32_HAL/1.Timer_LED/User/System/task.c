#include "config.h"
#include "task.h"

/*
 *Note:In order to add new task ,we must modify 4 places :
 *1.TASK_TIMER_CNT
 *2.timeout_cb declare
 *3.task_timer_para
 *4.timeout_cb realize
*/

/*--------------------------------1.任务定时器变量定义----------------------------------*/
#define TASK_TIMER_CNT   2   //任务定时器个数,增加任务时须修改该宏值
struct Timer task_timer[TASK_TIMER_CNT];
/*---------------------------------------------------------------------------------------*/


/*---------------2.任务回调函数声明(增加任务时须添加对应的回调函数声明)-----------------*/
void led0_task_timer_callback(void);//任务回调函数声明
void led1_task_timer_callback(void);//任务回调函数声明

/*---------------------------------------------------------------------------------------*/

/*---------------3.任务定时器参数(增加任务时须按如下格式添加对应的任务定时器参数)--------*/
task_timer_parameter task_timer_para[TASK_TIMER_CNT] =
 {
   /*{timeout_cb,				timeout(ms),		repeat(ms)}*/
	 {led0_task_timer_callback,	1000,			    1000},
	 {led1_task_timer_callback,	500,			    500},
	 
   /*{timeout_cb,				timeout(ms),		repeat(ms)}*/ 
 };
/*---------------------------------------------------------------------------------------*/       


/*--------4.任务定时器回调函数实现(增加任务时须添加对应的定时器回调函数实现)------------*/

void led0_task_timer_callback(void)//任务回调函数实现
{
	//led0_toggle();
	
}

void led1_task_timer_callback(void)//任务回调函数实现
{
	//led1_toggle();
	
}

/*---------------------------------------------------------------------------------------*/


/*--任务初始化task_init(sys_init()中调用)及运行函数task_run(main.c中调用)，本部分无须修改--*/

int task_init(void)
{   
	uint8_t task_index;
	for(task_index = 0;task_index < TASK_TIMER_CNT;task_index++)
	{
	    timer_init(&task_timer[task_index],
		           task_timer_para[task_index].timeout_cb, 
		           task_timer_para[task_index].timeout,
		           task_timer_para[task_index].repeat); 
		
	    timer_start(&task_timer[task_index]);
	}
	return 0;
}

int task_run(void)
{
	timer_loop();
	return 0;
}
/*---------------------------------------------------------------------------------------*/
