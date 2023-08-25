#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"

/*-------------------FreeRTOS����--------------------------------*/
/**
  * @brief  START_TASK ���� ����
  * @param  �������ȼ�
  * @param  ��ջ��С
  * @param  ������
  * @retval ��
  */
#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   512
TaskHandle_t    start_task_handler;
void start_task( void * pvParameters );

/**
  * @brief  TASK1 ���� ����
  * @param  �������ȼ�
  * @param  ��ջ��С
  * @param  ������
  * @retval ��
  */
#define TASK1_PRIO        2
#define TASK1_STACK_SIZE  50
TaskHandle_t      task1_handler;
void task1( void *pvParameters);

/**
  * @brief  TASK1 ���� ����
  * @param  �������ȼ�
  * @param  ��ջ��С
  * @param  ������
  * @retval ��
  */
#define TASK2_PRIO        3
#define TASK2_STACK_SIZE  50
TaskHandle_t      task2_handler;
void task2( void *pvParameters);

/**
  * @brief  TASK1 ���� ����
  * @param  �������ȼ�
  * @param  ��ջ��С
  * @param  ������
  * @retval ��
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
         taskENTER_CRITICAL();               /* �����ٽ��� */
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
         taskEXIT_CRITICAL();                /* �˳��ٽ��� */
     }
}

/* ����һ��ʵ��LED1ÿ500ms��תһ�� */
void task1( void * pvParameters )
{
    while(1)
    {
        printf("task1�������У�����\r\n");
        LED1_ON();
        vTaskDelay(500);
        LED1_OFF();
        vTaskDelay(500);
    }
}

/* �������ʵ��LED2ÿ500ms��תһ�� */
void task2( void * pvParameters )
{
    while(1)
    {
        printf("task2�������У�����\r\n");
        LED2_ON();
        vTaskDelay(500);
        LED2_OFF();
        vTaskDelay(500);
    }
}

/* ������ */
void task3( void * pvParameters )
{
//    uint8_t key = 0;
    while(1)
    {
          
        vTaskDelay(500);
         
        printf("task3�������У�����\r\n");
//        key = key_scan(0);
//        if(key == KEY0_PRES)
//        {
//            if(task1_handler != NULL)
//            {
//                printf("ɾ��task1����\r\n");
//                vTaskDelete(task1_handler);
//                task1_handler = NULL;
//            }

//        }
//        vTaskDelay(10);
    }
}
