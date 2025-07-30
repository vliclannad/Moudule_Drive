#ifndef __I2S_H
#define __I2S_H
#include "debug.h"



extern void (*i2s_tx_callback)(void);       //IIS TX回调函数指针


void USER_I2S_Init(u16 I2S_Standard,u16 I2S_Mode,u16 I2S_Clock_Polarity,u16 I2S_DataFormat,u32 I2S_Samplerate);

void DMA_Tx_Init( DMA_Channel_TypeDef* DMA_CHx, u32 ppadr, u32 memadr, u16 bufsize);



#endif // __I2S_H
