#ifndef __BEEP_H
#define __BEEP_H

#include "ch32v30x.h"

#define BEEP_GPIO_CLK RCC_APB2Periph_GPIOA
#define BEEP_GPIO_PORT GPIOA
#define BEEP_PIN GPIO_Pin_4

#define BEEP_ON() GPIO_SetBits(BEEP_GPIO_PORT, BEEP_PIN)
#define BEEP_OFF() GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_PIN)

void BEEP_Init(void);


#endif
