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
#include "debug.h"
#include "led.h"
#include "beep.h"
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




 ****************************************************************/

int main(void)
{

    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_Init();
	USART_Printf_Init(115200);
    // ��ʼ��PWM������ - ʹ�ù̶�1MHzʱ��
    BEEP_PWM_Init_1MHz(); 
    LED_Init();
    ES8388_close();
    LED_GREEN_ON();
    
    printf("���������ֲ�����ʾ��ʼ...\r\n");
    printf("PWMʱ�ӹ̶�Ϊ1MHz\r\n");
    Delay_Ms(3000);
    BEEP_PlayBadApple();  // ����Bad Apple
        
        
    
    while (1)
    {       

    }
}
