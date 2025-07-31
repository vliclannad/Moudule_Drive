/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_diode.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include "pm_diode.h"
#include "pm_measure.h"
#include "pm_probe.h"
#include "pm_adc.h"
#include "pm_timer.h"
#include <math.h>


#define CONFIG_PM_DIODE_DEBUG
#ifdef CONFIG_PM_DIODE_DEBUG
#define LOG_INFO(...)       printf(__VA_ARGS__)
#else
#define LOG_INFO(...)
#endif


/*******************************************************************************
 * @fn      PM_MeasureDiode
 *
 * @brief   二极管检测
 *
 * @param   none
 *
 * @return  FAILURE or SUCCESS
*******************************************************************************/
u8 PM_MeasureDiode(void)
{
    u16 adc_val1, adc_val2;     //ADC采集值
    u32 vol_val1, vol_val2;     //ADC对应的实际电压值
    u16 count;                  //定时器计数值
    u32 overflow;               //定时器溢出计数值
    u32 time;                   //充电时间
    u8 state = FAILURE;         //二极管测量状态
    u32 i = 0;

    if ((ComponentMeasureResult.ComponentParam.DiodeParam.Positive == 1) && (ComponentMeasureResult.ComponentParam.DiodeParam.Negative == 3)) //二极管导通方向为表笔1->表笔3
    {
        /* step1 */
        //计算导通电压
        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, HIGH_LV);
        Delay_Ms(50);
        adc_val1 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //VCC---680-|-x---680---GND
        vol_val1 = PM_ADCRealVol(adc_val1);
        adc_val2 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //VCC---680---x-|-680---GND
        vol_val2 = PM_ADCRealVol(adc_val2);
        ComponentMeasureResult.ComponentParam.DiodeParam.Uon = vol_val1 - vol_val2;
        LOG_INFO("Uon = %d\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Uon);

        PM_ComponentDischarge();
        Delay_Ms(50);

        /* step2 */
        //电容充电
        i = 0;
        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV); //GND---x-|-470K---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //计数器使能

        while (i < DIO_UPPER_LIMIT_CHARGE_TIME) //每个循环约77614ns
        {
            adc_val1 = PM_DMA_GetADCAverage_ForCAP(PortGroup_3.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val1 > DIO_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //计数器失能
        PM_TimerCountClear();

        if (i > 0)
        {
            vol_val1 = PM_ADCRealVol(adc_val1);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = time / (470000.0 * log(3300.0 / (3300.0 - vol_val1))); //计算电容值 nF
            LOG_INFO("470K | Cj = %lf\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Cj);
        }
        else
        {
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = 0; //寄生电容为0或者过小无法测量
        }
        state = SUCCESS;
    }

    else if ((ComponentMeasureResult.ComponentParam.DiodeParam.Positive == 3) && (ComponentMeasureResult.ComponentParam.DiodeParam.Negative == 1)) //二极管导通方向为表笔3->表笔1)
    {
        /* step1 */
        //计算导通电压
        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV);
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, HIGH_LV);
        Delay_Ms(50);
        adc_val1 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //GND---680-|-x---680---VCC
        vol_val1 = PM_ADCRealVol(adc_val1);
        adc_val2 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //GND---680---x-|-680---VCC
        vol_val2 = PM_ADCRealVol(adc_val2);
        ComponentMeasureResult.ComponentParam.DiodeParam.Uon = vol_val1 - vol_val2;
        LOG_INFO("Uon = %d\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Uon);

        PM_ComponentDischarge();
        Delay_Ms(50);

        /* step2 */
        //电容充电
        i = 0;
        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV); //VCC---470K-|-x---GND
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //计数器使能

        while (i < DIO_UPPER_LIMIT_CHARGE_TIME) //每个循环约77614ns
        {
            adc_val1 = PM_DMA_GetADCAverage_ForCAP(PortGroup_1.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val1 > DIO_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //计数器失能
        PM_TimerCountClear();

        if (i > 0)
        {
            vol_val1 = PM_ADCRealVol(adc_val1);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = time / (470000.0 * log(3300.0 / (3300.0 - vol_val1))); //计算电容值 nF
            LOG_INFO("470K | Cj = %lf\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Cj);
        }
        else
        {
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = 0; //寄生电容为0或者过小无法测量
        }
        state = SUCCESS;
    }

    else if ((ComponentMeasureResult.ComponentParam.DiodeParam.Positive == 1) && (ComponentMeasureResult.ComponentParam.DiodeParam.Negative == 2)) //二极管导通方向为表笔1->表笔2
    {
        /* step1 */
        //计算导通电压
        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, HIGH_LV);
        Delay_Ms(50);
        adc_val1 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //VCC---680-|-x---680---GND
        vol_val1 = PM_ADCRealVol(adc_val1);
        adc_val2 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //VCC---680---x-|-680---GND
        vol_val2 = PM_ADCRealVol(adc_val2);
        ComponentMeasureResult.ComponentParam.DiodeParam.Uon = vol_val1 - vol_val2;
        LOG_INFO("Uon = %d\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Uon);

        PM_ComponentDischarge();
        Delay_Ms(50);

        /* step2 */
        //电容充电
        i = 0;
        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV); //GND---x-|-470K---VCC
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //计数器使能

        while (i < DIO_UPPER_LIMIT_CHARGE_TIME) //每个循环约77614ns
        {
            adc_val1 = PM_DMA_GetADCAverage_ForCAP(PortGroup_2.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val1 > DIO_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //计数器失能
        PM_TimerCountClear();
        if (i > 0)
        {
            vol_val1 = PM_ADCRealVol(adc_val1);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = time / (470000.0 * log(3300.0 / (3300.0 - vol_val1))); //计算电容值 nF
            LOG_INFO("470K | Cj = %lf\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Cj);
        }
        else
        {
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = 0; //寄生电容为0或者过小无法测量
        }
        state = SUCCESS;
    }

    else if ((ComponentMeasureResult.ComponentParam.DiodeParam.Positive == 2) && (ComponentMeasureResult.ComponentParam.DiodeParam.Negative == 1)) //二极管导通方向为表笔2->表笔1
    {
        /* step1 */
        //计算导通电压
        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, HIGH_LV);
        Delay_Ms(50);
        adc_val1 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //GND---680-|-x---680---VCC
        vol_val1 = PM_ADCRealVol(adc_val1);
        adc_val2 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //GND---680---x-|-680---VCC
        vol_val2 = PM_ADCRealVol(adc_val2);
        ComponentMeasureResult.ComponentParam.DiodeParam.Uon = vol_val1 - vol_val2;
        LOG_INFO("Uon = %d\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Uon);

        PM_ComponentDischarge();
        Delay_Ms(50);

        /* step2 */
        //电容充电
        i = 0;
        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV); //VCC---470K-|-x---GND
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //计数器使能

        while (i < DIO_UPPER_LIMIT_CHARGE_TIME) //每个循环约77614ns
        {
            adc_val1 = PM_DMA_GetADCAverage_ForCAP(PortGroup_1.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val1 > DIO_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //计数器失能
        PM_TimerCountClear();
        if (i > 0)
        {
            vol_val1 = PM_ADCRealVol(adc_val1);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = time / (470000.0 * log(3300.0 / (3300.0 - vol_val1))); //计算电容值 nF
            LOG_INFO("470K | Cj = %lf\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Cj);
        }
        else
        {
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = 0; //寄生电容为0或者过小无法测量
        }
        state = SUCCESS;
    }

    else if ((ComponentMeasureResult.ComponentParam.DiodeParam.Positive == 2) && (ComponentMeasureResult.ComponentParam.DiodeParam.Negative == 3)) //二极管导通方向为表笔2->表笔3
    {
        /* step1 */
        //计算导通电压
        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, HIGH_LV);
        Delay_Ms(50);
        adc_val1 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //VCC---680-|-x---680---GND
        vol_val1 = PM_ADCRealVol(adc_val1);
        adc_val2 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //VCC---680---x-|-680---GND
        vol_val2 = PM_ADCRealVol(adc_val2);
        ComponentMeasureResult.ComponentParam.DiodeParam.Uon = vol_val1 - vol_val2;
        LOG_INFO("Uon = %d\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Uon);

        PM_ComponentDischarge();
        Delay_Ms(50);

        /* step2 */
        //电容充电
        i = 0;
        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV); //GND---x-|-470K---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //计数器使能

        while (i < DIO_UPPER_LIMIT_CHARGE_TIME) //每个循环约77614ns
        {
            adc_val1 = PM_DMA_GetADCAverage_ForCAP(PortGroup_3.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val1 > DIO_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //计数器失能
        PM_TimerCountClear();
        if (i > 0)
        {
            vol_val1 = PM_ADCRealVol(adc_val1);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = time / (470000.0 * log(3300.0 / (3300.0 - vol_val1))); //计算电容值 nF
            LOG_INFO("470K | Cj = %lf\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Cj);
        }
        else
        {
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = 0; //寄生电容为0或者过小无法测量
        }
        state = SUCCESS;
    }

    else if ((ComponentMeasureResult.ComponentParam.DiodeParam.Positive == 3) && (ComponentMeasureResult.ComponentParam.DiodeParam.Negative == 2)) //二极管导通方向为表笔3->表笔2
    {
        /* step1 */
        //计算导通电压
        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, LOW_LV);
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, HIGH_LV);
        Delay_Ms(50);
        adc_val1 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //GND---680-|-x---680---VCC
        vol_val1 = PM_ADCRealVol(adc_val1);
        adc_val2 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //GND---680---x-|-680---VCC
        vol_val2 = PM_ADCRealVol(adc_val2);
        ComponentMeasureResult.ComponentParam.DiodeParam.Uon = vol_val1 - vol_val2;
        LOG_INFO("Uon = %d\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Uon);

        PM_ComponentDischarge();
        Delay_Ms(50);

        /* step2*/
        //电容充电
        i = 0;
        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV); //VCC---470K-|-x---GND
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //计数器使能

        while (i < DIO_UPPER_LIMIT_CHARGE_TIME) //每个循环约77614ns
        {
            adc_val1 = PM_DMA_GetADCAverage_ForCAP(PortGroup_2.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val1 > DIO_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //计数器失能
        PM_TimerCountClear();
        if (i > 0)
        {
            vol_val1 = PM_ADCRealVol(adc_val1);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = time / (470000.0 * log(3300.0 / (3300.0 - vol_val1))); //计算电容值 nF
            LOG_INFO("470K | Cj = %lf\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Cj);
        }
        else
        {
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = 0; //寄生电容为0或者过小无法测量
        }
        state = SUCCESS;
    }

    ComponentMeasureResult.ComponentParam.DiodeParam.Cj = 0; //暂时不显示二极管的寄生电容
    return state;
}


/******************************** endfile @ pm_diode ******************************/
