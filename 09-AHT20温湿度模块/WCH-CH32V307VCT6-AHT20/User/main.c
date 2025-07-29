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


/*����*
ͨ������1�����ʪ��



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
    __disable_irq(); //��ֹ���ж�
	Delay_Init();
	USART_Printf_Init(115200);	
    AHT21_Init();
    LED_Init(); //LED��ʼ��
    //�粻ʹ����Ƶģ�飬���Թرձ�������
    ES8388_close(); //�ر�ES8388
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("��ʼ�����\r\n");
    printf("AHT20��ʪ�ȴ���������\r\n");
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
    state = AHT21_ReadState();//��ȡ״̬
    Delay_Ms(10);
    if((state&0x08) != 0x08)//������0000 0100����Ϊ0
         {
             AHT21_WriteInit();//�������λ��0�����ʼ���Ĵ���
             printf("��ʼ���Ĵ���\r\n");
             Delay_Ms(10);

         }
        AHT21_WriteMeasure();//��ֱ�ӷ��Ͳ�������
        Delay_Ms(500);
        TH_Value = AHT21_ReadTemperatureHumidity();//������������ȡ�¶�

        printf("ʪ��=%d%%\r\n",(uint16_t)(TH_Value[0]*100));//����ȡ�������ݷ��ͻص���
        printf("�¶�=%d��\r\n",(uint16_t)TH_Value[1]);//����ȡ�������ݷ��ͻص���
}

