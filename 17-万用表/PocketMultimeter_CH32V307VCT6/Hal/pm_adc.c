/********************************** (C) COPYRIGHT *****************************
* File Name          : pm_adc.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include "pm_adc.h"


/******************************************************************************
 *                             GLOBAL VARIABLES
 ******************************************************************************/
s16 Calibrattion_Val = 0;           //ADC校准值
u16 ADC_Buf[ADC_BUF_MAX_LEN];       //ADC采集值缓冲区


/*******************************************************************************
 * @fn      PM_ADCInit
 *
 * @brief   ADC1初始化，GPIO通过其他函数初始化为模拟输入，DMA1初始化
 *
 * @param   none
 *
 * @return  none
 *******************************************************************************/
void PM_ADCInit(void)
{
    ADC_InitTypeDef  ADC_InitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE );
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);                                       //ADC时钟不能超过14M,设置分频因子为8,96M/8=12M

    ADC_DeInit(ADC1);                                                       //复位ADC1,将外设 ADC1的全部寄存器重设为缺省值
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                      //ADC1工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                           //关闭扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                     //关闭连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;     //转换由软件而不是外部触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                  //ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                                 //规则转换的ADC通道的数目
    ADC_Init(ADC1, &ADC_InitStructure);                                     //ADC1初始化

    ADC_Cmd(ADC1, ENABLE);      //使能ADC1

    ADC_BufferCmd(ADC1, DISABLE);                           //使能校准前要关闭Buffer
    ADC_ResetCalibration(ADC1);                             //使能复位校准
    while(ADC_GetResetCalibrationStatus(ADC1));             //等待复位校准结束
    ADC_StartCalibration(ADC1);                             //开启AD校准
    while(ADC_GetCalibrationStatus(ADC1));                  //等待校准结束
    Calibrattion_Val = Get_CalibrationValue(ADC1);          //获取ADC1校准值

    DMA_DeInit(DMA1_Channel1);                                                  //复位DMA Channel1
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->RDATAR;              //从ADC的规则数据寄存器获取数据
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_Buf;                        //ADC数据接收缓冲区
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //数据源来自于ADC
    DMA_InitStructure.DMA_BufferSize = ADC_BUF_MAX_LEN;                         //缓冲区的大小为32
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //ADC数据源的地址不要递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //缓冲区的地址要递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据大小为半字
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //缓冲区数据大小为半字
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               //DMA存储模式不要循环
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                     //通道优先级为最高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //不是缓冲区到缓冲区
    DMA_Init( DMA1_Channel1, &DMA_InitStructure );
}


/**********************************************************************
 * @fn      PM_ADCContinuousConvModeCmd
 *
 * @brief   Enables or disables the continuous convert mode of ADC.
 *
 * @param   ADCx - where x can be 1 or 2 to select the ADC peripheral.
 *          NewState - ENABLE or DISABLE.
 *
 * @return  none
 ***********************************************************************/
void PM_ADCContinuousConvModeCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    if(NewState != DISABLE)
    {
        ADCx->CTLR2 |= CTLR2_CONT_Set;
    }
    else
    {
        ADCx->CTLR2 &= CTLR2_CONT_Reset;
    }
}


/*********************************************************************
 * @fn      PM_ADCCalibrattion
 *
 * @brief   Get Conversion Value.
 *
 * @param   val - Sampling value
 *
 * @return  val+Calibrattion_Val - Conversion Value.
 *********************************************************************/
u16 PM_ADCCalibrattion(s16 val)
{
    if ((val + Calibrattion_Val) < 0)
    {
        return 0;
    }
    if (((val + Calibrattion_Val) > 4095) || (val == 4095))
    {
        return 4095;
    }
    return (val + Calibrattion_Val);
}


/*******************************************************************************
 * @fn      PM_GetADC
 *
 * @brief   获取ADC(单次采集)
 *
 * @param   ch - 采样通道选择
 *          sample_time - 采样时间选择
 *
 * @return  ADC的值
 *******************************************************************************/
u16 PM_GetADC(u8 ch, u8 sample_time)
{
    u16 val;

    ADC_RegularChannelConfig(ADC1, ch, 1, sample_time);     //设置指定ADC的规则组通道，一个序列
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);                      //清楚EOC标志以防前一次转换未被读取
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                 //使能指定的ADC1的软件转换启动功能
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));          //等待转换结束
    val = ADC_GetConversionValue(ADC1);                     //ADC1规则组的转换结果
    val = PM_ADCCalibrattion((s16)val);                     //数值校准

    return val;
}


/*******************************************************************************
 * @fn      PM_GetADCAverage
 *
 * @brief   获取ADC(多次取平均值)
 *
 * @param   ch - 采样通道选择
 *          times - 采集次数
 *          sample_time - 采样时间选择
 *
 * @return  (u16)ADC的值
 *******************************************************************************/
u16 PM_GetADCAverage(u8 ch, u8 times, u8 sample_time)
{
    u32 val_sum = 0;
    u16 val = 0;
    u8 i;

    PM_GetADC(ch, ADC_SampleTime_239Cycles5);               //第一次转换值丢弃
    Delay_Us(50);

    ADC_RegularChannelConfig(ADC1, ch, 1, sample_time);     //设置指定ADC的规则组通道，一个序列
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);                      //清楚EOC标志以防前一次转换未被读取
    for(i=0; i<times; ++i)
    {
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);             //使能指定的ADC1的软件转换启动功能
        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));      //等待转换结束
        val_sum += ADC_GetConversionValue(ADC1);            //ADC1规则组的转换结果
        Delay_Us(50);
    }

    val = (val_sum * 10 / times + 5) / 10;                  //计算平均值，四舍五入
    val = PM_ADCCalibrattion((s16)val);                     //数值校准

    return val;
}


