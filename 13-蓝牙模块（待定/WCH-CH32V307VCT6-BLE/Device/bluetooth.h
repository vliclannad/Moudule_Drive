#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H
#include "ch32v30x.h"
#include "string.h"
#include "debug.h"
// 调试串口
#define Debug_USARTx                   UART4

// BLE 串口配置信息
#define BLE_USARTx                     UART4
#define BLE_USART_CLK                  RCC_APB1Periph_UART4
#define BLE_USART_BAUDRATE             115200

#define BLE_USART_GPIO_CLK             RCC_APB2Periph_GPIOE
#define BLE_USART_TX_GPIO_PORT         GPIOE
#define BLE_USART_TX_GPIO_PIN          GPIO_Pin_0
#define BLE_USART_RX_GPIO_PORT         GPIOE
#define BLE_USART_RX_GPIO_PIN          GPIO_Pin_1

#define BLE_USART_IRQ                  UART4_IRQn
#define BLE_USART_IRQHandler           UART4_IRQHandler



void BLE_USART_Config(void);  // BLE通讯端口初始化函数
void BLE_USART_SendString(u8 *str); // BLE串口发送数据，可通过该函数发送AT指令

void BLE_Trans_SendData(u8 *str);// 在数据透传模式下，发送数据

void BLE_Main(void); // BLE模块测试函数，通过该函数可以测试连接与数据发送
void BLE_analysis(u8 *str);// 解析函数，解析串口收到的数据，判断模块是否连接成功，并且进行下一步

#endif
