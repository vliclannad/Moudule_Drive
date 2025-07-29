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
#include "CS100A.h"
#include "ch32v30x_it.h"
#include "user_usart.h"
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
 超声波传感器获取距离通过串口1输出




 ****************************************************************/

void CS100A_TEST(void);
int main(void)
{

    gCS100ACount = 0;
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    __disable_irq(); //禁止所有中断
	Delay_Init();
	USART_Printf_Init(115200);
    LED_Init(); //LED初始化
	CS100A_Init();//超声波传感器初始化
    //如不使用音频模块，可以关闭避免杂音
    ES8388_close();
    __enable_irq(); //开启所有中断
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("初始化完成\r\n");
    printf("开始测试超声波模块\r\n");
	while(1)
    {
	    Delay_Ms(500);
	    CS100A_TEST();
    }
}
//距离传感器测试程序,获取距离通过串口输出
void CS100A_TEST(void)
{
    // 1. 扩大十倍
    CS100A_GET();
    Delay_Ms(200);
    printf("距离为：%.1fcm\r\n",gCS100A_Distance);
}
