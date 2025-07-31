/********************************** (C) COPYRIGHT *******************************
* File Name          : ch32v30x_it.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main Interrupt Service Routines.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "ch32v30x_it.h"

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      EXTI4_IRQHandler
 *
 * @brief   This function handles EXTI4 Handler.
 *
 * @return  none
 */
void EXTI4_IRQHandler(void)
{
    if(PUSH_BUTTON1())
    {
        TIM_Cmd(TIM2, DISABLE);                     //计数器失能
        TIM_SetCounter(TIM2, 0);                    //清计数器值
        TimerOverflowNum = 0;                       //清溢出计数值
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除TIM2的中断标志

        MeasureStartFlg = 1;
    }
    EXTI_ClearITPendingBit(EXTI_Line4);             //清除外部中断4的中断标志
}

/*********************************************************************
 * @fn      TIM2_IRQHandler
 *
 * @brief   This function handles TIM2 Handler.
 *
 * @return  none
 */
void TIM2_IRQHandler(void)
{
    if (TIM_GetFlagStatus(TIM2, TIM_IT_Update) != RESET) //判断TIM2是否产生更新中断
    {
        ++TimerOverflowNum;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除TIM2的中断标志
    }
}
