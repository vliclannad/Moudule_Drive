#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include "debug.h"
#include "user.h"

#define JOYSTICK_X_PIN     GPIO_Pin_1
#define JOYSTICK_Y_PIN     GPIO_Pin_5
#define JOYSTICK_RS_PIN     GPIO_Pin_2

#define JOYSTICK_XY_PORT   GPIOA
#define JOYSTICK_RS_PORT   GPIOE

#define JOYSTICK_XY_CLK         RCC_APB2Periph_GPIOA
#define JOYSTICK_RS_CLK         RCC_APB2Periph_GPIOE

#define JOYSTICK_ADC            ADC1
#define JOYSTICK_ADC_CLK        RCC_APB2Periph_ADC1
#define JOYSTICK_ADC_XCHANNEL   ADC_Channel_1
#define JOYSTICK_ADC_YCHANNEL   ADC_Channel_5

#define JOYSTICK_DMA            DMA1
#define JOYSTICK_DMA_CHANNEL    DMA1_Channel1
#define JOYSTICK_DMA_CLK        RCC_AHBPeriph_DMA1

void Joystick_Init(void);

#endif
