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
//����
//���ù���ť���������������ת


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
    // ��ʼ������Ͱ���
    __disable_irq();
    Delay_Init();
	USART_Printf_Init(115200);
	LED_Init();
    Motor_Init(20000, 72, 900);//�̶�Ƶ��50Hz
    Keyboard_Init();
    ES8388_close(); // �ر�ES8388��ƵоƬ
    __enable_irq();
    printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("��ʼ�����\r\n");
    LED_GREEN_ON();
    while(1)
    {
        // ��Ҫ�Ƚ������
        if (gkeyboard_status.key_pressed & (1<<KEY_11)) {
            Motor_Start();//�������
            printf("Key 11: �������\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_12)) {
            Motor_StopGradual();   // ����ֹͣ
            printf("Key 12: �������ֹͣ\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_13)) {
            Motor_Stop();
            printf("Key 13: �����ͣ\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_21)) {
            Motor_SetThrottleGradual(25, 20);  // ���䵽25%����
            printf("Key 21: ������� 25%%\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_22)) {
            Motor_SetThrottleGradual(50, 20); // ���䵽50%����
            printf("Key 22: ������� 50%%\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_23)) {
            Motor_SetThrottleGradual(100, 20); // ���䵽100%����
            printf("Key 23: ȫ���١�ǰ�����DA!\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_31)) {
            Motor_ThrottleStepUp(10); // ���Ų�������10%
            printf("Key 31: ���Ų�������10%%\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_32)) {
            Motor_ThrottleStepDown(10); // ���Ų�������10%
            printf("Key 32: ���Ų�������10%%\r\n");
        }
        else if (gkeyboard_status.key_pressed & (1<<KEY_33)) {
        }
        // �������״̬
        gkeyboard_status.key_pressed = 0;
        gkeyboard_status.key_released = 0;
        gkeyboard_status.key_long_pressed = 0;
        // ��ʱ100ms�������Ӧ��
        Delay_Ms(100);
    }

}
