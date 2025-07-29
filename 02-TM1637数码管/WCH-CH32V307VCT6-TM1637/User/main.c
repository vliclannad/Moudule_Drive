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
#include "tm1637.h"
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
 �������ʾ1-9999������
 ****************************************************************/
uint8_t gTM1637number[5]={0};
void TM1637_Test(void);

int main(void)
{
  SystemCoreClockUpdate();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  __disable_irq(); //��ֹ�ж�
	Delay_Init();
	USART_Printf_Init(115200);
	LED_Init();
	TM1637_Init();//����ܳ�ʼ��
  //�����ʹ����Ƶģ�飬��Ҫ�ر�
  ES8388_close();
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
  printf("��ʼ�����\r\n");
  printf("TM1637����ܲ���\r\n");
	while(1)
    {
	    Delay_Ms(500);
	    TM1637_Test();
    }
}
//����ܲ��Գ���ÿ��1s��λ����1
void TM1637_Test(void)
{
    uint8_t i;
    Delay_Ms(1000);
    TM1637_Display(gTM1637number[3],0,gTM1637number[2],0,gTM1637number[1],0,gTM1637number[0],0);
    gTM1637number[0]++;
    for(i=0;i<3;i++)
    {
      if(gTM1637number[i] > 9)
      {
        gTM1637number[i] = 0;
        gTM1637number[i+1]++;
      }
    }
}
