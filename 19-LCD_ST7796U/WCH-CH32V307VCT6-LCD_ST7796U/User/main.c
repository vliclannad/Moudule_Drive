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

#include "debug.h"
#include "lcd.h"
#include "touch-XPT2046.h"








/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
uint16_t RGB(uint8_t r,uint8_t g,uint8_t b)
{
	return r<<11|g<<6|b;
}

void TouchTest(void);

int main(void)
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(115200);	
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
	printf("This is printf example\r\n");
	Delay_Ms(3000);
	LCD_Init();
	//Touch_Init();
	LCD_Clear(LCD_WHITE);
	LCD_BadApple(2,2,80,80,bitmap_bytes,4);
	while(1)
    {
	}

}


/****************************
´¥ÃþÆÁ»­»­²âÊÔ
×ø±ê¼«ÏÞÖµ£ºx:213-3925    y:176-3883
Ð±ÂÊ¼ÆËã
x×ø±ê£º3925/320=12.2656
y×ø±ê£º3883/480=8.0896
*****************************/
void TouchTest(void)
{
	uint16_t touchXdata,touchYdata;
	uint16_t Xdata,Ydata;

	    if(GPIO_ReadInputDataBit(TOUCH_IRQ_PORT,TOUCH_IRQ_PIN)!=1)
	  {
		Touch_ReadData_XY(&touchXdata,&touchYdata);
		Xdata = (touchXdata-213)/12.2656;
		Ydata = (touchYdata-176)/8.0896;
		//LCD_DrawPixel(Xdata,Ydata,LCD_BLACK);
		LCD_FillRectangle(Xdata,Ydata,2,2,LCD_BLACK);

	  }
}
//ÉãÏñÍ·ÅÄÉã²âÊÔ
void CameraTest(void)
{

}


