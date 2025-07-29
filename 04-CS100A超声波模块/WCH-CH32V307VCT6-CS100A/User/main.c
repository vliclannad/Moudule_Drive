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
 ���Թ���
 ��������������ȡ����ͨ������1���




 ****************************************************************/

void CS100A_TEST(void);
int main(void)
{

    gCS100ACount = 0;
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    __disable_irq(); //��ֹ�����ж�
	Delay_Init();
	USART_Printf_Init(115200);
    LED_Init(); //LED��ʼ��
	CS100A_Init();//��������������ʼ��
    //�粻ʹ����Ƶģ�飬���Թرձ�������
    ES8388_close();
    __enable_irq(); //���������ж�
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("��ʼ�����\r\n");
    printf("��ʼ���Գ�����ģ��\r\n");
	while(1)
    {
	    Delay_Ms(500);
	    CS100A_TEST();
    }
}
//���봫�������Գ���,��ȡ����ͨ���������
void CS100A_TEST(void)
{
    // 1. ����ʮ��
    CS100A_GET();
    Delay_Ms(200);
    printf("����Ϊ��%.1fcm\r\n",gCS100A_Distance);
}
