#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
#include "Key.h"
#include "semphr.h"

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

//��ֵ�ź������
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
         taskENTER_CRITICAL();               /* �����ٽ��� */
         
         BinarySem_Handle = xSemaphoreCreateBinary(); //������ֵ�ź���
         if(NULL != BinarySem_Handle)
              printf("BinarySem_Handle��ֵ�ź��������ɹ�!\r\n");
         
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
     
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    uint8_t key;
    while(1)
    {
          key = Key_Num();
          if(key == 1)
          {
               xReturn = xSemaphoreGive(BinarySem_Handle);
               if(xReturn == pdTRUE)
                    printf("BinarySem_Handle��ֵ�ź����ͷųɹ���\n\r");
               else 
                    printf("BinarySem_Handle��ֵ�ź����ͷ�ʧ�ܣ�\n\r");
          }
          
    }
}

/* �������ʵ��LED2ÿ500ms��תһ�� */
void task2( void * pvParameters )
{
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    while(1)
    {
          xReturn = xSemaphoreTake(BinarySem_Handle,portARCH_NAME);/* ��ֵ�ź������   xSemaphoreTake��ȡһ���ź����������Ƕ�ֵ�ź����������ź�������������*/ /* �ȴ�ʱ�� */ 
          if( xReturn == pdTRUE )
               printf("BinarySem_Handle��ֵ�ź�����ȡ�ɹ�!\n\n");
               LED1_Turn();
    }
}

/* ������ */
void task3( void * pvParameters )
{
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    uint8_t key;
    while(1)
    {
          key = Key_Num();
          if(key == 2)
          {
               xReturn = xSemaphoreGive(BinarySem_Handle);
               if(xReturn == pdTRUE)
                    printf("BinarySem_Handle��ֵ�ź����ͷųɹ���\n\r");
               else 
                    printf("BinarySem_Handle��ֵ�ź����ͷ�ʧ�ܣ�\n\r");
          }
          
    }
}
