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
#include "motor.h"
#include "keyboard.h"
#include "led.h"
/* Global typedef */

/* Global define */

/* Global Variable */
//功能
//可用过按钮矩阵控制三相电机运转


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
/****************************************************************


 ****************************************************************/

int main(void)
{
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // 初始化电机和按键
    __disable_irq();
    Delay_Init();
	USART_Printf_Init(115200);
	LED_Init();
    Motor_Init(20000, 72, 900);//固定频率50Hz
    Keyboard_Init();
    ES8388_close(); // 关闭ES8388音频芯片
    __enable_irq();
    printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("初始化完成\r\n");
    LED_GREEN_ON();
    while(1)
    {
        // 需要先解锁电调
        if (gkeyboard_status.key_pressed & (1<<KEY_11)) {
            Motor_Start();//解锁电调
            printf("Key 11: 电机启动\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_12)) {
            Motor_StopGradual();   // 渐变停止
            printf("Key 12: 电机渐变停止\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_13)) {
            Motor_Stop();
            printf("Key 13: 电机急停\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_21)) {
            Motor_SetThrottleGradual(25, 20);  // 渐变到25%油门
            printf("Key 21: 电机油门 25%%\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_22)) {
            Motor_SetThrottleGradual(50, 20); // 渐变到50%油门
            printf("Key 22: 电机油门 50%%\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_23)) {
            Motor_SetThrottleGradual(100, 20); // 渐变到100%油门
            printf("Key 23: 全★速★前★进★DA!\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_31)) {
            Motor_ThrottleStepUp(10); // 油门步进增加10%
            printf("Key 31: 油门步进增加10%%\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_32)) {
            Motor_ThrottleStepDown(10); // 油门步进减少10%
            printf("Key 32: 油门步进减少10%%\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_33)) {
        }
        // 清除按键状态
        gkeyboard_status.key_pressed = 0;
        gkeyboard_status.key_released = 0;
        gkeyboard_status.key_long_pressed = 0;
        // 延时100ms，提高响应性
        Delay_Ms(100);
    }

}
