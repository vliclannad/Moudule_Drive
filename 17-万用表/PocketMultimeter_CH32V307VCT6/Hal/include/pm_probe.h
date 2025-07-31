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
/* �˿�ģʽѡ�� */
typedef enum
{
    PORT_OUTPUT_WITH_NONE = 0,      //δ��������������������ģ������
    PORT_OUTPUT_WITH_680,           //����680�������������������ģ������
    PORT_OUTPUT_WITH_470K,          //����470K�������������������ģ������
    PORT_ANALOG_INPUT_ALL,          //ȫ������ģ������
}PM_PortMode;

/* ���������ƽ */
typedef enum
{
    LOW_LV = 0,     //����͵�ƽ
    HIGH_LV         //����ߵ�ƽ
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
