/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_probe.h
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*********************************************************************************/
#ifndef __PM_GPIO_H
#define __PM_GPIO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ch32v30x.h"
#include "pm_measure.h"

/*********************************************************************************
 *                                   ENUM
 *********************************************************************************/
/* 端口模式选择 */
typedef enum
{
    PORT_OUTPUT_WITH_NONE = 0,      //未连电阻的引脚输出，其余模拟输入
    PORT_OUTPUT_WITH_680,           //连接680Ω电阻引脚输出，其余模拟输入
    PORT_OUTPUT_WITH_470K,          //连接470KΩ电阻引脚输出，其余模拟输入
    PORT_ANALOG_INPUT_ALL,          //全部引脚模拟输入
}PM_PortMode;

/* 引脚输出电平 */
typedef enum
{
    LOW_LV = 0,     //输出低电平
    HIGH_LV         //输出高电平
}PM_PortLevel;


/*********************************************************************************
 *                                 FUNCTION
 *********************************************************************************/
void PM_GPIOInit(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct);
void PM_GPIOModeCfg(PM_PortDef port, GPIOMode_TypeDef mode);
void PM_MeasurePortInit(PM_MeasurePortGroup p_group, PM_PortMode mode, PM_PortLevel level);


/*********************************************************************************
**********************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
