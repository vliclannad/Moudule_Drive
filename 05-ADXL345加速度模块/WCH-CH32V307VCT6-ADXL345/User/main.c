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
 ���Թ���
 ���ٶȴ�����ADLX345��ȡ���ٶ�ֵͨ������1���




 ****************************************************************/
void ADXL345_TEST(void);

int main(void)
{
    SystemCoreClockUpdate();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    __disable_irq(); //��ֹ�����ж�
	Delay_Init();
	USART_Printf_Init(115200);
	LED_Init();
	ADLX345_Init();//���ٶȴ�������ʼ��
    //�粻ʹ����Ƶģ�飬���Թرձ�������
    ES8388_close();    
    __enable_irq(); //���������ж�
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() ); 
    printf("��ʼ�����\r\n");
    printf("ADXL345���ٶȴ��������Գ���\r\n");
	while(1)
    {

	    Delay_Ms(1000);
	    ADXL345_TEST();
    }
}

//���ٶȴ��������Գ��򣬻�ȡx��y,z����������ٶ���ʾ��LCD����
void ADXL345_TEST(void)
{
    int16_t aaX =0,aaY = 0,aaZ = 0;
    uint8_t xyzData[6] = {0};         //x��y��z����ٶȣ���ռ�����ֽ�
    ADLX345_GetData(xyzData,6);
    Delay_Ms(1);

    aaX = (int16_t)((xyzData[1]<<8)+xyzData[0]);    //x������ٶ�
    aaY = (int16_t)((xyzData[3]<<8)+xyzData[2]);    //y������ٶ�
    aaZ = (int16_t)((xyzData[5]<<8)+xyzData[4]);    //z������ٶ�
    printf("x����ٶ�=%d,y����ٶ�=%d,z����ٶ�=%d\r\n",aaX,aaY,aaZ);
}
