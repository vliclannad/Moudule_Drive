#ifndef MOTOR_H
#define MOTOR_H

#include "debug.h"
// Motor control functions and definitions go here
#define MOTOR_GPIO_CLK RCC_APB2Periph_GPIOB
#define MOTOR_GPIO_PORT GPIOB
#define MOTOR_GPIO_PIN GPIO_Pin_4
#define MOTOR_TIM_CLK RCC_APB1Periph_TIM3
#define MOTOR_TIM TIM3

//油门渐变延时(ms)
#define MOTOR_THROTTLE_STEP_DELAY 10

void Motor_Init(uint16_t arr, uint16_t psc, uint16_t pulse);
void Motor_Start(void);
void Motor_Stop(void);
void Motor_StopGradual(void);
void Motor_SetThrottle(uint8_t throttle_value);
void Motor_SetThrottleGradual(uint8_t target_throttle, uint16_t step_delay_ms);
void Motor_ThrottleStepUp(uint8_t step_value);
void Motor_ThrottleStepDown(uint8_t step_value);
uint8_t Motor_GetCurrentThrottle(void);

#endif
