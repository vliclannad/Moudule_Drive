/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_ch9141.h
* Author             : YHR
* Version            : V1.0
* Date               : 2023/06/06
* Description        : 
********************************************************************************/

/*******************************************************************************/
#ifndef __PM_CH9141_H
#define __PM_CH9141_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ch32v30x.h"
	
/********************************************************************************
 *                                     MACROS
 *******************************************************************************/
#define BLE_AT_PCENR            (RCC_APB2Periph_GPIOA)
#define BLE_SLEEP_PCENR         (RCC_APB2Periph_GPIOC)

#define BLE_AT_GPIO             (GPIOA)
#define BLE_SLEEP_GPIO          (GPIOC)

#define BLE_AT_BV               (GPIO_Pin_7)
#define BLE_SLEEP_BV            (GPIO_Pin_13)

#define BLE_AT_ENABLE()         (GPIO_ResetBits(BLE_AT_GPIO, BLE_AT_BV))
#define BLE_AT_DISABLE()        (GPIO_SetBits(BLE_AT_GPIO, BLE_AT_BV))
#define BLE_SLEEP_ENABLE()      (GPIO_ResetBits(BLE_SLEEP_GPIO, BLE_SLEEP_BV))
#define BLE_SLEEP_DISABLE()     (GPIO_SetBits(BLE_SLEEP_GPIO, BLE_SLEEP_BV))


/********************************************************************************
 *                                     MACROS
 *******************************************************************************/
/* AT指令 */
typedef enum
{
    DEVICE_AT_MODE = 0,             //进入AT模式
    DEVICE_NAME_READ,               //设备名字读
    DEVICE_NAME_WRITE               //设备名字写
}CH9141_AT_COMMAND;


/*******************************************************************************
 *                                     FUNCTIONS
 *******************************************************************************/
void PM_CH9141Init(void);
void PM_UARTSendString(u8 *buf, u16 len);
u8 PM_UARTReceiveString(u8 *buf);

/********************************************************************************
********************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
