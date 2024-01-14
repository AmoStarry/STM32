#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
#include "Key.h"
#include "semphr.h"

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

//二值信号量句柄
SemaphoreHandle_t BinarySem_Handle =NULL;     

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
         
         BinarySem_Handle = xSemaphoreCreateBinary(); //创建二值信号量
         if(NULL != BinarySem_Handle)
              printf("BinarySem_Handle二值信号量创建成功!\r\n");
         
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
     
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    uint8_t key;
    while(1)
    {
          key = Key_Num();
          if(key == 1)
          {
               xReturn = xSemaphoreGive(BinarySem_Handle);
               if(xReturn == pdTRUE)
                    printf("BinarySem_Handle二值信号量释放成功！\n\r");
               else 
                    printf("BinarySem_Handle二值信号量释放失败！\n\r");
          }
          
    }
}

/* 任务二，实现LED2每500ms翻转一次 */
void task2( void * pvParameters )
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    while(1)
    {
          xReturn = xSemaphoreTake(BinarySem_Handle,portARCH_NAME);/* 二值信号量句柄   xSemaphoreTake获取一个信号量，可以是二值信号量、计数信号量、互斥量。*/ /* 等待时间 */ 
          if( xReturn == pdTRUE )
               printf("BinarySem_Handle二值信号量获取成功!\n\n");
               LED1_Turn();
    }
}

/* 任务三 */
void task3( void * pvParameters )
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    uint8_t key;
    while(1)
    {
          key = Key_Num();
          if(key == 2)
          {
               xReturn = xSemaphoreGive(BinarySem_Handle);
               if(xReturn == pdTRUE)
                    printf("BinarySem_Handle二值信号量释放成功！\n\r");
               else 
                    printf("BinarySem_Handle二值信号量释放失败！\n\r");
          }
          
    }
}
