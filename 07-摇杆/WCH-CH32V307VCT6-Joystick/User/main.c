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
#include "Joystick.h"
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
��ȡҡ�˵ķ���ֵ��ͨ���������




 ****************************************************************/
void JOYSTICK_TEST(void);

int main(void)
{
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    __disable_irq(); //��ֹ�����ж�
	Delay_Init();
	USART_Printf_Init(115200);
	Delay_Ms(10);
	LED_Init();
	Joystick_Init();
    //�粻ʹ����Ƶģ�飬���Թرձ�������
    ES8388_close();
    __enable_irq(); //���������ж�
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("��ʼ�����\r\n");
    printf("ҡ�˲��Կ�ʼ\r\n");
    LED_GREEN_ON();
	while(1)
    {
	    JOYSTICK_TEST();
    }
}
void JOYSTICK_TEST(void)
{
    printf("x����ֵΪ��%d;",gJoystickValue[0]);
    printf("y����ֵΪ��%d;",gJoystickValue[1]);
    if(GPIO_ReadInputDataBit(JOYSTICK_RS_PORT, JOYSTICK_RS_PIN) == RESET)
    {
        printf("ҡ�˰���\r\n");
    }
    Delay_Ms(100);
}

