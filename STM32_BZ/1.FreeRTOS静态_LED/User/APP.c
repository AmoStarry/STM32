#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
#include "Key.h"
#include "USART.h"

/*-------------------FreeRTOS����--------------------------------*/

/* �������������ջ */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* ��ʱ�������ջ */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];
/* ����������ƿ� */
static StaticTask_t Idle_Task_TCB;	
/* ��ʱ��������ƿ� */
static StaticTask_t Timer_Task_TCB;

/* ���������ڴ���� */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer=&Idle_Task_TCB;/* ������ƿ��ڴ� */
	*ppxIdleTaskStackBuffer=Idle_Task_Stack;/* �����ջ�ڴ� */
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;/* �����ջ��С */
}

/* �����ʱ���ڴ���� */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
									StackType_t **ppxTimerTaskStackBuffer, 
									uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer=&Timer_Task_TCB;/* ������ƿ��ڴ� */
	*ppxTimerTaskStackBuffer=Timer_Task_Stack;/* �����ջ�ڴ� */
	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;/* �����ջ��С */
}

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
StackType_t     start_task_stack[START_TASK_STACK_SIZE];
StaticTask_t    start_task_tcb;
void start_task( void * pvParameters );

/**
  * @brief  TASK1 ���� ����
  * @param  �������ȼ�
  * @param  ��ջ��С
  * @param  ������
  * @retval ��
  */
#define LED1_PRIO        2
#define TASK1_STACK_SIZE  50
TaskHandle_t      LED1_handler;
StackType_t       LED1_stack[TASK1_STACK_SIZE];
StaticTask_t      LED1_tcb;
void LED1( void *pvParameters);

/**
  * @brief  TASK1 ���� ����
  * @param  �������ȼ�
  * @param  ��ջ��С
  * @param  ������
  * @retval ��
  */
#define LED2_PRIO        2
#define TASK2_STACK_SIZE  50
TaskHandle_t      LED2_handler;
StackType_t       LED2_stack[TASK1_STACK_SIZE];
StaticTask_t      LED2_tcb;
void LED2( void *pvParameters);

/**
  * @brief  TASK1 ���� ����
  * @param  �������ȼ�
  * @param  ��ջ��С
  * @param  ������
  * @retval ��
  */
#define KEY_PRIO        2
#define TASK3_STACK_SIZE  50
TaskHandle_t      KEY_handler;
StackType_t       KEY_stack[TASK1_STACK_SIZE];
StaticTask_t      KEY_tcb;
void KEY( void *pvParameters);

     

void APP_turn(void)
{
    start_task_handler =xTaskCreateStatic((TaskFunction_t        )   start_task,             //������
                                         (char *                 )   "start_task",           //��������
                                         (configSTACK_DEPTH_TYPE )   START_TASK_STACK_SIZE,  //�����ջ��С
                                         (void *                 )   NULL,                   //���ݸ��������Ĳ���
                                         (UBaseType_t            )   START_TASK_PRIO,        //�������ȼ�
                                         (StackType_t *          )   start_task_stack,       //�����ջ
                                         (StaticTask_t *         )   &start_task_tcb );      //������ƿ�           
    vTaskStartScheduler();  
}

void start_task( void * pvParameters )
{
    while(1)
    {
          taskENTER_CRITICAL();               /* �����ٽ��� */
          LED1_handler = xTaskCreateStatic((TaskFunction_t         )   LED1,
                                          (char *                 )   "LED1",
                                          (configSTACK_DEPTH_TYPE )   TASK1_STACK_SIZE,
                                          (void *                 )   NULL,             //���ݸ��������Ĳ���
                                          (UBaseType_t            )   START_TASK_PRIO,  //�������ȼ�
                                          (StackType_t *          )   LED1_stack,       //�����ջ
                                          (StaticTask_t *         )   &LED1_tcb );      //������ƿ�  
                     
          LED2_handler = xTaskCreateStatic((TaskFunction_t         )   LED2,
                                          (char *                 )   "LED2",
                                          (configSTACK_DEPTH_TYPE )   TASK2_STACK_SIZE,
                                          (void *                 )   NULL,             //���ݸ��������Ĳ���
                                          (UBaseType_t            )   LED2_PRIO,        //�������ȼ�     
                                          (StackType_t *          )   LED2_stack,       //�����ջ
                                          (StaticTask_t *         )   &LED2_tcb );      //������ƿ� 
                     
          KEY_handler =  xTaskCreateStatic((TaskFunction_t        )    KEY,
                                          (char *                 )   "KEY",
                                          (configSTACK_DEPTH_TYPE )   TASK3_STACK_SIZE,
                                          (void *                 )   NULL,            //���ݸ��������Ĳ���
                                          (UBaseType_t            )   KEY_PRIO,        //�������ȼ�
                                          (StackType_t *          )   KEY_stack,       //�����ջ
                                          (StaticTask_t *         )   &KEY_tcb );      //������ƿ� 
                     
         vTaskDelete(start_task_handler);
         taskEXIT_CRITICAL();                /* �˳��ٽ��� */
     }
}

/* ����һ��ʵ��LED1ÿ500ms��תһ�� */
void LED1( void * pvParameters )
{
    while(1)
    {
        printf("LED2�������У�����\r\n");
        LED2_ON();            //PA1
        vTaskDelay(500);     
        LED2_OFF();
        vTaskDelay(500);

    }
}

/* �������ʵ��LED2ÿ500ms��תһ�� */
void LED2( void * pvParameters )
{
    while(1)
    {
        printf("LED1�������У�����\r\n");
        LED1_ON();            //PA8
        vTaskDelay(500);
        LED1_OFF();
        vTaskDelay(500);
    }
}

/* ������ */
void KEY( void * pvParameters )
{
    uint8_t KeyNum = 0;
    printf("KEY�������У�����\r\n");
    while(1)
    { 
        KeyNum = Key_GetNum();
        if(KeyNum == 1)
        {
           printf("ɾ��LED1����\r\n");
           vTaskDelete(LED1_handler);
           LED1_handler = NULL;   
        }
        else vTaskDelay(500);
          
     }
}

