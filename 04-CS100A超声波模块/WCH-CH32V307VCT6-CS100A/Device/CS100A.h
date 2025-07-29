#ifndef __CS100A_H
#define __CS100A_H
#include "debug.h"
#include "user.h"


#define CS100A_GPIO_PORT        GPIOD
#define CS100A_GPIO_CLK         RCC_APB2Periph_GPIOD

#define CS100A_TRIG_PIN         GPIO_Pin_5
#define CS100A_ECHO_PIN         GPIO_Pin_6
#define CS100A_PORTSOURCE       GPIO_PortSourceGPIOD
#define CS100A_PINSOURCE        GPIO_PinSource6
#define CS100A_EXTILINE         EXTI_Line6
#define CS100A_EXTI_IRQ         EXTI9_5_IRQn
#define CS100A_EXTI_IRQHandler  EXTI9_5_IRQHandler

#define CS100A_TIM_CLK          RCC_APB1Periph_TIM3
#define CS100A_TIM              TIM3
#define CS100A_TIM_IRQ          TIM3_IRQn
#define CS100A_TIM_IRQHandler   TIM3_IRQHandler

void CS100A_Init(void);
void CS100A_GET(void);//获取距离存到全局变量中








#endif
