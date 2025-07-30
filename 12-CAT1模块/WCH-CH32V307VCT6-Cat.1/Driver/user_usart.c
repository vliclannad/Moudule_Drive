#include "user_usart.h"



//����ָ���������ַ�
void USER_UART_Transmit(USART_TypeDef *USARTx,uint8_t *Data,uint32_t count)
{
    while(count--)
    {
        USART_SendData(USARTx, *Data++);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET);

    }

}

//������'\0'Ϊ��β���ַ���
void USER_UART_SendString(USART_TypeDef *USARTx,uint8_t *Data)
{
    while(*Data!='\0')
    {
        USART_SendData(USARTx, *Data++);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET);
    }
}
