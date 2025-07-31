/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_timer.h
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
********************************************************************************/

/*******************************************************************************/
#ifndef __PM_TIMER_H
#define __PM_TIMER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ch32v30x.h"

/*********************************************************************************
 *                                  MACRO
 *********************************************************************************/
/* Timer configuration */
#define TIMER_CLK                   ((u16)96)                                   //��ʱ��ʱ��96(M)
#define TIMER_PRESCALER             ((u16)0)                                    //��ʱ��ʱ�ӷ�Ƶϵ��
#define TIMER_COUNT_CLK             ((u16)(TIMER_CLK/(TIMER_PRESCALER+1)))      //����Ƶ��96(M)
#define TIMER_COUNT_PERIOD          ((u16)65471)                                //�Զ���װ��ֵ,Ϊ�˶�ʱ�����ʱ��Ϊ����
#define TIMER_COUNT_PERIOD_US       ((u16)((TIMER_COUNT_PERIOD+1)/TIMER_COUNT_CLK)) //��ʱ�����ʹ�õ�ʱ��682(us)


/*******************************************************************************
 *                                     FUNCTIONS
 *******************************************************************************/
void PM_TimerInit(u16 arr, u16 psc);
u32 PM_TimerCountTimeNS(u16 count, u32 overflow);
void PM_TimerCountClear(void);


/********************************************************************************
********************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
