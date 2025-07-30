#include "bluetooth.h"

void BLE_USART_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

#define maxsize 100  // KMP匹配数组最大长度
u8  TIMOUT = 5; // 超时判断时间  =  TIMOUT*10 ms
u8 RecvData[255] = {0}; // 串口接收数据缓冲区


u8 flag = 2;

float voltage[] = {12.19, 12.37, 11.87, 11.72, 11.74, 11.90, 12.45, 11.61, 12.40, 12.41, 11.85, 11.88, 12.00, 11.70, 12.18, 12.24, 12.17, 11.65, 11.57, 12.22};
float current[] = {2.59, 3.34, 2.52, 2.89, 2.60, 2.92, 3.11, 2.92, 3.11, 2.85,3.27, 2.79, 2.80, 2.92, 2.54, 2.82, 3.15, 3.23, 3.49, 2.74};
u8 inx = 0;

// 本文件使用内部函数
void CAT_CMD_ReturnCheck();
/**
 * @brief BLE模块通讯串口初始化函数
 * @param none    none
 *
 * @return none
 */
void BLE_USART_Config()
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(BLE_USART_CLK , ENABLE);
    RCC_APB2PeriphClockCmd(BLE_USART_GPIO_CLK , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_PinRemapConfig(GPIO_FullRemap_USART4, ENABLE); // 重映射UART4到PE0/PE1

    GPIO_InitStructure.GPIO_Pin = BLE_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(BLE_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = BLE_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(BLE_USART_RX_GPIO_PORT, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = BLE_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(BLE_USARTx, &USART_InitStructure);
    USART_ITConfig(BLE_USARTx, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = BLE_USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(BLE_USARTx, ENABLE);
}

/**
 * @brief 串口中断函数
 * @param none    none
 *
 * @return none
 */
void BLE_USART_IRQHandler()
{
    if(USART_GetITStatus(BLE_USARTx, USART_IT_RXNE) != RESET)
    {
        static uint16_t index = 0;
        u8 data = USART_ReceiveData(BLE_USARTx);
        printf("data:%c\r\n",data);
//        RecvData[Daindex++] = data;
//        last_time = timeout;

        RecvData[index++] = data;
        //printf("%c",data);
        if(data == '\n')
        {
            index = 0;
            CAT_CMD_ReturnCheck();
        }
    }
}

void CAT_CMD_ReturnCheck()
{

    printf("%s",RecvData);
    BLE_USART_SendString(RecvData);
    if(strstr(RecvData,"OK"))
    {
        flag = 1;
    }
    if(strstr(RecvData,"STOP"))
    {
        flag = 0;
    }
    memset(RecvData,0,sizeof(RecvData));
}

/**
 * @brief BLE模块通讯，向模块发送指令
 * @param str    将要发送的数据
 *
 * @return none
 */
void BLE_USART_SendString(u8 *str)
{
    while(*str!='\0')
    {
        USART_SendData(BLE_USARTx, *str++);
        while(USART_GetFlagStatus(BLE_USARTx, USART_FLAG_TXE)==RESET);
    }
}

/**
 * @brief 模块透传模式下发送数据，可以直接发送
 * @param str    将要发送的数据
 *
 * @return none
 */
void BLE_Trans_SendData(u8 *str)
{
    Delay_Ms(1000);
    BLE_USART_SendString(str);
}
/**
 * @brief 模块测试函数，测试服务器连接与数收发
 * @param none
 *
 * @return none
 */
void BLE_Main()
{

    BLE_USART_Config();// 通讯串口初始化

    printf("等待蓝牙连接...\r\n");
}
