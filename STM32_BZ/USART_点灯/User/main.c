#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "usart.h"
#include "key.h"
#include "string.h"
#include "LED.h"

uint8_t add[5] ={0x41,0x11,0x12,0x13,0x14};
uint8_t RxData;
void sendData(uint8_t data)
{
    // �ȴ����ͻ�����Ϊ��
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;

    // ��������
    USART_SendData(USART1, data);
}

int main(void)
{
    // ��ʼ��ϵͳʱ�Ӻ�GPIO
    // ...

    // ��ʼ��USART
     USART1_Init(9600);

    while (1)
    {
        // ��������
        if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
        {
            uint8_t data = USART_ReceiveData(USART1);

            // ���ݽ��յ������ݿ���GPIOA1
            if (data == 1)
            {
                GPIO_SetBits(GPIOA, GPIO_Pin_1);
            }
            else if (data == 0)
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_1);
            }
        }
    }
}

