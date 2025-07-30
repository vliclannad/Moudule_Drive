#ifndef __ES8388_H
#define __ES8388_H

#include "debug.h"

//音量范围
#define ES8388_VOL_MAX  33
#define ES8388_VOL_MIN  0

//IIC接口
#define ES8388_IIC              I2C1
#define ES8388_IIC_CLK          RCC_APB1Periph_I2C1

#define ES8388_IIC_GPIO_CLK     RCC_APB2Periph_GPIOB
#define ES8388_IIC_GPIO_PORT    GPIOB
#define ES8388_IIC_SCL_PIN      GPIO_Pin_8
#define ES8388_IIC_SDA_PIN      GPIO_Pin_9

#define ES8388_IIC_BOUND        100000        //100KHz
#define ES8388_ADDR      0x10          //ES8388的器件地址,固定为0x10

u8 ES8388_Init(void);
u8 ES8388_Write_Reg(u8 reg, u8 val);
u8 ES8388_Read_Reg(u8 reg);
void ES8388_I2S_Cfg(u8 fmt, u8 len);
void ES8388_ADDA_Cfg(uint8_t dacen, uint8_t adcen);
void ES8388_Output_Cfg(uint8_t o1en, uint8_t o2en);
void ES8388_Input_Cfg(u8 in);
void ES8388_hpvol_set(uint8_t volume);
void ES8388_spkvol_set(uint8_t volume);
void ES8388_close(void);

#endif
