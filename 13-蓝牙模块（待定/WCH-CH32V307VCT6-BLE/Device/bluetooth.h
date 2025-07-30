#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H
#include "ch32v30x.h"
#include "string.h"
#include "debug.h"
// ���Դ���
#define Debug_USARTx                   UART4

// BLE ����������Ϣ
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



void BLE_USART_Config(void);  // BLEͨѶ�˿ڳ�ʼ������
void BLE_USART_SendString(u8 *str); // BLE���ڷ������ݣ���ͨ���ú�������ATָ��

void BLE_Trans_SendData(u8 *str);// ������͸��ģʽ�£���������

void BLE_Main(void); // BLEģ����Ժ�����ͨ���ú������Բ������������ݷ���
void BLE_analysis(u8 *str);// �������������������յ������ݣ��ж�ģ���Ƿ����ӳɹ������ҽ�����һ��

#endif
