#ifndef __ESP8266_H
#define __ESP8266_H
#include "debug.h"

// CAT 串口配置信息
#define CAT_USARTx                     UART5
#define CAT_USART_CLK                  RCC_APB1Periph_UART5
#define CAT_USART_BAUDRATE             115200

#define CAT_USART_GPIO_CLK             RCC_APB2Periph_GPIOE
#define CAT_USART_TX_GPIO_PORT         GPIOE
#define CAT_USART_TX_GPIO_PIN          GPIO_Pin_8
#define CAT_USART_RX_GPIO_PORT         GPIOE
#define CAT_USART_RX_GPIO_PIN          GPIO_Pin_9

#define CAT_USART_IRQ                  UART5_IRQn
#define CAT_USART_IRQHandler           UART5_IRQHandler


// CAT使能GPIO引脚，高电平导通
#define CAT_Reboot_GPIO_CLK         RCC_APB2Periph_GPIOD
#define CAT_Reboot_GPIO_PORT        GPIOD
#define CAT_Reboot_GPIO_PIN         GPIO_Pin_4


//定义函数返回状态
#define CAT_OK 0
#define CAT_ERROR 1
#define CAT_NORESPONSE 2


//工作模式
#define CAT_STATION 1
//#define CAT_AP      2
//#define CAT_ST_AP   3

//接收模式
#define CAT_RECV_DATA 0
#define CAT_RECV_CMD  1
#define CAT_RECV_RST  2

//透传模式
#define CAT_NONE_PASS_THROUGH 0
#define CAT_PASS_THROUGH 1


void CAT_Init(uint8_t mode_type);
uint8_t CAT_LinkSSID(uint8_t* ssid,uint8_t* password);
uint8_t CAT_TCPConnect(uint8_t* server_ip,uint8_t* server_port);
uint8_t CAT_Disconnnect();
uint8_t CAT_SetTxMode(uint8_t modetype);

#endif