/*******************************************************************************
 * @fn      PM_DMA_GetADCAverage
 *
 * @brief   获取ADC(多次取平均值)，使用DMA
 *
 * @param   ch - 采样通道选择
 *          times - 采集次数
 *          sample_time - 采样时间选择
 *
 * @return  (u16)ADC的值
 *******************************************************************************/
u16 PM_DMA_GetADCAverage(u8 ch, u8 times, u8 sample_time)
{
    u32 val_sum = 0;
    u16 val = 0;
    u8 i;

    PM_GetADC(ch, ADC_SampleTime_239Cycles5);               //第一次转换值丢弃
    Delay_Us(50);

    if(times > ADC_BUF_MAX_LEN) //采集次数不能超过DMA缓冲区的大小
    {
        times = ADC_BUF_MAX_LEN;
    }
    DMA_SetCurrDataCounter(DMA1_Channel1, times);           //设置DMA结束数据的个数,DMA不使能才能够设置
    DMA_Cmd( DMA1_Channel1, ENABLE );                       //使能DMA
    ADC_RegularChannelConfig(ADC1, ch, 1, sample_time );    //设置指定ADC的规则组通道，一个序列
    PM_ADCContinuousConvModeCmd(ADC1, ENABLE);              //启动ADC的连续转换模式
    ADC_DMACmd(ADC1, ENABLE);                               //使能DMA模式
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                 //软件触发ADC转换

    while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);       //等待DMA缓冲区满
    DMA_Cmd( DMA1_Channel1, DISABLE );                      //失能DMA
    DMA_ClearFlag(DMA1_FLAG_TC1);                           //清除缓冲区满标志
    PM_ADCContinuousConvModeCmd(ADC1, DISABLE);             //关闭ADC的连续转换模式,否则ADC将不会停止转换
    ADC_DMACmd(ADC1, DISABLE);                              //关闭DMA模式，否则下一次DMA将接收到本次最后一次转换完成的数据

    for(i=0; i<times; ++i)
    {
        val_sum += ADC_Buf[i];
    }
    val = (val_sum * 10 / times + 5) / 10;                  //计算平均值，四舍五入
    val = PM_ADCCalibrattion((s16)val);                     //数值校准

    return val;
}


/*******************************************************************************
 * @fn      PM_DMA_GetADCAverage_ForCAP
 *
 * @brief   获取ADC，使用DMA
 *
 * @param   ch - 采样通道选择
 *          sample_time - 采样时间选择
 *          *p_count - 定时器计数值
 *          *p_overflow - 定时器溢出计数值
 *
 * @return  (u16)ADC的值
 *******************************************************************************/
u16 PM_DMA_GetADCAverage_ForCAP(u8 ch, u8 sample_time, u16 *p_count, u32 *p_overflow)
{
    u16 val = 0;

    PM_GetADC(ch, ADC_SampleTime_239Cycles5);               //第一次转换值丢弃
    Delay_Us(50);

    DMA_SetCurrDataCounter(DMA1_Channel1, 1);               //设置DMA结束数据的个数,DMA不使能才能够设置
    DMA_Cmd( DMA1_Channel1, ENABLE );                       //使能DMA
    ADC_RegularChannelConfig(ADC1, ch, 1, sample_time );    //设置指定ADC的规则组通道，一个序列
    PM_ADCContinuousConvModeCmd(ADC1, ENABLE);              //启动ADC的连续转换模式
    ADC_DMACmd(ADC1, ENABLE);                               //使能DMA模式
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                 //软件触发ADC转换
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);       //等待DMA缓冲区满

    *p_overflow = TimerOverflowNum;
    *p_count = TIM_GetCounter(TIM2);
    if(*p_overflow != TimerOverflowNum)
    {
        *p_count = TIM_GetCounter(TIM2);
        *p_overflow = TimerOverflowNum;
    }

    DMA_Cmd( DMA1_Channel1, DISABLE );                      //失能DMA
    DMA_ClearFlag(DMA1_FLAG_TC1);                           //清除缓冲区满标志
    PM_ADCContinuousConvModeCmd(ADC1, DISABLE);             //关闭ADC的连续转换模式,否则ADC将不会停止转换
    ADC_DMACmd(ADC1, DISABLE);                              //关闭DMA模式，否则下一次DMA将接收到本次最后一次转换完成的数据

    val = PM_ADCCalibrattion((s16)ADC_Buf[0]);                     //数值校准

    return val;
}


/*******************************************************************************
 * @fn      PM_ADCRealVol
 *
 * @brief   ADC实际电压值计算
 *
 * @param   val - ADC的值
 *
 * @return  实际电压值,单位为mV
 *******************************************************************************/
u32 PM_ADCRealVol(u16 val)
{
    return (((((u32) val * 8250) >> 10) + 5) / 10); // val / 4096 * 3300 + 0.5 四舍五入
}


/******************************** endfile @ pm_adc ******************************/
