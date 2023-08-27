#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
#include "Key.h"

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

/* ����һ������ */
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
                         printf("���������");
                    }
                    if(Flag1 == 0)
                    {
                         vTaskResume(task2_handler);
                         printf("�ָ������");
                    }
                    break;
               case KEY1_PRES:
                    Flag2 =!Flag2;
                    if(Flag2 == 1)
                    {
                         vTaskSuspend(task3_handler);
                         printf("����������");
                    }
                    if(Flag2 == 0)
                    {
                         vTaskResume(task3_handler);
                         printf("�ָ�������");
                    }
                    break;
          }
    }
}

/* �������ʵ��LED2ÿ500ms��תһ�� */
void task2( void * pvParameters )
{
     uint8_t task2_num =0;
    while(1)
    {
          task2_num ++;
          printf("task2������:%d ��\r\n",task2_num);
          LED1_Turn();
          vTaskDelay(500);
    }
}

/* ������ */
void task3( void * pvParameters )
{
    uint8_t task3_num = 0;
    while(1)
    {
          
          task3_num ++;
          printf("task3������:%d ��\r\n",task3_num);
          LED2_Turn();
          vTaskDelay(500);
    }
}
