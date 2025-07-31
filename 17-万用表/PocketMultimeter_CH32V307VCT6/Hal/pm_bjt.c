/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_bjt.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include "pm_bjt.h"
#include "pm_measure.h"
#include "pm_probe.h"
#include "pm_adc.h"


#define CONFIG_PM_BJT_DEBUG
#ifdef CONFIG_PM_BJT_DEBUG
#define LOG_INFO(...)       printf(__VA_ARGS__)
#else
#define LOG_INFO(...)
#endif


/*******************************************************************************
 * @fn      PM_MeasureBJT
 *
 * @brief   双极结型晶体管检测
 *
 * @param   none
 *
 * @return  FAILURE or SUCCESS
*******************************************************************************/
u8 PM_MeasureBJT(void)
{
    u16 adc_val1, adc_val2, adc_val3, adc_val4;     //ADC采集值
    u32 vol_val1, vol_val2, vol_val3, vol_val4;     //ADC对应的实际电压值
    u16 hFE1, hFE2;                                 //三极管放大倍数
    u8 state = FAILURE;                             //三极管测量状态

    if (ComponentMeasureResult.ComponentParam.BJTParam.Channel == P_CHANNEL) //NPN型，采用共集电极电路进行测量能够保证三级管工作在放大区
    {
        if (ComponentMeasureResult.ComponentParam.BJTParam.B == PortGroup_3.Probe_Num) //PortGroup_3.Probe_Num是基极
        {
            /* step1 */
            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step2 */
            //假设集电极为PortGroup_2.Probe_Num，ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_2.Probe_Num
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV); //选用680Ω，得到的发射极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, HIGH_LV); //选用470K，得到的基极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, HIGH_LV);
            Delay_Ms(50);
            adc_val1 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val1 = PM_ADCRealVol(adc_val1);
            adc_val2 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val2 = PM_ADCRealVol(adc_val2);
            hFE1 = (((vol_val2 - PortGroup_1.PortWith680.NMOS.Intercept) * (470000.0 + PortGroup_3.PortWith470K.PMOS.Slope) / (3300.0 - vol_val1 - PortGroup_3.PortWith470K.PMOS.Intercept) - PortGroup_1.PortWith680.NMOS.Slope) / 680.0 - 1);

            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step3 */
            //假设集电极为PortGroup_1.Probe_Num，ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_1.Probe_Num
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, LOW_LV); //选用680Ω，得到的发射极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, HIGH_LV); //选用470K，得到的基极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, HIGH_LV);
            Delay_Ms(50);
            adc_val3 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val3 = PM_ADCRealVol(adc_val3);
            adc_val4 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val4 = PM_ADCRealVol(adc_val4);
            hFE2 = (((vol_val4 - PortGroup_2.PortWith680.NMOS.Intercept) * (470000.0 + PortGroup_3.PortWith470K.PMOS.Slope) / (3300.0 - vol_val3 - PortGroup_3.PortWith470K.PMOS.Intercept) - PortGroup_2.PortWith680.NMOS.Slope) / 680.0 - 1);

            if (hFE1 > hFE2) //放大倍数大的假设为正确的假设
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_2.Probe_Num;   //集电极
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_1.Probe_Num;   //发射极
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE1;                  //放大倍数
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val1 - vol_val2;   //基极和发射极之间的电压
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_1.Probe_Num;   //集电极
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_2.Probe_Num;   //发射极
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE2;                  //放大倍数
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val3 - vol_val4;   //基极和发射极之间的电压
            }
            state = SUCCESS;
            LOG_INFO("hFE = %d | Ube = %d\n", ComponentMeasureResult.ComponentParam.BJTParam.hFE, ComponentMeasureResult.ComponentParam.BJTParam.Ube);
        }

        else if(ComponentMeasureResult.ComponentParam.BJTParam.B == PortGroup_1.Probe_Num) //PortGroup_1.Probe_Num为基极
        {
            
            /* step1 */
            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step2 */
            //假设集电极为PortGroup_3.Probe_Num，ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_3.Probe_Num
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, LOW_LV); //选用680Ω，得到的发射极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, HIGH_LV); //选用470K，得到的基极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV);
            Delay_Ms(50);
            adc_val1 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val1 = PM_ADCRealVol(adc_val1);
            adc_val2 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val2 = PM_ADCRealVol(adc_val2);
            hFE1 = (((vol_val2 - PortGroup_2.PortWith680.NMOS.Intercept) * (470000.0 + PortGroup_1.PortWith470K.PMOS.Slope) / (3300.0 - vol_val1 - PortGroup_1.PortWith470K.PMOS.Intercept) - PortGroup_2.PortWith680.NMOS.Slope) / 680.0 - 1);

            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step3 */
            //假设集电极为PortGroup_2.Probe_Num，ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_2.Probe_Num
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, LOW_LV);
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, HIGH_LV); //选用470K，得到的基极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, HIGH_LV); //选用680Ω，得到的发射极电压处于电压范围中间，更适合测量
            Delay_Ms(50);
            adc_val3 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val3 = PM_ADCRealVol(adc_val3);
            adc_val4 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val4 = PM_ADCRealVol(adc_val4);
            hFE2 = (((vol_val4 - PortGroup_3.PortWith680.NMOS.Intercept) * (470000.0 + PortGroup_1.PortWith470K.PMOS.Slope) / (3300.0 - vol_val3 - PortGroup_1.PortWith470K.PMOS.Intercept) - PortGroup_3.PortWith680.NMOS.Slope) / 680.0 - 1);

            if (hFE1 > hFE2) //放大倍数大的假设为正确的假设
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_3.Probe_Num;   //集电极
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_2.Probe_Num;   //发射极
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE1;                  //放大倍数
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val1 - vol_val2;   //基极和发射极之间的电压
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_2.Probe_Num;   //集电极
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_3.Probe_Num;   //发射极
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE2;                  //放大倍数
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val3 - vol_val4;   //基极和发射极之间的电压
            }
            state = SUCCESS;
            LOG_INFO("hFE = %d | Ube = %d\n", ComponentMeasureResult.ComponentParam.BJTParam.hFE, ComponentMeasureResult.ComponentParam.BJTParam.Ube);
        }

        else //PortGroup_2.Probe_Num为基极
        {
            /* step1 */
            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step2 */
            //假设集电极为PortGroup_1.Probe_Num，ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_1.Probe_Num
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, LOW_LV); //选用680Ω，得到的发射极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, HIGH_LV); //选用470K，得到的基极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, HIGH_LV);
            Delay_Ms(50);
            adc_val1 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val1 = PM_ADCRealVol(adc_val1);
            adc_val2 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val2 = PM_ADCRealVol(adc_val2);
            hFE1 = (((vol_val2 - PortGroup_3.PortWith680.NMOS.Intercept) * (470000.0 + PortGroup_2.PortWith470K.PMOS.Slope) / (3300.0 - vol_val1 - PortGroup_2.PortWith470K.PMOS.Intercept) - PortGroup_3.PortWith680.NMOS.Slope) / 680.0 - 1);

            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step3 */
            //假设集电极为PortGroup_3.Probe_Num，ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_3.Probe_Num
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV); //选用680Ω，得到的发射极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, HIGH_LV); //选用470K，得到的基极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV);
            Delay_Ms(50);
            adc_val3 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val3 = PM_ADCRealVol(adc_val3);
            adc_val4 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val4 = PM_ADCRealVol(adc_val4);
            hFE2 = (((vol_val4 - PortGroup_1.PortWith680.NMOS.Intercept) * (470000.0 + PortGroup_2.PortWith470K.PMOS.Slope) / (3300.0 - vol_val3 - PortGroup_2.PortWith470K.PMOS.Intercept) - PortGroup_1.PortWith680.NMOS.Slope) / 680.0 - 1);

            if (hFE1 > hFE2) //放大倍数大的假设为正确的假设
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_1.Probe_Num;   //集电极
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_3.Probe_Num;   //发射极
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE1;                  //放大倍数
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val1 - vol_val2;   //基极和发射极之间的电压
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_3.Probe_Num;     //集电极
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_1.Probe_Num;    //发射极
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE2;     //放大倍数
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val3 - vol_val4; //基极和发射极之间的电压
            }
            state = SUCCESS;
            LOG_INFO("hFE = %d | Ube = %d\n", ComponentMeasureResult.ComponentParam.BJTParam.hFE, ComponentMeasureResult.ComponentParam.BJTParam.Ube);
        }
    }

    else if (ComponentMeasureResult.ComponentParam.BJTParam.Channel == N_CHANNEL) //PNP型，采用共集电极电路进行测量能够保证三级管工作在放大区
    {
        if (ComponentMeasureResult.ComponentParam.BJTParam.B == PortGroup_3.Probe_Num) //PortGroup_3.Probe_Num为基极
        {
            /* step1 */
            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step2 */
            //假设集电极为PortGroup_2.Probe_Num，ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_2.Probe_Num
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV);
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, LOW_LV); //选用470K，得到的基极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, HIGH_LV); //选用680Ω，得到的发射极电压处于电压范围中间，更适合测量
            Delay_Ms(50);
            adc_val1 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val1 = PM_ADCRealVol(adc_val1);
            adc_val2 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val2 = PM_ADCRealVol(adc_val2);
            hFE1 = ((3300.0 - vol_val2 - PortGroup_1.PortWith680.PMOS.Intercept) * (470000.0 + PortGroup_3.PortWith470K.NMOS.Slope) / (vol_val1 - PortGroup_3.PortWith470K.NMOS.Intercept) - PortGroup_1.PortWith680.PMOS.Slope) / 680.0 - 1;

            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step3 */
            //假设集电极为PortGroup_1.Probe_Num，ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_1.Probe_Num
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV);
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, LOW_LV); //选用470K，得到的基极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, HIGH_LV); //选用680Ω，得到的发射极电压处于电压范围中间，更适合测量
            Delay_Ms(50);
            adc_val3 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val3 = PM_ADCRealVol(adc_val3);
            adc_val4 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val4 = PM_ADCRealVol(adc_val4);
            hFE2 = ((3300.0 - vol_val4 - PortGroup_2.PortWith680.PMOS.Intercept) * (470000.0 + PortGroup_3.PortWith470K.NMOS.Slope) / (vol_val3 - PortGroup_3.PortWith470K.NMOS.Intercept) - PortGroup_2.PortWith680.PMOS.Slope) / 680.0 - 1;

            if (hFE1 > hFE2) //放大倍数大的假设为正确的假设
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_2.Probe_Num;   //集电极
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_1.Probe_Num;   //发射极
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE1;                  //放大倍数
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val1 - vol_val2;   //基极和发射极之间的电压
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_1.Probe_Num;   //集电极
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_2.Probe_Num;   //发射极
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE2;                  //放大倍数
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val3 - vol_val4;   //基极和发射极之间的电压
            }
            state = SUCCESS;
            LOG_INFO("hFE = %d | Ube = %d\n", ComponentMeasureResult.ComponentParam.BJTParam.hFE, ComponentMeasureResult.ComponentParam.BJTParam.Ube);
        }

        else if (ComponentMeasureResult.ComponentParam.BJTParam.B == PortGroup_1.Probe_Num) //PortGroup_1.Probe_Num为基极
        {
            /* step1 */
            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step2 */
            //假设集电极为PortGroup_3.Probe_Num，ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_3.Probe_Num
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV);
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, LOW_LV); //选用470K，得到的基极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, HIGH_LV); //选用680Ω，得到的发射极电压处于电压范围中间，更适合测量
            Delay_Ms(50);
            adc_val1 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val1 = PM_ADCRealVol(adc_val1);
            adc_val2 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val2 = PM_ADCRealVol(adc_val2);
            hFE1 = ((3300.0 - vol_val2 - PortGroup_2.PortWith680.PMOS.Intercept) * (470000.0 + PortGroup_1.PortWith470K.NMOS.Slope) / (vol_val1 - PortGroup_1.PortWith470K.NMOS.Intercept) - PortGroup_2.PortWith680.PMOS.Slope) / 680.0 - 1;

            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step3 */
            //假设集电极为PortGroup_2.Probe_Num，ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_2.Probe_Num
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV);
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, LOW_LV); //选用470K，得到的基极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, HIGH_LV); //选用680Ω，得到的发射极电压处于电压范围中间，更适合测量
            Delay_Ms(50);
            adc_val3 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val3 = PM_ADCRealVol(adc_val3);
            adc_val4 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val4 = PM_ADCRealVol(adc_val4);
            hFE2 = ((3300.0 - vol_val4 - PortGroup_3.PortWith680.PMOS.Intercept) * (470000.0 + PortGroup_1.PortWith470K.NMOS.Slope) / (vol_val3 - PortGroup_1.PortWith470K.NMOS.Intercept) - PortGroup_3.PortWith680.PMOS.Slope) / 680.0 - 1;

            if (hFE1 > hFE2) //放大倍数大的假设为正确的假设
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_3.Probe_Num;           //集电极
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_2.Probe_Num;      //发射极
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE1;           //放大倍数
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val1 - vol_val2; //基极和发射极之间的电压
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_2.Probe_Num;      //集电极
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_3.Probe_Num;           //发射极
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE2;           //放大倍数
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val3 - vol_val4; //基极和发射极之间的电压
            }
            state = SUCCESS;
            LOG_INFO("hFE = %d | Ube = %d\n", ComponentMeasureResult.ComponentParam.BJTParam.hFE, ComponentMeasureResult.ComponentParam.BJTParam.Ube);
        }

        else //PortGroup_2.Probe_Num为基极
        {
            /* step1 */
            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step2 */
            //假设集电极为PortGroup_1.Probe_Num，ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_1.Probe_Num
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV);
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, LOW_LV); //选用470K，得到的基极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, HIGH_LV); //选用680Ω，得到的发射极电压处于电压范围中间，更适合测量
            Delay_Ms(50);
            adc_val1 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val1 = PM_ADCRealVol(adc_val1);
            adc_val2 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val2 = PM_ADCRealVol(adc_val2);
            hFE1 = ((3300.0 - vol_val2 - PortGroup_3.PortWith680.PMOS.Intercept) * (470000.0 + PortGroup_2.PortWith470K.NMOS.Slope) / (vol_val1 - PortGroup_2.PortWith470K.NMOS.Intercept) - PortGroup_3.PortWith680.PMOS.Slope) / 680.0 - 1;

            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step3 */
            //假设集电极为PortGroup_3.Probe_Num，ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_3.Probe_Num
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV);
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, LOW_LV); //选用470K，得到的基极电压处于电压范围中间，更适合测量
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, HIGH_LV); //选用680Ω，得到的发射极电压处于电压范围中间，更适合测量
            Delay_Ms(50);
            adc_val3 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val3 = PM_ADCRealVol(adc_val3);
            adc_val4 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val4 = PM_ADCRealVol(adc_val4);
            hFE2 = ((3300.0 - vol_val4 - PortGroup_1.PortWith680.PMOS.Intercept) * (470000.0 + PortGroup_2.PortWith470K.NMOS.Slope) / (vol_val3 - PortGroup_2.PortWith470K.NMOS.Intercept) - PortGroup_1.PortWith680.PMOS.Slope) / 680.0 - 1;

            if (hFE1 > hFE2) //放大倍数大的假设为正确的假设
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_1.Probe_Num;          //集电极
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_3.Probe_Num;           //发射极
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE1;           //放大倍数
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val1 - vol_val2; //基极和发射极之间的电压
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_3.Probe_Num;           //集电极
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_1.Probe_Num;          //发射极
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE2;           //放大倍数
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val3 - vol_val4; //基极和发射极之间的电压
            }
            state = SUCCESS;
            LOG_INFO("hFE = %d | Ube = %d\n", ComponentMeasureResult.ComponentParam.BJTParam.hFE, ComponentMeasureResult.ComponentParam.BJTParam.Ube);
        }
    }

    return state;
}


/******************************** endfile @ pm_bjt ******************************/
