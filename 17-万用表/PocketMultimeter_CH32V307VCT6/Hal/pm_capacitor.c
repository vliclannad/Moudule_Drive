/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_capacitor.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include "pm_capacitor.h"
#include "pm_measure.h"
#include "pm_probe.h"
#include "pm_adc.h"
#include "pm_timer.h"
#include <math.h>


#define CONFIG_PM_CAPACITOR_DEBUG
#ifdef CONFIG_PM_CAPACITOR_DEBUG
#define LOG_INFO(...)       printf(__VA_ARGS__)
#else
#define LOG_INFO(...)
#endif


/*******************************************************************************
 * @fn      PM_CheckIsCapacitor
 *
 * @brief   判断器件是否为电容,通过采集电容充电和放电过程的电流
 *
 * @param   none

 * @return  FAILURE or SUCCESS
*******************************************************************************/
u8 PM_CheckIsCapacitor(void)
{
    u16 adc_val1, adc_val2;     //采集ADC值
    u32 vol_val1, vol_val2;     //实际电压值mV
    u8 state = FAILURE;         //电容判断状态

    /* step1 */
    PM_ComponentDischarge(); //放电
    Delay_Ms(50);

    /* step2 */
    //电容充电
    PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV); //PortGroup_1->PortGroup_2
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, LOW_LV);
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, HIGH_LV);
    adc_val1 = PM_GetADC(PortGroup_2.ADC_Channel, ADC_SampleTime_239Cycles5); //采集下端电平
    vol_val1 = PM_ADCRealVol(adc_val1);
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV); //加快充电速度
    Delay_Ms(50);

    /* step3 */
    //电容放电
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, LOW_LV);
    Delay_Us(60); //放掉IO口内部存储的电
    adc_val2 = PM_GetADC(PortGroup_1.ADC_Channel, ADC_SampleTime_239Cycles5); //采集上端电平
    vol_val2 = PM_ADCRealVol(adc_val2);
    LOG_INFO("1<->2:%d | %d\r\n", vol_val1, vol_val2);

    if ((vol_val1 > CAP_LOWER_LIMIT_VOLTAGE) && (vol_val2 > CAP_LOWER_LIMIT_VOLTAGE)) //充电时在下端采集到电平（存在充电电流），放电时在上端采集的电平（存在放电电流）
    {
        ComponentMeasureResult.ComponentType = COMPONENT_CAPACITANCE;
        ComponentMeasureResult.ComponentParam.CapParam.Front = PortGroup_1.Probe_Num;
        ComponentMeasureResult.ComponentParam.CapParam.Rear = PortGroup_2.Probe_Num;
        state = SUCCESS;
        return state;
    }
    PM_ComponentDischarge(); //放电
    Delay_Ms(50);

    /* step4 */
    //电容充电
    PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV); //PortGroup_2->PortGroup_3
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, LOW_LV);
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, HIGH_LV);
    adc_val1 = PM_GetADC(PortGroup_3.ADC_Channel, ADC_SampleTime_239Cycles5); //采集下端电平
    vol_val1 = PM_ADCRealVol(adc_val1);
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV); //加快充电速度
    Delay_Ms(50);

    /* step5 */
    //电容放电
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, LOW_LV);
    Delay_Us(60); //放掉IO口内部存储的电
    adc_val2 = PM_GetADC(PortGroup_2.ADC_Channel, ADC_SampleTime_239Cycles5); //采集上端电平
    vol_val2 = PM_ADCRealVol(adc_val2);
    LOG_INFO("2<->3:%d | %d\r\n", vol_val1, vol_val2);
    if ((vol_val1 > CAP_LOWER_LIMIT_VOLTAGE) && (vol_val2 > CAP_LOWER_LIMIT_VOLTAGE)) //充电时在下端采集到电平（存在充电电流），放电时在上端采集的电平（存在放电电流）
    {
        ComponentMeasureResult.ComponentType = COMPONENT_CAPACITANCE;
        ComponentMeasureResult.ComponentParam.CapParam.Front = PortGroup_2.Probe_Num;
        ComponentMeasureResult.ComponentParam.CapParam.Rear = PortGroup_3.Probe_Num;
        state = SUCCESS;
        return state;
    }
    PM_ComponentDischarge(); //放电
    Delay_Ms(50);

    /* step6 */
    //电容充电
    PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV); //PortGroup_1->PortGroup_3
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, LOW_LV);
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, HIGH_LV);
    adc_val1 = PM_GetADC(PortGroup_3.ADC_Channel, ADC_SampleTime_239Cycles5); //采集下端电平
    vol_val1 = PM_ADCRealVol(adc_val1);
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV); //加快充电速度
    Delay_Ms(50);

    /* step7 */
    //电容放电
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, LOW_LV);
    Delay_Us(60); //放掉IO口内部存储的电
    adc_val2 = PM_GetADC(PortGroup_1.ADC_Channel, ADC_SampleTime_239Cycles5); //采集上端电平
    vol_val2 = PM_ADCRealVol(adc_val2);
    LOG_INFO("1<->3:%d | %d\r\n", vol_val1, vol_val2);

    if ((vol_val1 > CAP_LOWER_LIMIT_VOLTAGE) && (vol_val2 > CAP_LOWER_LIMIT_VOLTAGE)) //充电时在下端采集到电平（存在充电电流），放电时在上端采集的电平（存在放电电流）
    {
        ComponentMeasureResult.ComponentType = COMPONENT_CAPACITANCE;
        ComponentMeasureResult.ComponentParam.CapParam.Front = PortGroup_1.Probe_Num;
        ComponentMeasureResult.ComponentParam.CapParam.Rear = PortGroup_3.Probe_Num;
        state = SUCCESS;
        return state;
    }

    return state;
}


