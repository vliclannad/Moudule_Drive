#ifndef __TM1637_H     //防止重复定义（_ADLX345_H  开头)
#define __TM1637_H
#include "debug.h"
//====================接口函数声明=======================================

//======================================================================
//函数名称：TM1637_Init
//函数参数：tm1637用到的两个引脚clk_pin、dio_pin
//函数返回：无
//功能概要：向TM1637初始化
#define TM1637_CKL_PIN     GPIO_Pin_6
#define TM1637_DIO_PIN     GPIO_Pin_7
#define TM1637_PORT        GPIOE
#define TM1637_GPIO_CLK    RCC_APB2Periph_GPIOE
//======================================================================
void TM1637_Init(void);

//======================================================================
//函数名称：tm1637_Display
//函数参数：a,b,c,d分别表示在第1,2,3,4位数码管上显示的数字
//函数返回：无
//功能概要：向TM1637写入传输的一个字节的数据
//======================================================================
void TM1637_Display(uint8_t a,uint8_t a1,uint8_t b,uint8_t b1,uint8_t c,
		uint8_t c1,uint8_t d,uint8_t d1);



#endif    //防止重复定义（_ADLX345_H  结尾)
