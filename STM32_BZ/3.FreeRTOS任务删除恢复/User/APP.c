#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"

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

/* 任务一，实现LED1每500ms翻转一次 */
void task1( void * pvParameters )
{
    while(1)
    {
        printf("task1正在运行！！！\r\n");
        LED1_ON();
        vTaskDelay(500);
        LED1_OFF();
        vTaskDelay(500);
    }
}

/* 任务二，实现LED2每500ms翻转一次 */
void task2( void * pvParameters )
{
    while(1)
    {
        printf("task2正在运行！！！\r\n");
        LED2_ON();
        vTaskDelay(500);
        LED2_OFF();
        vTaskDelay(500);
    }
}

/* 任务三 */
void task3( void * pvParameters )
{
//    uint8_t key = 0;
    while(1)
    {
          
        vTaskDelay(500);
         
        printf("task3正在运行！！！\r\n");
//        key = key_scan(0);
//        if(key == KEY0_PRES)
//        {
//            if(task1_handler != NULL)
//            {
//                printf("删除task1任务\r\n");
//                vTaskDelete(task1_handler);
//                task1_handler = NULL;
//            }

//        }
//        vTaskDelay(10);
    }
}
