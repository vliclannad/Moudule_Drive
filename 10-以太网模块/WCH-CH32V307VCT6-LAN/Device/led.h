#ifndef __LED_H
#define __LED_H

#include "Debug.h"

#define LED_GPIO_CLK RCC_APB2Periph_GPIOC
#define LED_GPIO_PORT GPIOC
#define LED_RED_PIN GPIO_Pin_0
#define LED_GREEN_PIN GPIO_Pin_1
#define LED_BLUE_PIN GPIO_Pin_2

#define LED_RED_ON() GPIO_ResetBits(LED_GPIO_PORT, LED_RED_PIN)
#define LED_RED_OFF() GPIO_SetBits(LED_GPIO_PORT, LED_RED_PIN)
#define LED_GREEN_ON() GPIO_ResetBits(LED_GPIO_PORT, LED_GREEN_PIN)
#define LED_GREEN_OFF() GPIO_SetBits(LED_GPIO_PORT, LED_GREEN_PIN)
#define LED_BLUE_ON() GPIO_ResetBits(LED_GPIO_PORT, LED_BLUE_PIN)
#define LED_BLUE_OFF() GPIO_SetBits(LED_GPIO_PORT, LED_BLUE_PIN)
#define LED_TOGGLE(pin) GPIO_WriteBit(LED_GPIO_PORT, pin, (BitAction)(1-GPIO_ReadOutputDataBit(LED_GPIO_PORT, pin)));

void LED_Init(void);

#endif
