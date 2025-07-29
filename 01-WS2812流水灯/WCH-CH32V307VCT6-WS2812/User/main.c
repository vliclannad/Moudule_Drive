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
#include "ws2812-B.h"
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
WS2812流水灯模块前三个小灯分别显示红、绿、蓝
 ****************************************************************/

int main(void)
{
    // 定义9个灯珠的颜色数据数组 (GRB格式，每个灯珠3字节)
    u8 led_data[27] = {0}; // 9个灯珠 × 3字节/灯珠 = 27字节
    u8 position = 0;       // 当前亮灯位置
    u8 color_mode = 0;     // 颜色模式
    u16 delay_time = 200;  // 延时时间(ms)
    
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    __disable_irq();
	Delay_Init();
	USART_Printf_Init(115200);
	LED_Init();
	WS_Init();

    //如不使用音频模块，可以关闭避免杂音
    ES8388_close();

    __enable_irq();
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID());
    printf("初始化完成\r\n");
    printf("WS2812B流水灯演示开始\r\n");

    LED_GREEN_ON();
    
	while(1)
    {
        // 清空所有灯珠
        for(int i = 0; i < 27; i++) {
            led_data[i] = 0;
        }
        
        // 根据颜色模式设置当前位置的颜色
        switch(color_mode) {
            case 0: // 红色流水灯
                led_data[position * 3 + 0] = 0x00;    // G
                led_data[position * 3 + 1] = 0x20;    // R
                led_data[position * 3 + 2] = 0x00;    // B
                break;
            case 1: // 绿色流水灯
                led_data[position * 3 + 0] = 0x20;    // G
                led_data[position * 3 + 1] = 0x00;    // R
                led_data[position * 3 + 2] = 0x00;    // B
                break;
            case 2: // 蓝色流水灯
                led_data[position * 3 + 0] = 0x00;    // G
                led_data[position * 3 + 1] = 0x00;    // R
                led_data[position * 3 + 2] = 0x20;    // B
                break;
            case 3: // 彩虹流水灯
                if(position % 3 == 0) {
                    led_data[position * 3 + 0] = 0x00; led_data[position * 3 + 1] = 0x20; led_data[position * 3 + 2] = 0x00; // 红色
                } else if(position % 3 == 1) {
                    led_data[position * 3 + 0] = 0x20; led_data[position * 3 + 1] = 0x00; led_data[position * 3 + 2] = 0x00; // 绿色
                } else {
                    led_data[position * 3 + 0] = 0x00; led_data[position * 3 + 1] = 0x00; led_data[position * 3 + 2] = 0x20; // 蓝色
                }
                break;
            case 4: // 拖尾效果
                // 当前位置最亮
                led_data[position * 3 + 0] = 0x00;
                led_data[position * 3 + 1] = 0x30;
                led_data[position * 3 + 2] = 0x00;
                // 前一个位置稍暗
                if(position > 0) {
                    led_data[(position-1) * 3 + 0] = 0x00;
                    led_data[(position-1) * 3 + 1] = 0x10;
                    led_data[(position-1) * 3 + 2] = 0x00;
                } else {
                    led_data[8 * 3 + 0] = 0x00;
                    led_data[8 * 3 + 1] = 0x10;
                    led_data[8 * 3 + 2] = 0x00;
                }
                // 前两个位置更暗
                if(position > 1) {
                    led_data[(position-2) * 3 + 0] = 0x00;
                    led_data[(position-2) * 3 + 1] = 0x05;
                    led_data[(position-2) * 3 + 2] = 0x00;
                } else if(position == 1) {
                    led_data[8 * 3 + 0] = 0x00;
                    led_data[8 * 3 + 1] = 0x05;
                    led_data[8 * 3 + 2] = 0x00;
                } else {
                    led_data[7 * 3 + 0] = 0x00;
                    led_data[7 * 3 + 1] = 0x05;
                    led_data[7 * 3 + 2] = 0x00;
                }
                break;
        }
        
        // 发送数据到WS2812灯带
        WS_SendOnePix(led_data, 9);
        
        // 移动到下一个位置
        position++;
        if(position >= 9) {
            position = 0;
            color_mode++;
            if(color_mode >= 5) {
                color_mode = 0;
            }
            printf("切换到颜色模式: %d\r\n", color_mode);
        }
        
        Delay_Ms(delay_time);
    }
}