/*******************************************************************************
 * @fn      PM_MeasureCapacitor
 *
 * @brief   电容值测量
 *
 * @param   none
 *
 * @return  FAILURE or SUCCESS
*******************************************************************************/
u8 PM_MeasureCapacitor(void)
{
    u16 adc_val;            //采集ADC值
    u32 vol_val;            //实际电压值mV
    u16 count;              //定时器计数值
    u32 overflow;           //定时器溢出计数值
    u32 time;               //充电时间
    u8 state = FAILURE;     //电容测量状态
    u32 i = 0;

    //三种接法依次判断测量
    if ((ComponentMeasureResult.ComponentParam.CapParam.Front == 1) && (ComponentMeasureResult.ComponentParam.CapParam.Rear == 3))
    {
        /* step1 */
        PM_ComponentDischarge();
        Delay_Ms(50);

        /* step2 */
        //电容充电
        i = 0;
        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV); //VCC---470K-|-x---GND
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV); //使用470K充电以限制电流，因为电流较低可以忽略IO推挽输出MOS管的压降
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //计数器使能

        while (i < CAP_UPPER_LIMIT_CHARGE_TIME) //每个循环约77614ns
        {
            adc_val = PM_DMA_GetADCAverage_ForCAP(PortGroup_1.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val > CAP_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //计数器失能
        PM_TimerCountClear();

        if (i > 0)
        {
            vol_val = PM_ADCRealVol(adc_val);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.CapParam.CapVal = time / (470000.0 * log(3300.0 / (3300.0 - vol_val))); //计算电容值 nF
            LOG_INFO("470K | CapVal = %lf\r\n", ComponentMeasureResult.ComponentParam.CapParam.CapVal);
            state = SUCCESS;
        }
    }
    else if ((ComponentMeasureResult.ComponentParam.CapParam.Front == 1) && (ComponentMeasureResult.ComponentParam.CapParam.Rear == 2))
    {
        /* step1 */
        PM_ComponentDischarge();
        Delay_Ms(50);

        /* step2 */
        //电容充电
        i = 0;
        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV); //VCC---470K-|-x---GND
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV); //使用470K充电以限制电流，因为电流较低可以忽略IO推挽输出MOS管的压降
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //计数器使能

        while (i < CAP_UPPER_LIMIT_CHARGE_TIME) //每个循环约77614ns
        {
            adc_val = PM_DMA_GetADCAverage_ForCAP(PortGroup_1.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val > CAP_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //计数器失能
        PM_TimerCountClear();

        if (i > 0)
        {
            vol_val = PM_ADCRealVol(adc_val);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.CapParam.CapVal = time / (470000.0 * log(3300.0 / (3300.0 - vol_val))); //计算电容值 nF
            LOG_INFO("470K | CapVal = %lf\r\n", ComponentMeasureResult.ComponentParam.CapParam.CapVal);
            state = SUCCESS;
        }
    }
    else if ((ComponentMeasureResult.ComponentParam.CapParam.Front == 2) && (ComponentMeasureResult.ComponentParam.CapParam.Rear == 3))
    {
        /* step1 */
        PM_ComponentDischarge();
        Delay_Ms(50);

        /* step2 */
        //电容充电
        i = 0;
        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV); //VCC---470K-|-x---GND
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV); //使用470K充电以限制电流，因为电流较低可以忽略IO推挽输出MOS管的压降
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //计数器使能

        while (i < CAP_UPPER_LIMIT_CHARGE_TIME) //每个循环约77614ns
        {
            adc_val = PM_DMA_GetADCAverage_ForCAP(PortGroup_2.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val > CAP_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //计数器失能
        PM_TimerCountClear();

        if (i > 0)
        {
            vol_val = PM_ADCRealVol(adc_val);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.CapParam.CapVal = time / (470000.0 * log(3300.0 / (3300.0 - vol_val))); //计算电容值 nF
            LOG_INFO("470K | CapVal = %lf\r\n", ComponentMeasureResult.ComponentParam.CapParam.CapVal);
            state = SUCCESS;
        }
    }

    return state;
}


/******************************** endfile @ pm_capacitor ******************************/
