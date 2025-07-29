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
 ���Թ���
WS2812��ˮ��ģ��ǰ����С�Ʒֱ���ʾ�졢�̡���
 ****************************************************************/

int main(void)
{
    // ����9���������ɫ�������� (GRB��ʽ��ÿ������3�ֽ�)
    u8 led_data[27] = {0}; // 9������ �� 3�ֽ�/���� = 27�ֽ�
    u8 position = 0;       // ��ǰ����λ��
    u8 color_mode = 0;     // ��ɫģʽ
    u16 delay_time = 200;  // ��ʱʱ��(ms)
    
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    __disable_irq();
	Delay_Init();
	USART_Printf_Init(115200);
	LED_Init();
	WS_Init();

    //�粻ʹ����Ƶģ�飬���Թرձ�������
    ES8388_close();

    __enable_irq();
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID());
    printf("��ʼ�����\r\n");
    printf("WS2812B��ˮ����ʾ��ʼ\r\n");

    LED_GREEN_ON();
    
	while(1)
    {
        // ������е���
        for(int i = 0; i < 27; i++) {
            led_data[i] = 0;
        }
        
        // ������ɫģʽ���õ�ǰλ�õ���ɫ
        switch(color_mode) {
            case 0: // ��ɫ��ˮ��
                led_data[position * 3 + 0] = 0x00;    // G
                led_data[position * 3 + 1] = 0x20;    // R
                led_data[position * 3 + 2] = 0x00;    // B
                break;
            case 1: // ��ɫ��ˮ��
                led_data[position * 3 + 0] = 0x20;    // G
                led_data[position * 3 + 1] = 0x00;    // R
                led_data[position * 3 + 2] = 0x00;    // B
                break;
            case 2: // ��ɫ��ˮ��
                led_data[position * 3 + 0] = 0x00;    // G
                led_data[position * 3 + 1] = 0x00;    // R
                led_data[position * 3 + 2] = 0x20;    // B
                break;
            case 3: // �ʺ���ˮ��
                if(position % 3 == 0) {
                    led_data[position * 3 + 0] = 0x00; led_data[position * 3 + 1] = 0x20; led_data[position * 3 + 2] = 0x00; // ��ɫ
                } else if(position % 3 == 1) {
                    led_data[position * 3 + 0] = 0x20; led_data[position * 3 + 1] = 0x00; led_data[position * 3 + 2] = 0x00; // ��ɫ
                } else {
                    led_data[position * 3 + 0] = 0x00; led_data[position * 3 + 1] = 0x00; led_data[position * 3 + 2] = 0x20; // ��ɫ
                }
                break;
            case 4: // ��βЧ��
                // ��ǰλ������
                led_data[position * 3 + 0] = 0x00;
                led_data[position * 3 + 1] = 0x30;
                led_data[position * 3 + 2] = 0x00;
                // ǰһ��λ���԰�
                if(position > 0) {
                    led_data[(position-1) * 3 + 0] = 0x00;
                    led_data[(position-1) * 3 + 1] = 0x10;
                    led_data[(position-1) * 3 + 2] = 0x00;
                } else {
                    led_data[8 * 3 + 0] = 0x00;
                    led_data[8 * 3 + 1] = 0x10;
                    led_data[8 * 3 + 2] = 0x00;
                }
                // ǰ����λ�ø���
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
        
        // �������ݵ�WS2812�ƴ�
        WS_SendOnePix(led_data, 9);
        
        // �ƶ�����һ��λ��
        position++;
        if(position >= 9) {
            position = 0;
            color_mode++;
            if(color_mode >= 5) {
                color_mode = 0;
            }
            printf("�л�����ɫģʽ: %d\r\n", color_mode);
        }
        
        Delay_Ms(delay_time);
    }
}
