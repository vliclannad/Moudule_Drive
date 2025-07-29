#ifndef __WS2812B_H
#define __WS2812B_H

#include "debug.h"


#define WS2812B_GPIO_PORT       GPIOB
#define WS2812B_GPIO_CLK        RCC_APB2Periph_GPIOB
#define WS2812B_GPIO_PIN        GPIO_Pin_2

void WS_Init();
//======================================================================
//�������ƣ�ws_sendOnePix
//����������ptr�����������ɫ���ֽ�����;num:��������
//�������أ���
//���ܸ�Ҫ������ws2812�������ɫ
//======================================================================
void WS_SendOnePix(uint8_t *ptr,uint8_t num);

#endif
