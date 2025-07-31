/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_timer.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
#include "pm_timer.h"


/**************************************************************************************************
 * @fn      PM_TimerInit
 *
 * @brief   Initializes timer2 to count.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *
 * @return  none
 **************************************************************************************************/
void PM_TimerInit(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitStructure.TIM_Period = arr;                     //�Զ���װ��ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                  //Ԥ��Ƶϵ��, ��������ʱ��Ƶ��=TIMxCLK/(psc+1), TIMxCLK=APB1*2=96MHz
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //��Ӧ����δʹ�õ�,���벶��ʱ�˲���
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);             //��ʱ��2��ʼ��

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);                       //��ʼ����ʱ��ʱ���������±�־����λ�������������ʱ��ʹ�ܺ����̽��ж�
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                  //��ʱ��2�ж�ʹ��

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //TIM2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          //��Ӧ���ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);
}


/**************************************************************************************************
 * @fn      PM_TimerCountTimeNS
 *
 * @brief   Timer counting  time in nanoseconds
 *
 * @param   count - Timer count value
 *          overflow - Timer overflow times
 *
 * @return  time(ns)
 **************************************************************************************************/
u32 PM_TimerCountTimeNS(u16 count, u32 overflow)
{
    u32 time;

    time = ((u32) count + 1) * 1000 / TIMER_COUNT_CLK + overflow * TIMER_COUNT_PERIOD_US * 1000; //ns

    return time;
}


/**************************************************************************************************
 * @fn      PM_TimerCountClear
 *
 * @brief   Clear timer count
 *
 * @param   none
 *
 * @return  none
 **************************************************************************************************/
void PM_TimerCountClear(void)
{
    TIM_SetCounter(TIM2, 0); //�������ֵ
    TimerOverflowNum = 0;    //���������ֵ
}


/******************************** endfile @ pm_timer ******************************/
