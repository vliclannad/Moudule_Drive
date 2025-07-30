#ifndef __IIC_H
#define __IIC_H

#include "debug.h"

#define USER_IIC              I2C2
#define USER_IIC_CLK          RCC_APB1Periph_I2C2

#define USER_IIC_GPIO_CLK     RCC_APB2Periph_GPIOB
#define USER_IIC_GPIO_PORT    GPIOB
#define USER_IIC_SCL_PIN      GPIO_Pin_10
#define USER_IIC_SDA_PIN      GPIO_Pin_11

#define USER_IIC_BOUND        100000        //200KHz
#define USER_IIC_ADDR         0x01



void IIC_Init();
u8 IIC_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
u8 IIC_ReadByte(u8 addr,u8 reg);
u8 IIC_WriteLen(u8 addr, u8 reg, u8 len, u8 *buf);
u8 IIC_ReadLen(u8 addr, u8 reg, u8 len, u8 *buf);
u8 IIC_WriteByte(u8 addr,uint8_t reg, uint8_t data);
#endif
