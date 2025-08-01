/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_resistance.h
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
********************************************************************************/

/*******************************************************************************/
#ifndef __PM_RESISTANCE_H
#define __PM_RESISTANCE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ch32v30x.h"

/*********************************************************************************
 *                                  MACRO
 *********************************************************************************/
/* Voltage upper limit */
#define RES_UPPER_LIMIT_VOLTAGE              ((u32)3250)    //3250mV

/* Voltage lower limit */
#define RES_LOWER_LIMIT_VOLTAGE              ((u32)50)      //50mV


/*******************************************************************************
 *                                     FUNCTIONS
 *******************************************************************************/
u8 PM_CheckIsResistance(void);
u8 PM_MeasureResistance(void);


/********************************************************************************
********************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
