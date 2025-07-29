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
#include "AP3216C.h"
#include "ES8388.h"
#include "led.h"
#include "user_usart.h"
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
��ȡ����ֵ��ͨ�����ڴ�ӡ����



 ****************************************************************/

int main(void)
{
    uint16_t als;
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    __disable_irq(); //��ֹ�����ж�
	Delay_Init();
	USART_Printf_Init(115200);
	LED_Init();
	AP3216C_Init();
    //�粻ʹ����Ƶģ�飬���Թرձ�������
    ES8388_close();
    __enable_irq(); //���������ж�
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("��ʼ�����\r\n");
    printf("AP3216C���������Կ�ʼ\r\n");
    LED_GREEN_ON();
	while(1)
    {

	    Delay_Ms(500);
	    AP3216C_ReadData(&als);
        printf("����ǿ��:als = %d\r\n",als);
    }
}
