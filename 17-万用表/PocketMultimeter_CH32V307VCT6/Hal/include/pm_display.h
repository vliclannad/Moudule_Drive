/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_display.h
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        : 
********************************************************************************/

/*******************************************************************************/
#ifndef __PM_DISPLAY_H
#define __PM_DISPLAY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ch32v30x.h"
	
/*********************************************************************************
 *                                    MACROS
 *********************************************************************************/
/* BLE通知信息 起始码和结束码*/
#define BLE_BUF_START                   0x07
#define BLE_BUF_END                     0xD7

/*器件名字*/
#define COMPONENT_TYPE_NONE             0x00
#define COMPONENT_TYPE_RES              0x01
#define COMPONENT_TYPE_CAP              0x02
#define COMPONENT_TYPE_DIO              0x03
#define COMPONENT_TYPE_BJT              0x04
#define COMPONENT_TYPE_JFET             0x05
#define COMPONENT_TYPE_EMOS             0x06
#define COMPONENT_TYPE_DMOS             0x07

/*BJT类型*/
#define COMPONENT_TYPE_BJT_NPN          1
#define COMPONENT_TYPE_BJT_PNP          2

/*JFET类型*/
#define COMPONENT_TYPE_JFET_P           1
#define COMPONENT_TYPE_JFET_N           2

/*MOSFET类型*/
#define COMPONENT_TYPE_MOSFET_P         1
#define COMPONENT_TYPE_MOSFET_N         2
#define COMPONENT_TYPE_MOSFET_P_DIO     3
#define COMPONENT_TYPE_MOSFET_N_DIO     4


/*******************************************************************************
 *                           GLOBAL VARIABLES
 *******************************************************************************/
extern const unsigned char RES_PIC[];
extern const unsigned char OHM_UNIT[];
extern const unsigned char CAP_PIC[];
extern const unsigned char IND_PIC[];
extern const unsigned char DIO_PIC[];
extern const unsigned char PBJT_PIC[];
extern const unsigned char NBJT_PIC[];
extern const unsigned char READY_PIC[];
extern const unsigned char ERROR_PIC[];
extern const unsigned char TESTING_PIC[];
extern const unsigned char WARNING_PIC[];


/*******************************************************************************
 *                                     FUNCTIONS
 *******************************************************************************/
void PM_DisplayCH9141Reset(void);
void PM_DisplayReady(void);
void PM_DisplayTesting(void);
void PM_DisplayError(void);
void PM_DisplayResistance(PM_ComponentMeasureResult *ComParam);
void PM_DisplayCapacitor(PM_ComponentMeasureResult *ComParam);
void PM_DisplayDiode(PM_ComponentMeasureResult *ComParam);
void PM_DisplayBJT(PM_ComponentMeasureResult *ComParam);


/********************************************************************************
********************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
