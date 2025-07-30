/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
   SDIO routine to operate SD card:
      This example demonstrates reading and writing all sectors of the SD card
      through the SDIO interface.
  DVP--PIN:
    D0--PC8
    D1--PC9
    D2--PC10
    D3--PC11
    SCK--PC12
    CMD--PD2
     Note: Except for SCK, the rest need to pull up 47K resistors

*/

#include "debug.h"
#include "sdio.h"
#include "string.h"
#include "diskio.h"
#include "ff.h"
#include "ffconf.h"
#include "ES8388.h"
#include "led.h"
#include "wavplay.h"

/*********************************************************************
 * @fn      show_sdcard_info
 *
 * @brief   SD Card information.
 *
 * @return  none
 */

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */

 /*����
 ��ȡSD���е�16bit,44.1Khz��WAV��Ƶ�ļ�����ͨ������������
 
 
 
 */
FATFS fs;                         /* FatFs�ļ�ϵͳ���� */
FIL fnew;                         /* �ļ����� */
UINT fnum;                        /* �ļ��ɹ���д����*/
FRESULT res_sd;                   /* �ļ�������� */
BYTE    work[FF_MAX_SS];
BYTE    WriteBuffer[] = {"TEST\r\n"};   /* д������ */

int main(void)
{
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(115200);	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init(); //LED��ʼ��
    Delay_Ms(3000);

    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf("CH32V307VCT6 + ES8388\r\n");
	ES8388_Init();//ES8388��ʼ��
    GPIO_WriteBit(GPIOC,GPIO_Pin_13,0);//�л���ES8388�����
    GPIO_WriteBit(GPIOD,GPIO_Pin_7,1);//�л���SD��
    printf("ES8388 Init OK\r\n");
    ES8388_I2S_Cfg(0,3); //����I2S��16bit
    ES8388_ADDA_Cfg(1,0);   //����DAC
    ES8388_Output_Cfg(1, 0);    /* DACѡ��ͨ����� */
    ES8388_hpvol_set(25);        /* ���ö������� */
    //ES8388_spkvol_set(15);       /* ������������ */


    //���ζ�ȡ�Ĵ�������ӡ
    //for(int i=0;i<0x33;i++)
   // {
   //     printf("ES8388 Reg[%02X]=%02X\r\n",i,ES8388_Read_Reg(i));
    //    delay_ms(30);
   // }
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("��ʼ������Ƶ����\r\n");
	printf("��ʼ�����ļ�ϵͳ\r\n");
	res_sd = f_mount(&fs, "1:", 1);
	printf("�ļ�ϵͳ���ؽ��%d\r\n",res_sd);
	if(res_sd == FR_NO_FILESYSTEM)
	{
	    printf("formatting\r\n");
	    res_sd = f_mkfs("1:", 0, work,sizeof(work));
	    res_sd = f_mount(NULL, "1:",1);
	    //���¹���
	    res_sd = f_mount(&fs, "1:", 1);
	}
	wav_play_song("1:OneLastKiss.wav");//���Ų�����Ƶ*/
	while(1)
	{
	    Delay_Ms(1000);
	}
}





