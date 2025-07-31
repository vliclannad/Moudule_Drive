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
#include "keyboard.h"
#include "user_usart.h"
#include "led.h"
#include "ES8388.h" 





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

 按下矩阵键盘对应的按键后，串口输出按键位置



 ****************************************************************/

int main(void)
{
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    __disable_irq(); //禁止所有中断
	Delay_Init();
	USART_Printf_Init(115200);
    LED_Init();
    Keyboard_Init();//按钮矩阵初始化
    //如不使用音频模块，可以关闭避免杂音
    ES8388_close();
    __enable_irq(); //开启所有中断
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("初始化完成\r\n");
    printf("请按下矩阵键盘对应的按键\r\n");
	while(1)
    {
        if (IS_KEY_PRESSED(KEY_11)) {
            printf("Key 11\r\n");
        }
        else if (IS_KEY_PRESSED(KEY_12)) {
            printf("Key 12\r\n");
        }
        else if (IS_KEY_PRESSED(KEY_13)) {
            printf("Key 13\r\n");
        }
        else if (IS_KEY_PRESSED(KEY_21)) {
            printf("Key 21\r\n");
        }
        else if (IS_KEY_PRESSED(KEY_22)) {
            printf("Key 22\r\n");
        }
        else if (IS_KEY_PRESSED(KEY_23)) {
            printf("Key 23\r\n");
        }
        else if (IS_KEY_PRESSED(KEY_31)) {
            printf("Key 31\r\n");
        }
        else if (IS_KEY_PRESSED(KEY_32)) {
            printf("Key 32\r\n");
        }
        else if (IS_KEY_PRESSED(KEY_33)) {
            printf("Key 33\r\n");
        }
        // 清除按键状态
        gkeyboard_status.key_pressed = 0;
        gkeyboard_status.key_released = 0;
        gkeyboard_status.key_long_pressed = 0;
        // 延时100ms，提高响应性
        Delay_Ms(100);
    }
}

