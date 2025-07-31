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
        TIM_Cmd(TIM2, DISABLE);                     //������ʧ��
        TIM_SetCounter(TIM2, 0);                    //�������ֵ
        TimerOverflowNum = 0;                       //���������ֵ
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //���TIM2���жϱ�־

        MeasureStartFlg = 1;
    }
    EXTI_ClearITPendingBit(EXTI_Line4);             //����ⲿ�ж�4���жϱ�־
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
    if (TIM_GetFlagStatus(TIM2, TIM_IT_Update) != RESET) //�ж�TIM2�Ƿ���������ж�
    {
        ++TimerOverflowNum;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //���TIM2���жϱ�־
    }
}
