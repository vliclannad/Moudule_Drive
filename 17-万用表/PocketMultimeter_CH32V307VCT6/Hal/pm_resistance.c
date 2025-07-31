/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_resistance.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include "pm_resistance.h"
#include "pm_measure.h"
#include "pm_probe.h"
#include "pm_adc.h"


#define CONFIG_PM_RESISTANCE_DEBUG
#ifdef CONFIG_PM_RESISTANCE_DEBUG
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
u8 PM_CheckIsResistance(void)
{
    u16 adc_val;            //采集ADC值
    u32 vol_val;            //实际电压值mV
    u8 state = FAILURE;     //电容判断状态

    /* step1 */
    PM_ComponentDischarge(); //放电
    Delay_Ms(50);

    /* step2 */
    //电容充电
    PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV); //PortGroup_1->PortGroup_2
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, LOW_LV);
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, HIGH_LV);
    Delay_Ms(50); //等待ADC内部的采样电容充电

    /* step3 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //采集470K分压
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("1<->2:%d | %d\r\n", vol_val, vol_val);

    if (vol_val > RES_LOWER_LIMIT_VOLTAGE)  //470K上存在分压
    {
        ComponentMeasureResult.ComponentType = COMPONENT_RESISTANCE;
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
    Delay_Ms(50); //等待ADC内部的采样电容充电

    /* step5 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //采集470K分压
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("2<->3:%d | %d\r\n", vol_val, vol_val);

    if (vol_val > RES_LOWER_LIMIT_VOLTAGE)  //470K上存在分压
    {
        ComponentMeasureResult.ComponentType = COMPONENT_RESISTANCE;
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
    Delay_Ms(50); //等待ADC内部的采样电容充电

    /* step7 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //采集下端电平
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("1<->3:%d | %d\r\n", vol_val, vol_val);

    if (vol_val > RES_LOWER_LIMIT_VOLTAGE)  //470K上存在分压
    {
        ComponentMeasureResult.ComponentType = COMPONENT_RESISTANCE;
        ComponentMeasureResult.ComponentParam.CapParam.Front = PortGroup_1.Probe_Num;
        ComponentMeasureResult.ComponentParam.CapParam.Rear = PortGroup_3.Probe_Num;
        state = SUCCESS;
        return state;
    }

    return state;
}


/*******************************************************************************
 * @fn      PM_MeasureResistance
 *
 * @brief   电阻值测量
 *
 * @param   none
 *
 * @return  FAILURE or SUCCESS
*******************************************************************************/
u8 PM_MeasureResistance(void)
{
    u16 adc_val1, adc_val2, adc_val3, adc_val4;     //ADC采集值
    u32 vol_val1, vol_val2, vol_val3, vol_val4;     //ADC对应的实际电压值
    double res_val1=0, res_val2=0, res_val3=0, res_val4=0;     //电阻值
    u8 ResMeasureFlg = 0x00;                        //FLAG,用于判断哪些电阻的分压值被采用
    u8 state = FAILURE;

    if ((ComponentMeasureResult.ComponentParam.ResParam.Front == 1) && (ComponentMeasureResult.ComponentParam.ResParam.Rear == 3))
    {
        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV); //第一次测量：GND---x-|-680---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, HIGH_LV);
        Delay_Ms(50);
        adc_val1 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val1 = PM_ADCRealVol(adc_val1);
        LOG_INFO("1<->3 adc_val1:%d | vol_val1:%d\r\n", adc_val1, vol_val1);

        if (vol_val1 > RES_UPPER_LIMIT_VOLTAGE) //待测电阻过大(>44K).设置最大值,当被测电阻较大时不采用680分压测量
        {
            ResMeasureFlg |= 0x01;
        }
        else
        {
            res_val1 = (vol_val1 - PortGroup_1.PortWithNone.NMOS.Intercept) * (680.0 + PortGroup_3.PortWith680.PMOS.Slope) / (3300.0 - PortGroup_3.PortWith680.PMOS.Intercept - vol_val1)
                       - PortGroup_1.PortWithNone.NMOS.Slope - PM_Probe1Res; //减去表笔1的电阻，和MOS管的电压开销
        }

        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV); //第二次测量：GND---680-|-x---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV);
        Delay_Ms(50);
        adc_val2 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val2 = PM_ADCRealVol(adc_val2);
        LOG_INFO("1<->3 adc_val2:%d | vol_val2:%d\r\n", adc_val2, vol_val2);

        if (vol_val2 < RES_LOWER_LIMIT_VOLTAGE) //待测电阻过大(>44K)
        {
            ResMeasureFlg |= 0x01;
        }
        else
        {
            res_val2 = (3300.0 - PortGroup_3.PortWithNone.PMOS.Intercept - vol_val2) * (680.0 + PortGroup_1.PortWith680.NMOS.Slope) / (vol_val2 - PortGroup_1.PortWith680.NMOS.Intercept)
                       - PortGroup_3.PortWithNone.PMOS.Slope - PM_Probe3Res; //减去表笔3的电阻，和MOS管的电压开销
        }

        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV); //第三次测量：GND---x-|-470k---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, HIGH_LV);
        Delay_Ms(50);
        adc_val3 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val3 = PM_ADCRealVol(adc_val3);
        LOG_INFO("1<->3 adc_val3:%d | vol_val3:%d\r\n", adc_val3, vol_val3);

        if ((vol_val3 < RES_LOWER_LIMIT_VOLTAGE) || (vol_val3 > RES_UPPER_LIMIT_VOLTAGE)) //待测量电阻过大(>30M)或过小(<7K)
        {
            ResMeasureFlg |= 0x02;
        }
        else
        {
            res_val3 = (vol_val3 - PortGroup_1.PortWithNone.NMOS.Intercept) * (470000.0 + PortGroup_3.PortWith470K.PMOS.Slope) / (3300.0 - PortGroup_3.PortWith470K.PMOS.Intercept - vol_val3)
                       - PortGroup_1.PortWithNone.NMOS.Slope - PM_Probe1Res; //减去表笔1的电阻，和MOS管的电压开销
        }

        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, LOW_LV); //第四次测量：GND---470K-|-x---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV);
        Delay_Ms(50);
        adc_val4 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val4 = PM_ADCRealVol(adc_val4);
        LOG_INFO("1<->3 adc_val4:%d | vol_val4:%d\r\n", adc_val4, vol_val4);

        if ((vol_val4 < RES_LOWER_LIMIT_VOLTAGE) || (vol_val4 > RES_UPPER_LIMIT_VOLTAGE)) //待测量电阻过大(>30M)或过小(<7K)
        {
            ResMeasureFlg |= 0x02;
        }
        else
        {

            res_val4 = (3300.0 - PortGroup_3.PortWithNone.PMOS.Intercept - vol_val4) * (470000.0 + PortGroup_1.PortWith470K.NMOS.Slope) / (vol_val4 - PortGroup_1.PortWith470K.NMOS.Intercept)
                       - PortGroup_3.PortWithNone.PMOS.Slope - PM_Probe3Res; //减去表笔3的电阻，和MOS管的电压开销
        }
    }
    else if ((ComponentMeasureResult.ComponentParam.ResParam.Front == 1) && (ComponentMeasureResult.ComponentParam.ResParam.Rear == 2))
    {
        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV); //第一次测量：GND---x-|-680---VCC
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, HIGH_LV);
        Delay_Ms(50);
        adc_val1 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val1 = PM_ADCRealVol(adc_val1);
        LOG_INFO("1<->2 adc_val1:%d | vol_val1:%d\r\n", adc_val1, vol_val1);

        if (vol_val1 > RES_UPPER_LIMIT_VOLTAGE) //待测电阻过大(>44K).设置最大值,当被测电阻较大时不采用680分压测量
        {
            ResMeasureFlg |= 0x01;
        }
        else
        {
            res_val1 = (vol_val1 - PortGroup_1.PortWithNone.NMOS.Intercept) * (680.0 + PortGroup_2.PortWith680.PMOS.Slope) / (3300.0 - PortGroup_2.PortWith680.PMOS.Intercept - vol_val1)
                       - PortGroup_1.PortWithNone.NMOS.Slope - PM_Probe1Res; //减去表笔1的电阻，和MOS管的电压开销
        }

        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV); //第二次测量：GND---680-|-x---VCC
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, HIGH_LV);
        Delay_Ms(50);
        adc_val2 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val2 = PM_ADCRealVol(adc_val2);
        LOG_INFO("1<->2 adc_val2:%d | vol_val2:%d\r\n", adc_val2, vol_val2);

        if (vol_val2 < RES_LOWER_LIMIT_VOLTAGE) //待测电阻过大(>44K)
        {
            ResMeasureFlg |= 0x01;
        }
        else
        {
            res_val2 = (3300.0 - PortGroup_2.PortWithNone.PMOS.Intercept - vol_val2) * (680.0 + PortGroup_1.PortWith680.NMOS.Slope) / (vol_val2 - PortGroup_1.PortWith680.NMOS.Intercept)
                       - PortGroup_2.PortWithNone.PMOS.Slope - PM_Probe2Res; //减去表笔2的电阻，和MOS管的电压开销
        }

        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV); //第三次测量：GND---x-|-470k---VCC
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, HIGH_LV);
        Delay_Ms(50);
        adc_val3 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val3 = PM_ADCRealVol(adc_val3);
        LOG_INFO("1<->2 adc_val3:%d | vol_val3:%d\r\n", adc_val3, vol_val3);

        if ((vol_val3 < RES_LOWER_LIMIT_VOLTAGE) || (vol_val3 > RES_UPPER_LIMIT_VOLTAGE)) //待测量电阻过大(>30M)或过小(<7K)
        {
            ResMeasureFlg |= 0x02;
        }
        else
        {
            res_val3 = (vol_val3 - PortGroup_1.PortWithNone.NMOS.Intercept) * (470000.0 + PortGroup_2.PortWith470K.PMOS.Slope) / (3300.0 - PortGroup_2.PortWith470K.PMOS.Intercept - vol_val3)
                       - PortGroup_1.PortWithNone.NMOS.Slope - PM_Probe1Res; //减去表笔1的电阻，和MOS管的电压开销
        }

        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, LOW_LV); //第四次测量：GND---470K-|-x---VCC
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, HIGH_LV);
        Delay_Ms(50);
        adc_val4 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val4 = PM_ADCRealVol(adc_val4);
        LOG_INFO("1<->2 adc_val4:%d | vol_val4:%d\r\n", adc_val4, vol_val4);

        if ((vol_val4 < RES_LOWER_LIMIT_VOLTAGE) || (vol_val4 > RES_UPPER_LIMIT_VOLTAGE)) //待测量电阻过大(>30M)或过小(<7K)
        {
            ResMeasureFlg |= 0x02;
        }
        else
        {
            res_val4 = (3300.0 - PortGroup_2.PortWithNone.PMOS.Intercept - vol_val4) * (470000.0 + PortGroup_1.PortWith470K.NMOS.Slope) / (vol_val4 - PortGroup_1.PortWith470K.NMOS.Intercept)
                       - PortGroup_2.PortWithNone.PMOS.Slope - PM_Probe2Res; //减去表笔2的电阻，和MOS管的电压开销
        }
    }
    else if ((ComponentMeasureResult.ComponentParam.ResParam.Front == 2) && (ComponentMeasureResult.ComponentParam.ResParam.Rear == 3))
    {
        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV); //第一次测量：GND---x-|-680---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, HIGH_LV);
        Delay_Ms(50);
        adc_val1 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val1 = PM_ADCRealVol(adc_val1);
        LOG_INFO("2<->3 adc_val1:%d | vol_val1:%d\r\n", adc_val1, vol_val1);

        if (vol_val1 > RES_UPPER_LIMIT_VOLTAGE) //待测电阻过大(>44K).设置最大值,当被测电阻较大时不采用680分压测量
        {
            ResMeasureFlg |= 0x01;
        }
        else
        {
            res_val1 = (vol_val1 - PortGroup_2.PortWithNone.NMOS.Intercept) * (680.0 + PortGroup_3.PortWith680.PMOS.Slope) / (3300.0 - PortGroup_3.PortWith680.PMOS.Intercept - vol_val1)
                       - PortGroup_2.PortWithNone.NMOS.Slope - PM_Probe2Res; //减去表笔2的电阻，和MOS管的电压开销
        }

        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, LOW_LV); //第二次测量：GND---680-|-x---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV);
        Delay_Ms(50);
        adc_val2 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val2 = PM_ADCRealVol(adc_val2);
        LOG_INFO("2<->3 adc_val2:%d | vol_val2:%d\r\n", adc_val2, vol_val2);

        if (vol_val2 < RES_LOWER_LIMIT_VOLTAGE) //待测电阻过大(>44K)
        {
            ResMeasureFlg |= 0x01;
        }
        else
        {
            res_val2 = (3300.0 - PortGroup_3.PortWithNone.PMOS.Intercept - vol_val2) * (680.0 + PortGroup_2.PortWith680.NMOS.Slope) / (vol_val2 - PortGroup_2.PortWith680.NMOS.Intercept)
                       - PortGroup_3.PortWithNone.PMOS.Slope - PM_Probe3Res; //减去表笔3的电阻，和MOS管的电压开销
        }

        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV); //第三次测量：GND---x-|-470k---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, HIGH_LV);
        Delay_Ms(50);
        adc_val3 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val3 = PM_ADCRealVol(adc_val3);
        LOG_INFO("2<->3 adc_val3:%d | vol_val3:%d\r\n", adc_val3, vol_val3);

        if ((vol_val3 < RES_LOWER_LIMIT_VOLTAGE) || (vol_val3 > RES_UPPER_LIMIT_VOLTAGE)) //待测量电阻过大(>30M)或过小(<7K)
        {
            ResMeasureFlg |= 0x02;
        }
        else
        {
            res_val3 = (vol_val3 - PortGroup_2.PortWithNone.NMOS.Intercept) * (470000.0 + PortGroup_3.PortWith470K.PMOS.Slope) / (3300.0 - PortGroup_3.PortWith470K.PMOS.Intercept - vol_val3)
                       - PortGroup_2.PortWithNone.NMOS.Slope - PM_Probe2Res; //减去表笔2的电阻，和MOS管的电压开销
        }

        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, LOW_LV); //第四次测量：GND---470K-|-x---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV);
        Delay_Ms(50);
        adc_val4 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val4 = PM_ADCRealVol(adc_val4);
        LOG_INFO("2<->3 adc_val4:%d | vol_val4:%d\r\n", adc_val4, vol_val4);

        if ((vol_val4 < RES_LOWER_LIMIT_VOLTAGE) || (vol_val4 > RES_UPPER_LIMIT_VOLTAGE)) //待测量电阻过大(>30M)或过小(<7K)
        {
            ResMeasureFlg |= 0x02;
        }
        else
        {
            res_val4 = (3300.0 - PortGroup_3.PortWithNone.PMOS.Intercept - vol_val4) * (470000.0 + PortGroup_2.PortWith470K.NMOS.Slope) / (vol_val4 - PortGroup_2.PortWith470K.NMOS.Intercept)
                       - PortGroup_3.PortWithNone.PMOS.Slope - PM_Probe3Res; //减去表笔3的电阻，和MOS管的电压开销
        }
    }

    switch (ResMeasureFlg)
    {
        case 0x00: //采用680Ω和470kΩ测量的值
            LOG_INFO("[680+470K] res_val1:%lf | res_val2:%lf | res_val3:%lf | res_val4:%lf\r\n", res_val1, res_val2, res_val3, res_val4);
            ComponentMeasureResult.ComponentParam.ResParam.ResVal = (res_val1 + res_val2 + res_val3 + res_val4) / 4;
            LOG_INFO("ResVal = %lf\r\n", ComponentMeasureResult.ComponentParam.ResParam.ResVal);
            state = SUCCESS;
            break;

        case 0x01: //只采用470kΩ测量的值
            LOG_INFO("[470K] res_val3:%lf | res_val4:%lf\r\n", res_val3, res_val4);
            ComponentMeasureResult.ComponentParam.ResParam.ResVal = (res_val3 + res_val4) / 2;
            LOG_INFO("ResVal = %lf\r\n", ComponentMeasureResult.ComponentParam.ResParam.ResVal);
            state = SUCCESS;
            break;

        case 0x02: //只采用680Ω测量的值
            LOG_INFO("[680] res_val1:%lf | res_val2:%lf\r\n", res_val1, res_val2);
            ComponentMeasureResult.ComponentParam.ResParam.ResVal = (res_val1 + res_val2) / 2;
            LOG_INFO("ResVal = %lf\r\n", ComponentMeasureResult.ComponentParam.ResParam.ResVal);
            state = SUCCESS;
            break;

        default:
            state = FAILURE;
            break;
    }

    return state;
}


/******************************** endfile @ pm_resistance ******************************/
