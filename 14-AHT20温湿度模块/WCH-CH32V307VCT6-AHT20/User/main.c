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
#include "AHT20.h"
#include "led.h"
#include "ES8388.h"


/*功能*
通过串口1输出温湿度



/


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
 void AHT20_Test(void);
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
    __disable_irq(); //禁止总中断
	Delay_Init();
	USART_Printf_Init(115200);	
    AHT21_Init();
    LED_Init(); //LED初始化
    //如不使用音频模块，可以关闭避免杂音
    ES8388_close(); //关闭ES8388
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("初始化完成\r\n");
    printf("AHT20温湿度传感器测试\r\n");
    LED_GREEN_ON();
	while(1)
    {
        Delay_Ms(1000);
        AHT20_Test();
	}
}

void AHT20_Test(void)
{
    uint8_t state=0;
    float* TH_Value;
    state = AHT21_ReadState();//读取状态
    Delay_Ms(10);
    if((state&0x08) != 0x08)//不等于0000 0100，即为0
         {
             AHT21_WriteInit();//如果第三位是0，则初始化寄存器
             printf("初始化寄存器\r\n");
             Delay_Ms(10);

         }
        AHT21_WriteMeasure();//再直接发送测量命令
        Delay_Ms(500);
        TH_Value = AHT21_ReadTemperatureHumidity();//触发测量及读取温度

        printf("湿度=%d%%\r\n",(uint16_t)(TH_Value[0]*100));//将读取到的数据发送回电脑
        printf("温度=%d℃\r\n",(uint16_t)TH_Value[1]);//将读取到的数据发送回电脑
}

