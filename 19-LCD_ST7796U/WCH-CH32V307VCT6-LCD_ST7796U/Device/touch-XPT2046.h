#ifndef __TOUCH_H
#define __TOUCH_H
 
#include "debug.h"
#include "spi.h"

#define TOUCH_CS_RCC       RCC_APB2Periph_GPIOB
#define TOUCH_CS_PORT      GPIOB
#define TOUCH_CS_PIN       (GPIO_Pin_12)

#define TOUCH_IRQ_RCC      RCC_APB2Periph_GPIOB
#define TOUCH_IRQ_PORT     GPIOB
#define TOUCH_IRQ_PIN      (GPIO_Pin_10)

#define TOUCH_MOSI_RCC     RCC_APB2Periph_GPIOB
#define TOUCH_MOSI_PORT    GPIOB
#define TOUCH_MOSI_PIN     (GPIO_Pin_15)

#define TOUCH_MISO_RCC     RCC_APB2Periph_GPIOB
#define TOUCH_MISO_PORT    GPIOB
#define TOUCH_MISO_PIN     (GPIO_Pin_14)

#define TOUCH_CLK_RCC      RCC_APB2Periph_GPIOB
#define TOUCH_CLK_PORT     GPIOB
#define TOUCH_CLK_PIN      (GPIO_Pin_13)


#define TOUCH_CS_SET   (TOUCH_CS_PORT->BSHR = TOUCH_CS_PIN)
#define TOUCH_CS_CLR   (TOUCH_CS_PORT->BCR = TOUCH_CS_PIN)
#define TOUCH_CLK_SET  (TOUCH_CLK_PORT->BSHR = TOUCH_CLK_PIN)
#define TOUCH_CLK_CLR  (TOUCH_CLK_PORT->BCR = TOUCH_CLK_PIN)
#define TOUCH_MOSI_SET (TOUCH_MOSI_PORT->BSHR = TOUCH_MOSI_PIN)
#define TOUCH_MOSI_CLR (TOUCH_MOSI_PORT->BCR = TOUCH_MOSI_PIN)


void Touch_Init();
uint16_t Touch_ReadData(uint8_t cmd);
void Touch_ReadData_XY(uint16_t *x,uint16_t *y);







#endif