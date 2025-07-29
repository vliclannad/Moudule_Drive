#include "user_usart.h"




void USER_UART_Transmit(USART_TypeDef *USARTx,uint8_t *Data,uint32_t count)
{
    while(count--)
    {
        USART_SendData(USARTx, *Data++);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET);

    }

}
