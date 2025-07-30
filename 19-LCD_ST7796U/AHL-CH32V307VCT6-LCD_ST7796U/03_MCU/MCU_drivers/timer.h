//=====================================================================
//文件名称：timer.h
//功能概要：timer底层驱动构件头文件
//制作单位：苏州大学嵌入式系统与物联网研究所(sumcu.suda.edu.cn)
//版    本：
//适用芯片：CH32V103R8T6
//=====================================================================
#ifndef TIMER_H
#define TIMER_H

#include "string.h"
#include "mcu.h"


#define TIMER1  1   
#define TIMER2  2
#define TIMER3  3  
#define TIMER6  6
#define TIMER7  7
#define TIMER8  8 

#define  RCC_TIM1EN                      ((uint32_t)0x00000800)        /* Timer 1 clock enabled */
#define  RCC_TIM8EN                      ((uint32_t)0x00002000)        /* Timer 8 clock enabled */
#define  RCC_TIM2EN                      ((uint32_t)0x00000001)        /* Timer 2 clock enabled*/
#define  RCC_TIM3EN                      ((uint32_t)0x00000002)        /* Timer 3 clock enabled */
#define  RCC_TIM6EN                      ((uint32_t)0x00000010)        /* Timer 6 clock enabled*/
#define  RCC_TIM7EN                      ((uint32_t)0x00000020)        /* Timer 7 clock enabled */

//=======================================================================
//函数名称：timer_init
//函数返回：无
//参数说明：timer_No：时钟模块号（使用宏定义TIMER1、TIMER2、…）
//          time_ms：定时器中断的时间间隔，单位为毫秒，合理范围：1~2^16ms
//功能概要：时钟模块初始化，其中Timer1、Timer8为高级定时器，Timer2、Timer3为通用定时器，
//          TIM6、TIM7为基本定时器
//=======================================================================

void timer_init(uint8_t timer_No,uint32_t time_ms);


//============================================================================
//函数名称：timer_enable_int
//函数返回：无
//参数说明： timer_No:时钟模块号 使用宏定义TIMER1、TIMER2、…）
//功能概要：时钟模块使能，开启时钟模块中断及定时器中断
//============================================================================
void timer_enable_int(uint8_t timer_No);

//============================================================================
//函数名称：timer_disable_int
//函数返回：无
//参数说明： timer_No:时钟模块号 （使用宏定义TIMER1、TIMER2、…）
//功能概要：定时器中断除能
//============================================================================
void timer_disable_int(uint8_t timer_No);

//===================================================================
//函数名称：timer_get_int
//参数说明： timer_No:时钟模块号 （使用宏定义TIMER1、TIMER2、…）
//功能概要：获取timer模块中断标志
//函数返回：中断标志 1=有对应模块中断产生;0=无对应模块中断产生
//===================================================================
uint8_t timer_get_int(uint8_t timer_No);

//============================================================================
//函数名称：timer_clear_int
//函数返回：无
//参数说明： timer_No:时钟模块号 （使用宏定义TIMER1、TIMER2、…）
//功能概要：定时器清除中断标志
//============================================================================
void timer_clear_int(uint8_t timer_No);


#endif


