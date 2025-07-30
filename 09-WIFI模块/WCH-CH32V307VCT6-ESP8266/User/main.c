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
 测试功能
WIFI通过TCP连接并发送数据




 ****************************************************************/
int main(void)
{
    uint8_t test = 0;
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    __disable_irq(); //禁止所有中断
	Delay_Init();
	USART_Printf_Init(115200);
    LED_Init();
    //如不使用音频模块，可以关闭避免杂音
    ES8388_close();
    __enable_irq(); //开启所有中断
    WIFI_Init(WIFI_STATION); //初始化WIFI模块为工作站模式
    printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("初始化完成\r\n");
    Delay_Ms(50);
    printf("开始连接WIFI");
    WIFI_LinkSSID((uint8_t*)"1302",(uint8_t*)"xiyukeji13+02");//此处更改为自己的WIFI名称和密码，仅支持2.4G频段
    printf("开始连接TCP服务器");
    WIFI_TCPConnect((uint8_t*)"192.168.3.66",(uint8_t*)"11451");//此处更改为自己的TCP服务器IP和端口,关闭防火墙
    WIFI_SetTxMode(1);//设置透传模式
	while(1)
    {
	    Delay_Ms(5000);
	    USER_UART_Transmit(WIFI_USARTx, &test, 1);
    }
}


