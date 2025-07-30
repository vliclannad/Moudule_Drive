//======================================================================
//文件名称：adc.h
//框架提供：SD-Arm(sumcu.suda.edu.cn)
//版本更新：20210309-20211111
//功能描述：CH32V103R8T6芯片AD转换头文件
//采集精度12位
//======================================================================

#ifndef __ADC_H_   //防止重复定义（开头）
#define __ADC_H_

#include "mcu.h"
#include "gpio.h"

//通道号宏定义
#define ADC_CHANNEL_0 0           //通道0
#define ADC_CHANNEL_1 1           //通道1
#define ADC_CHANNEL_2 2           //通道2
#define ADC_CHANNEL_3 3           //通道3
#define ADC_CHANNEL_4 4           //通道4
#define ADC_CHANNEL_5 5           //通道5
#define ADC_CHANNEL_6 6           //通道6
#define ADC_CHANNEL_7 7           //通道7
#define ADC_CHANNEL_8 8           //通道8
#define ADC_CHANNEL_9 9           //通道9
#define ADC_CHANNEL_10 10         //通道10
#define ADC_CHANNEL_11 11         //通道11
#define ADC_CHANNEL_12 12         //通道12
#define ADC_CHANNEL_13 13         //通道13
#define ADC_CHANNEL_14 14         //通道14  (PTC_NUM|4)
#define ADC_CHANNEL_15 15         //通道15
#define ADC_CHANNEL_TEMPSENSOR 16 //内部温度检测 需要使能TEMPSENSOR
#define ADC_CHANNEL_VREFINT 17     //内部参考电压监测 需要使能VREFINT功能

//温度采集参数AD_CAL2与AD_CAL1
#define AD_CAL2 (*(uint16_t*) 0x1FFF75CA)
#define AD_CAL1 (*(uint16_t*) 0x1FFF75A8)



//======================================================================
//函数名称：adc_init
//功能概要：初始化一个AD通道号
//参数说明：Channel：通道号。可选范围：ADC_CHANNEL_x(0=<x<=15)、
//                              ADC_CHANNEL_TEMPSENSOR(16)、
//                              ADC_CHANNEL_VREFINT(17)
//         NC：本函数未使用，为增强函数可移植性
//======================================================================
void adc_init(uint16_t Channel, uint16_t Nc);



//======================================================================
//函数名称：adc_read
//功能概要：将模拟量转换成数字量，并返回
//参数说明：Channel：通道号。可选范围：ADC_CHANNEL_x(0=<x<=15)、
//                              ADC_CHANNEL_TEMPSENSOR(16)、
//                              ADC_CHANNEL_VREFINT(17)
//======================================================================
uint16_t adc_read(uint16_t Channel);


//======================================================================
//函 数 名:adc_mid                                                  
//功    能:获取通道channel中值滤波后的A/D转换结果                    
//参    数:channel = 通道号                                           
//返    回:该通道中值滤波后的A/D转换结果                         
//内部调用:adc_read                                               
//======================================================================
uint16_t adc_mid(uint16_t Channel);

//======================================================================
//函 数 名:adc_ave                                                    
//功    能:1路A/D转换函数(均值滤波),通道channel进行n次中值滤波,求和再作  
//          均值,得出均值滤波结果                                        
//参    数:channel = 通道号,n = 中值滤波次数                               
//返    回:该通道均值滤波后的A/D转换结果                                   
//内部调用:adc_mid                                                          
//======================================================================
uint16_t adc_ave(uint16_t Channel,uint8_t n); 


#endif    
