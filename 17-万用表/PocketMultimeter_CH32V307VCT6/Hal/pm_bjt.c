/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_bjt.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
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
 * @brief   ˫�����;���ܼ��
 *
 * @param   none
 *
 * @return  FAILURE or SUCCESS
*******************************************************************************/
u8 PM_MeasureBJT(void)
{
    u16 adc_val1, adc_val2, adc_val3, adc_val4;     //ADC�ɼ�ֵ
    u32 vol_val1, vol_val2, vol_val3, vol_val4;     //ADC��Ӧ��ʵ�ʵ�ѹֵ
    u16 hFE1, hFE2;                                 //�����ܷŴ���
    u8 state = FAILURE;                             //�����ܲ���״̬

    if (ComponentMeasureResult.ComponentParam.BJTParam.Channel == P_CHANNEL) //NPN�ͣ����ù����缫��·���в����ܹ���֤�����ܹ����ڷŴ���
    {
        if (ComponentMeasureResult.ComponentParam.BJTParam.B == PortGroup_3.Probe_Num) //PortGroup_3.Probe_Num�ǻ���
        {
            /* step1 */
            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step2 */
            //���輯�缫ΪPortGroup_2.Probe_Num��ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_2.Probe_Num
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV); //ѡ��680�����õ��ķ��伫��ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, HIGH_LV); //ѡ��470K���õ��Ļ�����ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
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
            //���輯�缫ΪPortGroup_1.Probe_Num��ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_1.Probe_Num
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, LOW_LV); //ѡ��680�����õ��ķ��伫��ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, HIGH_LV); //ѡ��470K���õ��Ļ�����ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, HIGH_LV);
            Delay_Ms(50);
            adc_val3 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val3 = PM_ADCRealVol(adc_val3);
            adc_val4 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val4 = PM_ADCRealVol(adc_val4);
            hFE2 = (((vol_val4 - PortGroup_2.PortWith680.NMOS.Intercept) * (470000.0 + PortGroup_3.PortWith470K.PMOS.Slope) / (3300.0 - vol_val3 - PortGroup_3.PortWith470K.PMOS.Intercept) - PortGroup_2.PortWith680.NMOS.Slope) / 680.0 - 1);

            if (hFE1 > hFE2) //�Ŵ�����ļ���Ϊ��ȷ�ļ���
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_2.Probe_Num;   //���缫
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_1.Probe_Num;   //���伫
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE1;                  //�Ŵ���
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val1 - vol_val2;   //�����ͷ��伫֮��ĵ�ѹ
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_1.Probe_Num;   //���缫
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_2.Probe_Num;   //���伫
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE2;                  //�Ŵ���
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val3 - vol_val4;   //�����ͷ��伫֮��ĵ�ѹ
            }
            state = SUCCESS;
            LOG_INFO("hFE = %d | Ube = %d\n", ComponentMeasureResult.ComponentParam.BJTParam.hFE, ComponentMeasureResult.ComponentParam.BJTParam.Ube);
        }

        else if(ComponentMeasureResult.ComponentParam.BJTParam.B == PortGroup_1.Probe_Num) //PortGroup_1.Probe_NumΪ����
        {
            
            /* step1 */
            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step2 */
            //���輯�缫ΪPortGroup_3.Probe_Num��ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_3.Probe_Num
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, LOW_LV); //ѡ��680�����õ��ķ��伫��ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, HIGH_LV); //ѡ��470K���õ��Ļ�����ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
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
            //���輯�缫ΪPortGroup_2.Probe_Num��ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_2.Probe_Num
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, LOW_LV);
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, HIGH_LV); //ѡ��470K���õ��Ļ�����ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, HIGH_LV); //ѡ��680�����õ��ķ��伫��ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            Delay_Ms(50);
            adc_val3 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val3 = PM_ADCRealVol(adc_val3);
            adc_val4 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val4 = PM_ADCRealVol(adc_val4);
            hFE2 = (((vol_val4 - PortGroup_3.PortWith680.NMOS.Intercept) * (470000.0 + PortGroup_1.PortWith470K.PMOS.Slope) / (3300.0 - vol_val3 - PortGroup_1.PortWith470K.PMOS.Intercept) - PortGroup_3.PortWith680.NMOS.Slope) / 680.0 - 1);

            if (hFE1 > hFE2) //�Ŵ�����ļ���Ϊ��ȷ�ļ���
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_3.Probe_Num;   //���缫
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_2.Probe_Num;   //���伫
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE1;                  //�Ŵ���
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val1 - vol_val2;   //�����ͷ��伫֮��ĵ�ѹ
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_2.Probe_Num;   //���缫
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_3.Probe_Num;   //���伫
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE2;                  //�Ŵ���
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val3 - vol_val4;   //�����ͷ��伫֮��ĵ�ѹ
            }
            state = SUCCESS;
            LOG_INFO("hFE = %d | Ube = %d\n", ComponentMeasureResult.ComponentParam.BJTParam.hFE, ComponentMeasureResult.ComponentParam.BJTParam.Ube);
        }

        else //PortGroup_2.Probe_NumΪ����
        {
            /* step1 */
            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step2 */
            //���輯�缫ΪPortGroup_1.Probe_Num��ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_1.Probe_Num
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, LOW_LV); //ѡ��680�����õ��ķ��伫��ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, HIGH_LV); //ѡ��470K���õ��Ļ�����ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
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
            //���輯�缫ΪPortGroup_3.Probe_Num��ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_3.Probe_Num
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV); //ѡ��680�����õ��ķ��伫��ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, HIGH_LV); //ѡ��470K���õ��Ļ�����ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV);
            Delay_Ms(50);
            adc_val3 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val3 = PM_ADCRealVol(adc_val3);
            adc_val4 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val4 = PM_ADCRealVol(adc_val4);
            hFE2 = (((vol_val4 - PortGroup_1.PortWith680.NMOS.Intercept) * (470000.0 + PortGroup_2.PortWith470K.PMOS.Slope) / (3300.0 - vol_val3 - PortGroup_2.PortWith470K.PMOS.Intercept) - PortGroup_1.PortWith680.NMOS.Slope) / 680.0 - 1);

            if (hFE1 > hFE2) //�Ŵ�����ļ���Ϊ��ȷ�ļ���
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_1.Probe_Num;   //���缫
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_3.Probe_Num;   //���伫
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE1;                  //�Ŵ���
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val1 - vol_val2;   //�����ͷ��伫֮��ĵ�ѹ
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_3.Probe_Num;     //���缫
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_1.Probe_Num;    //���伫
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE2;     //�Ŵ���
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val3 - vol_val4; //�����ͷ��伫֮��ĵ�ѹ
            }
            state = SUCCESS;
            LOG_INFO("hFE = %d | Ube = %d\n", ComponentMeasureResult.ComponentParam.BJTParam.hFE, ComponentMeasureResult.ComponentParam.BJTParam.Ube);
        }
    }

    else if (ComponentMeasureResult.ComponentParam.BJTParam.Channel == N_CHANNEL) //PNP�ͣ����ù����缫��·���в����ܹ���֤�����ܹ����ڷŴ���
    {
        if (ComponentMeasureResult.ComponentParam.BJTParam.B == PortGroup_3.Probe_Num) //PortGroup_3.Probe_NumΪ����
        {
            /* step1 */
            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step2 */
            //���輯�缫ΪPortGroup_2.Probe_Num��ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_2.Probe_Num
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV);
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, LOW_LV); //ѡ��470K���õ��Ļ�����ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, HIGH_LV); //ѡ��680�����õ��ķ��伫��ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            Delay_Ms(50);
            adc_val1 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val1 = PM_ADCRealVol(adc_val1);
            adc_val2 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val2 = PM_ADCRealVol(adc_val2);
            hFE1 = ((3300.0 - vol_val2 - PortGroup_1.PortWith680.PMOS.Intercept) * (470000.0 + PortGroup_3.PortWith470K.NMOS.Slope) / (vol_val1 - PortGroup_3.PortWith470K.NMOS.Intercept) - PortGroup_1.PortWith680.PMOS.Slope) / 680.0 - 1;

            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step3 */
            //���輯�缫ΪPortGroup_1.Probe_Num��ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_1.Probe_Num
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV);
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, LOW_LV); //ѡ��470K���õ��Ļ�����ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, HIGH_LV); //ѡ��680�����õ��ķ��伫��ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            Delay_Ms(50);
            adc_val3 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val3 = PM_ADCRealVol(adc_val3);
            adc_val4 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val4 = PM_ADCRealVol(adc_val4);
            hFE2 = ((3300.0 - vol_val4 - PortGroup_2.PortWith680.PMOS.Intercept) * (470000.0 + PortGroup_3.PortWith470K.NMOS.Slope) / (vol_val3 - PortGroup_3.PortWith470K.NMOS.Intercept) - PortGroup_2.PortWith680.PMOS.Slope) / 680.0 - 1;

            if (hFE1 > hFE2) //�Ŵ�����ļ���Ϊ��ȷ�ļ���
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_2.Probe_Num;   //���缫
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_1.Probe_Num;   //���伫
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE1;                  //�Ŵ���
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val1 - vol_val2;   //�����ͷ��伫֮��ĵ�ѹ
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_1.Probe_Num;   //���缫
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_2.Probe_Num;   //���伫
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE2;                  //�Ŵ���
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val3 - vol_val4;   //�����ͷ��伫֮��ĵ�ѹ
            }
            state = SUCCESS;
            LOG_INFO("hFE = %d | Ube = %d\n", ComponentMeasureResult.ComponentParam.BJTParam.hFE, ComponentMeasureResult.ComponentParam.BJTParam.Ube);
        }

        else if (ComponentMeasureResult.ComponentParam.BJTParam.B == PortGroup_1.Probe_Num) //PortGroup_1.Probe_NumΪ����
        {
            /* step1 */
            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step2 */
            //���輯�缫ΪPortGroup_3.Probe_Num��ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_3.Probe_Num
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV);
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, LOW_LV); //ѡ��470K���õ��Ļ�����ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, HIGH_LV); //ѡ��680�����õ��ķ��伫��ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            Delay_Ms(50);
            adc_val1 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val1 = PM_ADCRealVol(adc_val1);
            adc_val2 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val2 = PM_ADCRealVol(adc_val2);
            hFE1 = ((3300.0 - vol_val2 - PortGroup_2.PortWith680.PMOS.Intercept) * (470000.0 + PortGroup_1.PortWith470K.NMOS.Slope) / (vol_val1 - PortGroup_1.PortWith470K.NMOS.Intercept) - PortGroup_2.PortWith680.PMOS.Slope) / 680.0 - 1;

            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step3 */
            //���輯�缫ΪPortGroup_2.Probe_Num��ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_2.Probe_Num
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV);
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, LOW_LV); //ѡ��470K���õ��Ļ�����ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, HIGH_LV); //ѡ��680�����õ��ķ��伫��ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            Delay_Ms(50);
            adc_val3 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val3 = PM_ADCRealVol(adc_val3);
            adc_val4 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val4 = PM_ADCRealVol(adc_val4);
            hFE2 = ((3300.0 - vol_val4 - PortGroup_3.PortWith680.PMOS.Intercept) * (470000.0 + PortGroup_1.PortWith470K.NMOS.Slope) / (vol_val3 - PortGroup_1.PortWith470K.NMOS.Intercept) - PortGroup_3.PortWith680.PMOS.Slope) / 680.0 - 1;

            if (hFE1 > hFE2) //�Ŵ�����ļ���Ϊ��ȷ�ļ���
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_3.Probe_Num;           //���缫
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_2.Probe_Num;      //���伫
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE1;           //�Ŵ���
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val1 - vol_val2; //�����ͷ��伫֮��ĵ�ѹ
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_2.Probe_Num;      //���缫
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_3.Probe_Num;           //���伫
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE2;           //�Ŵ���
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val3 - vol_val4; //�����ͷ��伫֮��ĵ�ѹ
            }
            state = SUCCESS;
            LOG_INFO("hFE = %d | Ube = %d\n", ComponentMeasureResult.ComponentParam.BJTParam.hFE, ComponentMeasureResult.ComponentParam.BJTParam.Ube);
        }

        else //PortGroup_2.Probe_NumΪ����
        {
            /* step1 */
            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step2 */
            //���輯�缫ΪPortGroup_1.Probe_Num��ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_1.Probe_Num
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV);
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, LOW_LV); //ѡ��470K���õ��Ļ�����ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, HIGH_LV); //ѡ��680�����õ��ķ��伫��ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            Delay_Ms(50);
            adc_val1 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val1 = PM_ADCRealVol(adc_val1);
            adc_val2 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val2 = PM_ADCRealVol(adc_val2);
            hFE1 = ((3300.0 - vol_val2 - PortGroup_3.PortWith680.PMOS.Intercept) * (470000.0 + PortGroup_2.PortWith470K.NMOS.Slope) / (vol_val1 - PortGroup_2.PortWith470K.NMOS.Intercept) - PortGroup_3.PortWith680.PMOS.Slope) / 680.0 - 1;

            PM_ComponentDischarge();
            Delay_Ms(50);

            /* step3 */
            //���輯�缫ΪPortGroup_3.Probe_Num��ComponentMeasureResult.ComponentParam.BJTParam.C == PortGroup_3.Probe_Num
            PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV);
            PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, LOW_LV); //ѡ��470K���õ��Ļ�����ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, HIGH_LV); //ѡ��680�����õ��ķ��伫��ѹ���ڵ�ѹ��Χ�м䣬���ʺϲ���
            Delay_Ms(50);
            adc_val3 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val3 = PM_ADCRealVol(adc_val3);
            adc_val4 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
            vol_val4 = PM_ADCRealVol(adc_val4);
            hFE2 = ((3300.0 - vol_val4 - PortGroup_1.PortWith680.PMOS.Intercept) * (470000.0 + PortGroup_2.PortWith470K.NMOS.Slope) / (vol_val3 - PortGroup_2.PortWith470K.NMOS.Intercept) - PortGroup_1.PortWith680.PMOS.Slope) / 680.0 - 1;

            if (hFE1 > hFE2) //�Ŵ�����ļ���Ϊ��ȷ�ļ���
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_1.Probe_Num;          //���缫
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_3.Probe_Num;           //���伫
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE1;           //�Ŵ���
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val1 - vol_val2; //�����ͷ��伫֮��ĵ�ѹ
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.C = PortGroup_3.Probe_Num;           //���缫
                ComponentMeasureResult.ComponentParam.BJTParam.E = PortGroup_1.Probe_Num;          //���伫
                ComponentMeasureResult.ComponentParam.BJTParam.hFE = hFE2;           //�Ŵ���
                ComponentMeasureResult.ComponentParam.BJTParam.Ube = vol_val3 - vol_val4; //�����ͷ��伫֮��ĵ�ѹ
            }
            state = SUCCESS;
            LOG_INFO("hFE = %d | Ube = %d\n", ComponentMeasureResult.ComponentParam.BJTParam.hFE, ComponentMeasureResult.ComponentParam.BJTParam.Ube);
        }
    }

    return state;
}


/******************************** endfile @ pm_bjt ******************************/
