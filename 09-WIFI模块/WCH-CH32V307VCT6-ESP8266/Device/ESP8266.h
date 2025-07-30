#ifndef __ESP8266_H
#define __ESP8266_H
#include "debug.h"

// WIFI ����������Ϣ
#define WIFI_USARTx                     UART6
#define WIFI_USART_CLK                  RCC_APB1Periph_UART6
#define WIFI_USART_BAUDRATE             115200

#define WIFI_USART_GPIO_CLK             RCC_APB2Periph_GPIOE
#define WIFI_USART_TX_GPIO_PORT         GPIOE
#define WIFI_USART_TX_GPIO_PIN          GPIO_Pin_10
#define WIFI_USART_RX_GPIO_PORT         GPIOE
#define WIFI_USART_RX_GPIO_PIN          GPIO_Pin_11

#define WIFI_USART_IRQ                  UART6_IRQn
#define WIFI_USART_IRQHandler           UART6_IRQHandler

//���庯������״̬
#define WIFI_OK 0
#define WIFI_ERROR 1
#define WIFI_NORESPONSE 2


//����ģʽ
#define WIFI_STATION 1
#define WIFI_AP      2
#define WIFI_ST_AP   3

//����ģʽ
#define WIFI_RECV_DATA 0
#define WIFI_RECV_CMD  1
#define WIFI_RECV_RST  2

//͸��ģʽ
#define WIFI_NONE_PASS_THROUGH 0
#define WIFI_PASS_THROUGH 1


void WIFI_Init(uint8_t mode_type);
uint8_t WIFI_LinkSSID(uint8_t* ssid,uint8_t* password);
uint8_t WIFI_TCPConnect(uint8_t* server_ip,uint8_t* server_port);
uint8_t WIFI_Disconnnect();
uint8_t WIFI_SetTxMode(uint8_t modetype);

#endif
