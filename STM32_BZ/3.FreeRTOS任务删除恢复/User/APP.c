#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
#include "Key.h"

/*-------------------FreeRTOS配置--------------------------------*/
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
void start_task( void * pvParameters );

/**
  * @brief  TASK1 任务 配置
  * @param  任务优先级
  * @param  堆栈大小
  * @param  任务句柄
  * @retval 无
  */
#define TASK1_PRIO        2
#define TASK1_STACK_SIZE  50
TaskHandle_t      task1_handler;
void task1( void *pvParameters);

/**
  * @brief  TASK1 任务 配置
  * @param  任务优先级
  * @param  堆栈大小
  * @param  任务句柄
  * @retval 无
  */
#define TASK2_PRIO        3
#define TASK2_STACK_SIZE  50
TaskHandle_t      task2_handler;
void task2( void *pvParameters);

/**
  * @brief  TASK1 任务 配置
  * @param  任务优先级
  * @param  堆栈大小
  * @param  任务句柄
  * @retval 无
  */
#define TASK3_PRIO        4
#define TASK3_STACK_SIZE  50
TaskHandle_t      task3_handler;
void task3( void *pvParameters);

     

void APP_turn(void)
{
    xTaskCreate((TaskFunction_t         )   start_task,
                (char *                 )   "start_task",
                (configSTACK_DEPTH_TYPE )   START_TASK_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   START_TASK_PRIO,
                (TaskHandle_t *         )   &start_task_handler );
                                   
    vTaskStartScheduler();  
}

void start_task( void * pvParameters )
{
    while(1)
    {
         taskENTER_CRITICAL();               /* 进入临界区 */
         xTaskCreate((TaskFunction_t         )   task1,
                     (char *                 )   "task1",
                     (configSTACK_DEPTH_TYPE )   TASK1_STACK_SIZE,
                     (void *                 )   NULL,
                     (UBaseType_t            )   TASK1_PRIO,
                     (TaskHandle_t *         )   &task1_handler );
                     
         xTaskCreate((TaskFunction_t         )   task2,
                     (char *                 )   "task2",
                     (configSTACK_DEPTH_TYPE )   TASK2_STACK_SIZE,
                     (void *                 )   NULL,
                     (UBaseType_t            )   TASK2_PRIO,
                     (TaskHandle_t *         )   &task2_handler );
                     
         xTaskCreate((TaskFunction_t         )   task3,
                     (char *                 )   "task3",
                     (configSTACK_DEPTH_TYPE )   TASK3_STACK_SIZE,
                     (void *                 )   NULL,
                     (UBaseType_t            )   TASK3_PRIO,
                     (TaskHandle_t *         )   &task3_handler );
                     
         vTaskDelete(NULL);
         taskEXIT_CRITICAL();                /* 退出临界区 */
     }
}

/* 任务一，按键 */
void task1( void * pvParameters )
{
    uint8_t key,Flag1,Flag2;
    while(1)
    {
          key = Key_Num(); 
          switch (key)
          {
               case KEY0_PRES:
                    Flag1 =!Flag1;
                    if(Flag1 == 1)
                    {
                         vTaskSuspend(task2_handler);
                         printf("挂起任务二");
                    }
                    if(Flag1 == 0)
                    {
                         vTaskResume(task2_handler);
                         printf("恢复任务二");
                    }
                    break;
               case KEY1_PRES:
                    Flag2 =!Flag2;
                    if(Flag2 == 1)
                    {
                         vTaskSuspend(task3_handler);
                         printf("挂起任务三");
                    }
                    if(Flag2 == 0)
                    {
                         vTaskResume(task3_handler);
                         printf("恢复任务三");
                    }
                    break;
          }
    }
}

/* 任务二，实现LED2每500ms翻转一次 */
void task2( void * pvParameters )
{
     uint8_t task2_num =0;
    while(1)
    {
          task2_num ++;
          printf("task2已运行:%d 次\r\n",task2_num);
          LED1_Turn();
          vTaskDelay(500);
    }
}

/* 任务三 */
void task3( void * pvParameters )
{
    uint8_t task3_num = 0;
    while(1)
    {
          
          task3_num ++;
          printf("task3已运行:%d 次\r\n",task3_num);
          LED2_Turn();
          vTaskDelay(500);
    }
}
