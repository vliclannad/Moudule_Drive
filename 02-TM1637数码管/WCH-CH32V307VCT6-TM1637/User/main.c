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
 测试功能
 数码管显示1-9999的数字
 ****************************************************************/
uint8_t gTM1637number[5]={0};
void TM1637_Test(void);

int main(void)
{
  SystemCoreClockUpdate();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  __disable_irq(); //禁止中断
	Delay_Init();
	USART_Printf_Init(115200);
	LED_Init();
	TM1637_Init();//数码管初始化
  //如果不使用音频模块，需要关闭
  ES8388_close();
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
  printf("初始化完成\r\n");
  printf("TM1637数码管测试\r\n");
	while(1)
    {
	    Delay_Ms(500);
	    TM1637_Test();
    }
}
//数码管测试程序，每隔1s个位数加1
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
