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
#include "led.h"
#include "ES8388.h"


/*********************************************************************
 * @fn      show_sdcard_info
 *
 * @brief   SD Card information.
 *
 * @return  none
 */
void show_sdcard_info(void)
{
    switch(SDCardInfo.CardType)
    {
        case SDIO_STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
        case SDIO_STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
        case SDIO_HIGH_CAPACITY_SD_CARD:printf("Card Type:SDHC V2.0\r\n");break;
        case SDIO_MULTIMEDIA_CARD:printf("Card Type:MMC Card\r\n");break;
    }
    printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);
    printf("Card RCA:%d\r\n",SDCardInfo.RCA);
    printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));
    printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);
}


u8 buf[512];
u8 Readbuf[512];

void Switch_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */

 /*功能
 写入SD卡0扇区，读取并验证
 */
int main(void)
{
    u32 i;
    u32 Sector_Nums;
    uint8_t writetest=1;
    uint8_t readtest = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
    __disable_irq(); // 禁止中断
	Delay_Init();
	USART_Printf_Init(115200);	
    LED_Init();
    ES8388_close();// 关闭ES8388音频芯片
    Switch_Init(); // 初始化开关
    __enable_irq(); // 允许中断
	GPIO_SetBits(GPIOD, GPIO_Pin_7); //切换到SD卡
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    while(SD_Init())
    {
        printf("SD Card Error!\r\n");
        delay_ms(1000);
    }
    show_sdcard_info();
    printf("SD Card OK\r\n");
    for(i=0; i<512; i++){
        buf[i] = i;
    }

    // 测试扇区0
    u32 test_sector = 0;
    printf("Testing sector %d...\n", test_sector); 
    u8 write_result = SD_WriteDisk(buf, test_sector, 1);
    if(write_result) {
        printf("Wr %d sector fail, error code: %d\n", test_sector, write_result);
    } else {
        printf("Wr %d sector success\n", test_sector);
    }
    
    u8 read_result = SD_ReadDisk(Readbuf, test_sector, 1);
    if(read_result) {
        printf("Rd %d sector fail, error code: %d\n", test_sector, read_result);
    } else {
        printf("Rd %d sector success\n", test_sector);
    }

    if(write_result == 0 && read_result == 0) {
        if(memcmp(buf, Readbuf, 512) == 0) {
            printf("Sector %d verify SUCCESS!\n", test_sector);
        } else {
            printf("Sector %d verify FAIL - data mismatch\n", test_sector);
            // 打印前几个字节进行对比
            printf("Written: ");
            for(i=0; i<8; i++) printf("%02X ", buf[i]);
            printf("\nRead:    ");
            for(i=0; i<8; i++) printf("%02X ", Readbuf[i]);
            printf("\n");
        }
    }
    printf("end\n");
    while(1)
    {
    }
}







