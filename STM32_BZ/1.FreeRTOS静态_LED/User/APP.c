#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
#include "Key.h"
#include "USART.h"

/*-------------------FreeRTOS配置--------------------------------*/

/* 空闲任务任务堆栈 */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* 定时器任务堆栈 */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];
/* 空闲任务控制块 */
static StaticTask_t Idle_Task_TCB;	
/* 定时器任务控制块 */
static StaticTask_t Timer_Task_TCB;

/* 空闲任务内存分配 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer=&Idle_Task_TCB;/* 任务控制块内存 */
	*ppxIdleTaskStackBuffer=Idle_Task_Stack;/* 任务堆栈内存 */
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;/* 任务堆栈大小 */
}

/* 软件定时器内存分配 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
									StackType_t **ppxTimerTaskStackBuffer, 
									uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer=&Timer_Task_TCB;/* 任务控制块内存 */
	*ppxTimerTaskStackBuffer=Timer_Task_Stack;/* 任务堆栈内存 */
	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;/* 任务堆栈大小 */
}

/**
  * @brief  START_TASK 任务 配置
  * @param  任务优先级
  * @param  堆栈大小
  * @param  任务句柄
  * @retval 无
  */
#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   512
TaskHandle_t    start_task_handler;
StackType_t     start_task_stack[START_TASK_STACK_SIZE];
StaticTask_t    start_task_tcb;
void start_task( void * pvParameters );

/**
  * @brief  TASK1 任务 配置
  * @param  任务优先级
  * @param  堆栈大小
  * @param  任务句柄
  * @retval 无
  */
#define LED1_PRIO        2
#define TASK1_STACK_SIZE  50
TaskHandle_t      LED1_handler;
StackType_t       LED1_stack[TASK1_STACK_SIZE];
StaticTask_t      LED1_tcb;
void LED1( void *pvParameters);

/**
  * @brief  TASK1 任务 配置
  * @param  任务优先级
  * @param  堆栈大小
  * @param  任务句柄
  * @retval 无
  */
#define LED2_PRIO        2
#define TASK2_STACK_SIZE  50
TaskHandle_t      LED2_handler;
StackType_t       LED2_stack[TASK1_STACK_SIZE];
StaticTask_t      LED2_tcb;
void LED2( void *pvParameters);

/**
  * @brief  TASK1 任务 配置
  * @param  任务优先级
  * @param  堆栈大小
  * @param  任务句柄
  * @retval 无
  */
#define KEY_PRIO        2
#define TASK3_STACK_SIZE  50
TaskHandle_t      KEY_handler;
StackType_t       KEY_stack[TASK1_STACK_SIZE];
StaticTask_t      KEY_tcb;
void KEY( void *pvParameters);

     

void APP_turn(void)
{
    start_task_handler =xTaskCreateStatic((TaskFunction_t        )   start_task,             //任务函数
                                         (char *                 )   "start_task",           //任务名称
                                         (configSTACK_DEPTH_TYPE )   START_TASK_STACK_SIZE,  //任务堆栈大小
                                         (void *                 )   NULL,                   //传递给任务函数的参数
                                         (UBaseType_t            )   START_TASK_PRIO,        //任务优先级
                                         (StackType_t *          )   start_task_stack,       //任务堆栈
                                         (StaticTask_t *         )   &start_task_tcb );      //任务控制块           
    vTaskStartScheduler();  
}

void start_task( void * pvParameters )
{
    while(1)
    {
          taskENTER_CRITICAL();               /* 进入临界区 */
          LED1_handler = xTaskCreateStatic((TaskFunction_t         )   LED1,
                                          (char *                 )   "LED1",
                                          (configSTACK_DEPTH_TYPE )   TASK1_STACK_SIZE,
                                          (void *                 )   NULL,             //传递给任务函数的参数
                                          (UBaseType_t            )   START_TASK_PRIO,  //任务优先级
                                          (StackType_t *          )   LED1_stack,       //任务堆栈
                                          (StaticTask_t *         )   &LED1_tcb );      //任务控制块  
                     
          LED2_handler = xTaskCreateStatic((TaskFunction_t         )   LED2,
                                          (char *                 )   "LED2",
                                          (configSTACK_DEPTH_TYPE )   TASK2_STACK_SIZE,
                                          (void *                 )   NULL,             //传递给任务函数的参数
                                          (UBaseType_t            )   LED2_PRIO,        //任务优先级     
                                          (StackType_t *          )   LED2_stack,       //任务堆栈
                                          (StaticTask_t *         )   &LED2_tcb );      //任务控制块 
                     
          KEY_handler =  xTaskCreateStatic((TaskFunction_t        )    KEY,
                                          (char *                 )   "KEY",
                                          (configSTACK_DEPTH_TYPE )   TASK3_STACK_SIZE,
                                          (void *                 )   NULL,            //传递给任务函数的参数
                                          (UBaseType_t            )   KEY_PRIO,        //任务优先级
                                          (StackType_t *          )   KEY_stack,       //任务堆栈
                                          (StaticTask_t *         )   &KEY_tcb );      //任务控制块 
                     
         vTaskDelete(start_task_handler);
         taskEXIT_CRITICAL();                /* 退出临界区 */
     }
}

/* 任务一，实现LED1每500ms翻转一次 */
void LED1( void * pvParameters )
{
    while(1)
    {
        printf("LED2正在运行！！！\r\n");
        LED2_ON();            //PA1
        vTaskDelay(500);     
        LED2_OFF();
        vTaskDelay(500);

    }
}

/* 任务二，实现LED2每500ms翻转一次 */
void LED2( void * pvParameters )
{
    while(1)
    {
        printf("LED1正在运行！！！\r\n");
        LED1_ON();            //PA8
        vTaskDelay(500);
        LED1_OFF();
        vTaskDelay(500);
    }
}

/* 任务三 */
void KEY( void * pvParameters )
{
    uint8_t KeyNum = 0;
    printf("KEY正在运行！！！\r\n");
    while(1)
    { 
        KeyNum = Key_GetNum();
        if(KeyNum == 1)
        {
           printf("删除LED1任务\r\n");
           vTaskDelete(LED1_handler);
           LED1_handler = NULL;   
        }
        else vTaskDelay(500);
          
     }
}

