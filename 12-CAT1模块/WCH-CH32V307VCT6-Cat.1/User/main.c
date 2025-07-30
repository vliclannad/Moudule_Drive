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
#include "CAT1.h"
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
CATͨ��TCP���Ӳ���������




 ****************************************************************/
int main(void)
{
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //__disable_irq(); //��ֹ���ж�
	Delay_Init();
	USART_Printf_Init(115200);
    LED_Init();
    ES8388_close();//�ر�ES8388
    //__enable_irq(); //�������ж�
    
	Delay_Ms(10);
	uint8_t* test = "hello";
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
	printf("This is printf example\r\n");
	CAT_Init(CAT_STATION);
    Delay_Ms(2000);
    printf("��ʼ����TCP������\r\n");
    CAT_TCPConnect((uint8_t*)"43.139.170.206",(uint8_t*)"24840");
    CAT_SetTxMode(1);//����͸��ģʽ
    printf("��ʼ�����\r\n");
	while(1)
    {
	    Delay_Ms(5000);
	    USER_UART_Transmit(CAT_USARTx, test, 5);
    }
}


