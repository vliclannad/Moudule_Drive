#ifndef __INFRARED_H
#define __INFRARED_H
#include "debug.h"

#define INFRARED_GPIO_PORT        GPIOC
#define INFRARED_GPIO_CLK         RCC_APB2Periph_GPIOC

#define INFRARED_GPIO_PIN         GPIO_Pin_3


void Infrared_Init(void);
uint8_t Infrared_GetData(void);

#endif
