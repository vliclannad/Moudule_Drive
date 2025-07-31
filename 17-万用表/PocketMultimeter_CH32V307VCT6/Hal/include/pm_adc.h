/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_adc.h
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*********************************************************************************/
#ifndef __PM_ADC_H
#define __PM_ADC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ch32v30x.h"

/*********************************************************************************
 *                                  MACRO
 *********************************************************************************/
/* ADC CONT mask */
#define CTLR2_CONT_Set                  ((u32)0x00000002)
#define CTLR2_CONT_Reset                ((u32)0xFFFFFFFD)

/* DMA ADC buffer max length */
#define ADC_BUF_MAX_LEN                 ((u8)255)


/********************************************************************************
 *                                FUNCTIONS
 ********************************************************************************/
void PM_ADCInit(void);
void PM_ADCContinuousConvModeCmd(ADC_TypeDef *ADCx, FunctionalState NewState);
u16 PM_ADCCalibrattion(s16 val);
u16 PM_GetADC(u8 ch, uint8_t sample_time);
u16 PM_GetADCAverage(u8 ch, u8 times, uint8_t sample_time);
u16 PM_DMA_GetADCAverage(u8 ch, u8 times, uint8_t sample_time);
u16 PM_DMA_GetADCAverage_ForCAP(u8 ch, u8 sample_time, u16 *p_count, u32 *p_overflow);
u32 PM_ADCRealVol(u16 val);


/*********************************************************************************
**********************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
