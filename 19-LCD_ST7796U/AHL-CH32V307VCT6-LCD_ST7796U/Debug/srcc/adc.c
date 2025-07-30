//======================================================================
//文件名称：adc.c
//版权所有：SD-Arm(sumcu.suda.edu.cn)
//版本更新：2021-10-26  V1.1
//功能描述：CH32V307芯片AD转换头文件
//         采集精度12位
//======================================================================

#include "adc.h"

//======================================================================
//函数名称：adc_init
//功能概要：初始化一个AD通道号
//参数说明：Channel：通道号。可选范围：ADC_CHANNEL_x(0=<x<=15)、
//                              ADC_CHANNEL_TEMPSENSOR(16)、
//                              ADC_CHANNEL_VREFINT(17)
//         NC：本函数未使用，为增强函数可移植性
//======================================================================
void adc_init(uint16_t Channel, uint16_t Nc)
{
        Nc = 1;
    //(1)开启ADC时钟
    RCC->APB2PCENR |= RCC_ADC1EN;
    //(2)ADC时钟频率分频,PCLK2时钟8分频
    RCC->CFGR0 |=(0x3UL << (14U));
    //(3)配置寄存器：右对齐、单次单通道转换
    ADC1->CTLR1 &= ~(0x7UL << (13U));      //置0，单通道
    ADC1->CTLR1 &=~ ADC_SCAN;              //禁止扫描模式
    ADC1->CTLR1 &= ~(0x1FUL << (0U));
    ADC1->CTLR1 |= (0x1UL << (5U));        //使能转换结束中断（EOC标志）

    ADC1->CTLR2 &=~ ADC_ALIGN;             //对齐方式(右对齐)
    ADC1->CTLR2 &=~ ADC_CONT;              //单次转换模式
    ADC1->CTLR2 &=~ (0x1UL << (20U));      //禁止外部事件触发
    ADC1->CTLR2 &= ~(0x7UL << (17U));
    ADC1->CTLR2 |=  (0x7UL << (17U));      //REXTSEL软件触发

    //(4)常规通道序列长度为1
    ADC1->RSQR1 &= ~(0xFUL << 20U);
    //(5)退出断电模式          开启ADC，并启动转换
    ADC1->CTLR2 |= ADC_ADON;
    ADC1->CTLR1 &= ~(1<<26);       //26位，ADC_Buffer除能
    ADC1->CTLR2 |= ADC_RSTCAL;      //初始化校准寄存器
    //查询指定次数是否初始化校正完成（增加验证校准是否完成是为了增加测量的准确性）
    while((ADC1->CTLR2 &(0x1UL<<(3U)))==(0x1UL<<(3U)));
    ADC1->CTLR2 |= ADC_CAL;      //开始校准
    while((ADC1->CTLR2 &(0x1UL<<(2U)))==(0x1UL<<(2U)));
    ADC1->CTLR1 |= (0x1UL << (26U));      //26位，ADC_Buffer使能
    
    
    //(6)使能ADC内部采集功能       采集温度和电压
    //内置温度传感器 更适合监测温度变化 不是测量绝对温度
    if ((Channel == 16)||(Channel ==17))
    {
        ADC1->CTLR2 |= ADC_TSVREFE;
    }


}

//======================================================================
//函数名称：adc_read
//功能概要：将模拟量转换成数字量，并返回
//参数说明：Channel：通道号。可选范围：ADC_CHANNEL_x(0=<x<=15)、
//                              ADC_CHANNEL_TEMPSENSOR(16)、
//                              ADC_CHANNEL_VREFINT(17)
//======================================================================
uint16_t adc_read(uint16_t Channel)
{
    uint32_t ADCResult;     //用于存放AD值
    ADCResult = 0;
   //（1）配置采样周期
   if (Channel > 9)
   {
       ADC1->SAMPTR1 &= ~(0x7UL << (3 * (Channel - 10)));
       ADC1->SAMPTR1 |= (0x7UL << (3 * (Channel - 10)));  //配置采样时间为239.5周期
   }
   else
   {
       ADC1->SAMPTR2 &= ~(0x7UL << (3 * Channel ));
       ADC1->SAMPTR2 |= (0x7UL << (3 * Channel ));
   }

    //(2)清空第一次转换序列(单通道)
    ADC1->RSQR3 &=~ (0x1FUL << (0U));
    //(3)所选通道加入第一个转换序列中
    ADC1->RSQR3 |= ((uint8_t)Channel << (0U));

    //(4)开始转换，RSWSTART
    ADC1->CTLR2 |= (0x1UL << (20U));     //使能外部事件触发
    ADC1->CTLR2 |= (0X1UL << (22U));      //启动规则通道转换

    //（5）等待转换完成，即EOC标志
    while((ADC1->STATR & (0x1UL << (1U))  != (0x1UL<<(1U))));
    ADCResult = (uint16_t)ADC1->RDATAR;   //读取数据,清转换完成标志位
    return ADCResult;
}

//======================================================================
//函 数 名:adc_mid                                                  
//功    能:获取通道channel中值滤波后的A/D转换结果                    
//参    数:channel = 通道号                                           
//返    回:该通道中值滤波后的A/D转换结果                         
//内部调用:adc_read                                               
//======================================================================
uint16_t adc_mid(uint16_t Channel)
{
    uint16_t a,b,c;
    volatile uint16_t i;
    
    //1.取三次A/D转换结果
    a = adc_read(Channel);
    for (i=0;i<500;i++) __asm("nop");
    b = adc_read(Channel);
    for (i=0;i<500;i++) __asm("nop");
    c = adc_read(Channel);
    //2.从三次A/D转换结果中取中值
    return  a > b ? (b > c ? b : ( a > c ? c : a)) : ( a > c ? a: (b > c ? c : a));
}

//======================================================================
//函 数 名:adc_ave                                                    
//功    能:1路A/D转换函数(均值滤波),通道channel进行n次中值滤波,求和再作  
//          均值,得出均值滤波结果                                        
//参    数:channel = 通道号,n = 中值滤波次数                               
//返    回:该通道均值滤波后的A/D转换结果                                   
//内部调用:adc_mid                                                          
//======================================================================
uint16_t adc_ave(uint16_t Channel,uint8_t n) 
{
    uint16_t i;
    uint32_t j;
    j = 0;
    for (i = 0; i < n; i++) j += adc_mid(Channel);
    j = j/n;
    return (uint16_t)j;
}
