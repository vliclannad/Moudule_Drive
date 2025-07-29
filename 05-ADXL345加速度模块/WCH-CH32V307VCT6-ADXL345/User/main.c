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
#include "adxl345.h"
#include "user.h"
#include "ch32v30x_it.h"
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
 测试功能
 加速度传感器ADLX345获取加速度值通过串口1输出




 ****************************************************************/
void ADXL345_TEST(void);

int main(void)
{
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    __disable_irq(); //禁止所有中断
	Delay_Init();
	USART_Printf_Init(115200);
	LED_Init();
	ADLX345_Init();//加速度传感器初始化
    //如不使用音频模块，可以关闭避免杂音
    ES8388_close();    
    __enable_irq(); //开启所有中断
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() ); 
    printf("初始化完成\r\n");
    printf("ADXL345加速度传感器测试程序\r\n");
	while(1)
    {

	    Delay_Ms(1000);
	    ADXL345_TEST();
    }
}

//加速度传感器测试程序，获取x，y,z三个方向加速度显示在LCD屏上
void ADXL345_TEST(void)
{
    int16_t aaX =0,aaY = 0,aaZ = 0;
    uint8_t xyzData[6] = {0};         //x、y、z轴加速度，均占两个字节
    ADLX345_GetData(xyzData,6);
    Delay_Ms(1);

    aaX = (int16_t)((xyzData[1]<<8)+xyzData[0]);    //x方向加速度
    aaY = (int16_t)((xyzData[3]<<8)+xyzData[2]);    //y方向加速度
    aaZ = (int16_t)((xyzData[5]<<8)+xyzData[4]);    //z方向加速度
    printf("x轴加速度=%d,y轴加速度=%d,z轴加速度=%d\r\n",aaX,aaY,aaZ);
}
