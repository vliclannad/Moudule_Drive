/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/
#define GLOBLE_VAR
#include "string.h"
#include "debug.h"
#include "user.h"
#include "ch32v30x_it.h"
#include "user_usart.h"
#include "ESP8266.h"
#include "ES8388.h"
#include "led.h"




/* Global typedef */

/* Global define */

/* Global Variable */


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
/****************************************************************
 ���Թ���
WIFIͨ��TCP���Ӳ���������




 ****************************************************************/
int main(void)
{
    uint8_t test = 0;
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    __disable_irq(); //��ֹ�����ж�
	Delay_Init();
	USART_Printf_Init(115200);
    LED_Init();
    //�粻ʹ����Ƶģ�飬���Թرձ�������
    ES8388_close();
    __enable_irq(); //���������ж�
    WIFI_Init(WIFI_STATION); //��ʼ��WIFIģ��Ϊ����վģʽ
    printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("��ʼ�����\r\n");
    Delay_Ms(50);
    printf("��ʼ����WIFI");
    WIFI_LinkSSID((uint8_t*)"1302",(uint8_t*)"xiyukeji13+02");//�˴�����Ϊ�Լ���WIFI���ƺ����룬��֧��2.4GƵ��
    printf("��ʼ����TCP������");
    WIFI_TCPConnect((uint8_t*)"192.168.3.66",(uint8_t*)"11451");//�˴�����Ϊ�Լ���TCP������IP�Ͷ˿�,�رշ���ǽ
    WIFI_SetTxMode(1);//����͸��ģʽ
	while(1)
    {
	    Delay_Ms(5000);
	    USER_UART_Transmit(WIFI_USARTx, &test, 1);
    }
}


