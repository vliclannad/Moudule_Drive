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
 ���Թ���

 ���¾�����̶�Ӧ�İ����󣬴����������λ��



 ****************************************************************/

int main(void)
{
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    __disable_irq(); //��ֹ�����ж�
	Delay_Init();
	USART_Printf_Init(115200);
    LED_Init();
    Keyboard_Init();//��ť�����ʼ��
    //�粻ʹ����Ƶģ�飬���Թرձ�������
    ES8388_close();
    __enable_irq(); //���������ж�
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("��ʼ�����\r\n");
    printf("�밴�¾�����̶�Ӧ�İ���\r\n");
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
        // �������״̬
        gkeyboard_status.key_pressed = 0;
        gkeyboard_status.key_released = 0;
        gkeyboard_status.key_long_pressed = 0;
        // ��ʱ100ms�������Ӧ��
        Delay_Ms(100);
    }
}

