#ifndef __WS2812B_H
#define __WS2812B_H

#include "debug.h"


#define WS2812B_GPIO_PORT       GPIOB
#define WS2812B_GPIO_CLK        RCC_APB2Periph_GPIOB
#define WS2812B_GPIO_PIN        GPIO_Pin_2

void WS_Init();
//======================================================================
//函数名称：ws_sendOnePix
//函数参数：ptr：代表灯珠颜色的字节数组;num:灯珠数量
//函数返回：无
//功能概要：设置ws2812灯珠的颜色
//======================================================================
void WS_SendOnePix(uint8_t *ptr,uint8_t num);

#endif
