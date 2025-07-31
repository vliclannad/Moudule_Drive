/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_diode.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
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
 * @brief   �����ܼ��
 *
 * @param   none
 *
 * @return  FAILURE or SUCCESS
*******************************************************************************/
u8 PM_MeasureDiode(void)
{
    u16 adc_val1, adc_val2;     //ADC�ɼ�ֵ
    u32 vol_val1, vol_val2;     //ADC��Ӧ��ʵ�ʵ�ѹֵ
    u16 count;                  //��ʱ������ֵ
    u32 overflow;               //��ʱ���������ֵ
    u32 time;                   //���ʱ��
    u8 state = FAILURE;         //�����ܲ���״̬
    u32 i = 0;

    if ((ComponentMeasureResult.ComponentParam.DiodeParam.Positive == 1) && (ComponentMeasureResult.ComponentParam.DiodeParam.Negative == 3)) //�����ܵ�ͨ����Ϊ���1->���3
    {
        /* step1 */
        //���㵼ͨ��ѹ
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
        //���ݳ��
        i = 0;
        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV); //GND---x-|-470K---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //������ʹ��

        while (i < DIO_UPPER_LIMIT_CHARGE_TIME) //ÿ��ѭ��Լ77614ns
        {
            adc_val1 = PM_DMA_GetADCAverage_ForCAP(PortGroup_3.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val1 > DIO_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //������ʧ��
        PM_TimerCountClear();

        if (i > 0)
        {
            vol_val1 = PM_ADCRealVol(adc_val1);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = time / (470000.0 * log(3300.0 / (3300.0 - vol_val1))); //�������ֵ nF
            LOG_INFO("470K | Cj = %lf\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Cj);
        }
        else
        {
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = 0; //��������Ϊ0���߹�С�޷�����
        }
        state = SUCCESS;
    }

    else if ((ComponentMeasureResult.ComponentParam.DiodeParam.Positive == 3) && (ComponentMeasureResult.ComponentParam.DiodeParam.Negative == 1)) //�����ܵ�ͨ����Ϊ���3->���1)
    {
        /* step1 */
        //���㵼ͨ��ѹ
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
        //���ݳ��
        i = 0;
        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV); //VCC---470K-|-x---GND
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //������ʹ��

        while (i < DIO_UPPER_LIMIT_CHARGE_TIME) //ÿ��ѭ��Լ77614ns
        {
            adc_val1 = PM_DMA_GetADCAverage_ForCAP(PortGroup_1.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val1 > DIO_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //������ʧ��
        PM_TimerCountClear();

        if (i > 0)
        {
            vol_val1 = PM_ADCRealVol(adc_val1);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = time / (470000.0 * log(3300.0 / (3300.0 - vol_val1))); //�������ֵ nF
            LOG_INFO("470K | Cj = %lf\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Cj);
        }
        else
        {
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = 0; //��������Ϊ0���߹�С�޷�����
        }
        state = SUCCESS;
    }

    else if ((ComponentMeasureResult.ComponentParam.DiodeParam.Positive == 1) && (ComponentMeasureResult.ComponentParam.DiodeParam.Negative == 2)) //�����ܵ�ͨ����Ϊ���1->���2
    {
        /* step1 */
        //���㵼ͨ��ѹ
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
        //���ݳ��
        i = 0;
        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV); //GND---x-|-470K---VCC
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //������ʹ��

        while (i < DIO_UPPER_LIMIT_CHARGE_TIME) //ÿ��ѭ��Լ77614ns
        {
            adc_val1 = PM_DMA_GetADCAverage_ForCAP(PortGroup_2.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val1 > DIO_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //������ʧ��
        PM_TimerCountClear();
        if (i > 0)
        {
            vol_val1 = PM_ADCRealVol(adc_val1);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = time / (470000.0 * log(3300.0 / (3300.0 - vol_val1))); //�������ֵ nF
            LOG_INFO("470K | Cj = %lf\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Cj);
        }
        else
        {
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = 0; //��������Ϊ0���߹�С�޷�����
        }
        state = SUCCESS;
    }

    else if ((ComponentMeasureResult.ComponentParam.DiodeParam.Positive == 2) && (ComponentMeasureResult.ComponentParam.DiodeParam.Negative == 1)) //�����ܵ�ͨ����Ϊ���2->���1
    {
        /* step1 */
        //���㵼ͨ��ѹ
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
        //���ݳ��
        i = 0;
        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV); //VCC---470K-|-x---GND
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //������ʹ��

        while (i < DIO_UPPER_LIMIT_CHARGE_TIME) //ÿ��ѭ��Լ77614ns
        {
            adc_val1 = PM_DMA_GetADCAverage_ForCAP(PortGroup_1.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val1 > DIO_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //������ʧ��
        PM_TimerCountClear();
        if (i > 0)
        {
            vol_val1 = PM_ADCRealVol(adc_val1);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = time / (470000.0 * log(3300.0 / (3300.0 - vol_val1))); //�������ֵ nF
            LOG_INFO("470K | Cj = %lf\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Cj);
        }
        else
        {
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = 0; //��������Ϊ0���߹�С�޷�����
        }
        state = SUCCESS;
    }

    else if ((ComponentMeasureResult.ComponentParam.DiodeParam.Positive == 2) && (ComponentMeasureResult.ComponentParam.DiodeParam.Negative == 3)) //�����ܵ�ͨ����Ϊ���2->���3
    {
        /* step1 */
        //���㵼ͨ��ѹ
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
        //���ݳ��
        i = 0;
        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV); //GND---x-|-470K---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //������ʹ��

        while (i < DIO_UPPER_LIMIT_CHARGE_TIME) //ÿ��ѭ��Լ77614ns
        {
            adc_val1 = PM_DMA_GetADCAverage_ForCAP(PortGroup_3.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val1 > DIO_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //������ʧ��
        PM_TimerCountClear();
        if (i > 0)
        {
            vol_val1 = PM_ADCRealVol(adc_val1);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = time / (470000.0 * log(3300.0 / (3300.0 - vol_val1))); //�������ֵ nF
            LOG_INFO("470K | Cj = %lf\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Cj);
        }
        else
        {
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = 0; //��������Ϊ0���߹�С�޷�����
        }
        state = SUCCESS;
    }

    else if ((ComponentMeasureResult.ComponentParam.DiodeParam.Positive == 3) && (ComponentMeasureResult.ComponentParam.DiodeParam.Negative == 2)) //�����ܵ�ͨ����Ϊ���3->���2
    {
        /* step1 */
        //���㵼ͨ��ѹ
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
        //���ݳ��
        i = 0;
        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV); //VCC---470K-|-x---GND
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //������ʹ��

        while (i < DIO_UPPER_LIMIT_CHARGE_TIME) //ÿ��ѭ��Լ77614ns
        {
            adc_val1 = PM_DMA_GetADCAverage_ForCAP(PortGroup_2.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val1 > DIO_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //������ʧ��
        PM_TimerCountClear();
        if (i > 0)
        {
            vol_val1 = PM_ADCRealVol(adc_val1);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = time / (470000.0 * log(3300.0 / (3300.0 - vol_val1))); //�������ֵ nF
            LOG_INFO("470K | Cj = %lf\r\n", ComponentMeasureResult.ComponentParam.DiodeParam.Cj);
        }
        else
        {
            ComponentMeasureResult.ComponentParam.DiodeParam.Cj = 0; //��������Ϊ0���߹�С�޷�����
        }
        state = SUCCESS;
    }

    ComponentMeasureResult.ComponentParam.DiodeParam.Cj = 0; //��ʱ����ʾ�����ܵļ�������
    return state;
}


/******************************** endfile @ pm_diode ******************************/
