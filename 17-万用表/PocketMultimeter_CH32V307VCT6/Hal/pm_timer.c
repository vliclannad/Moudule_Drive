/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_timer.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
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

    TIM_TimeBaseInitStructure.TIM_Period = arr;                     //自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                  //预分频系数, 计数器的时钟频率=TIMxCLK/(psc+1), TIMxCLK=APB1*2=96MHz
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //该应用中未使用到,输入捕获时滤波用
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);             //定时器2初始化

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);                       //初始化定时器时计数器更新标志会置位，需求清除否则定时器使能后立刻进中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                  //定时器2中断使能

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //TIM2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //抢占优先级2级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          //响应优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道被使能
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
    TIM_SetCounter(TIM2, 0); //清计数器值
    TimerOverflowNum = 0;    //清溢出计数值
}


/******************************** endfile @ pm_timer ******************************/
