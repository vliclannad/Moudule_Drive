/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_key.h
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        : 
********************************************************************************/

/*******************************************************************************/
#ifndef __PM_KEY_H
#define __PM_KEY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ch32v30x.h"
	
/********************************************************************************
 *                                     MACROS
 *******************************************************************************/
/* Switch/Key */
#ifdef BOARD_CHITU
#define SW1_PCENR               (RCC_APB2Periph_GPIOE)
#define SW2_PCENR               (RCC_APB2Periph_GPIOE)

#define SW1_GPIO                (GPIOE)
#define SW2_GPIO                (GPIOE)

#define SW1_BV                  (GPIO_Pin_4)
#define SW2_BV                  (GPIO_Pin_5)

#define SW1_IN                  (GPIO_ReadInputDataBit(SW1_GPIO, SW1_BV) == 0)
#define SW2_IN                  (GPIO_ReadInputDataBit(SW2_GPIO, SW2_BV) == 0)

#define PUSH_BUTTON1()          (SW1_IN)
#define PUSH_BUTTON2()          (SW2_IN)
#elif defined BOARD_XUELONG
#define SW1_PCENR               (RCC_APB2Periph_GPIOE)
#define SW2_PCENR               (RCC_APB2Periph_GPIOE)
#define SW3_PCENR               (RCC_APB2Periph_GPIOE)
#define SW4_PCENR               (RCC_APB2Periph_GPIOE)

#define SW1_GPIO                (GPIOE)
#define SW2_GPIO                (GPIOE)
#define SW3_GPIO                (GPIOE)
#define SW4_GPIO                (GPIOE)

#define SW1_BV                  (GPIO_Pin_4)
#define SW2_BV                  (GPIO_Pin_5)
#define SW3_BV                  (GPIO_Pin_6)
#define SW4_BV                  (GPIO_Pin_7)

#define SW1_IN                  (GPIO_ReadInputDataBit(SW1_GPIO, SW1_BV) == 0)
#define SW2_IN                  (GPIO_ReadInputDataBit(SW2_GPIO, SW2_BV) == 0)
#define SW3_IN                  (GPIO_ReadInputDataBit(SW3_GPIO, SW3_BV) == 0)
#define SW4_IN                  (GPIO_ReadInputDataBit(SW4_GPIO, SW4_BV) == 0)

#define PUSH_BUTTON1()          (SW1_IN)
#define PUSH_BUTTON2()          (SW2_IN)
#define PUSH_BUTTON3()          (SW3_IN)
#define PUSH_BUTTON4()          (SW4_IN)
#endif


/*******************************************************************************
 *                                     FUNCTIONS
 *******************************************************************************/
void PM_KeyInit(void);


/********************************************************************************
********************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
