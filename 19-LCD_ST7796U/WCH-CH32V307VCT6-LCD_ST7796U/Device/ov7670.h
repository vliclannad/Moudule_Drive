#ifndef __OV7670_H
#define __OV7670_H

#include "debug.h"


#define CAM_VSYNC_RCC     RCC_APB2Periph_GPIOC  //帧传送结束
#define CAM_VSYNC_PORT    GPIOE
#define CAM_VSYNC_PIN     GPIO_Pin_12

#define CAM_OE_RCC        RCC_APB2Periph_GPIOA  //输出使能
#define CAM_OE_PORT       GPIOE
#define CAM_OE_PIN        GPIO_Pin_8

#define CAM_WRST_RCC      RCC_APB2Periph_GPIOC  //写复位
#define CAM_WRST_PORT     GPIOC
#define CAM_WRST_PIN      GPIO_Pin_9

#define CAM_RRST_RCC      RCC_APB2Periph_GPIOC //读复位
#define CAM_RRST_PORT     GPIOC
#define CAM_RRST_PIN      GPIO_Pin_8

//SCCB
#define CAM_CLK_RCC       RCC_APB2Periph_GPIOE
#define CAM_CLK_PORT      GPIOE
#define CAM_CLK_PIN       (GPIO_Pin_13)

#define CAM_SDA_RCC       RCC_APB2Periph_GPIOE
#define CAM_SDA_PORT      GPIOE
#define CAM_SDA_PIN       (GPIO_Pin_14)

#define CAM_DATA_RCC     RCC_APB2Periph_GPIOD
#define CAM_DATA_PORT    GPIOD
#define CAM_DATA0_PIN     (GPIO_Pin_0)
#define CAM_DATA1_PIN     (GPIO_Pin_1)
#define CAM_DATA2_PIN     (GPIO_Pin_2)
#define CAM_DATA3_PIN     (GPIO_Pin_3)
#define CAM_DATA4_PIN     (GPIO_Pin_4)
#define CAM_DATA5_PIN     (GPIO_Pin_5)
#define CAM_DATA6_PIN     (GPIO_Pin_6)
#define CAM_DATA7_PIN     (GPIO_Pin_7)


#define CAM_WRST_SET    (CAM_WRST_PORT->BSHR = CAM_WRST_PIN)
#define CAM_WRST_CLR    (CAM_WRST_PORT->BCR = CAM_WRST_PIN)
#define CAM_RRST_SET    (CAM_RRST_PORT->BSHR = CAM_RRST_PIN)
#define CAM_RRST_CLR    (CAM_RRST_PORT->BCR = CAM_RRST_PIN)

#define CAM_CLK_SET    (CAM_CLK_PORT->BSHR = CAM_CLK_PIN)
#define CAM_CLK_CLR    (CAM_CLK_PORT->BCR = CAM_CLK_PIN)
#define CAM_SDA_SET    (CAM_SDA_PORT->BSHR = CAM_SDA_PIN)
#define CAM_SDA_CLR    (CAM_SDA_PORT->BCR = CAM_SDA_PIN)

#define CAM_OE_SET     (CAM_OE_PORT->BSHR = CAM_OE_PIN)//禁止读取
#define CAM_OE_CLR     (CAM_OE_PORT->BCR = CAM_OE_PIN)//允许读取





#endif