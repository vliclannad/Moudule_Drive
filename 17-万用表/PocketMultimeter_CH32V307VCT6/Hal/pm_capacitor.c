/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_capacitor.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
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
 * @brief   �ж������Ƿ�Ϊ����,ͨ���ɼ����ݳ��ͷŵ���̵ĵ���
 *
 * @param   none

 * @return  FAILURE or SUCCESS
*******************************************************************************/
u8 PM_CheckIsCapacitor(void)
{
    u16 adc_val1, adc_val2;     //�ɼ�ADCֵ
    u32 vol_val1, vol_val2;     //ʵ�ʵ�ѹֵmV
    u8 state = FAILURE;         //�����ж�״̬

    /* step1 */
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /* step2 */
    //���ݳ��
    PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV); //PortGroup_1->PortGroup_2
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, LOW_LV);
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, HIGH_LV);
    adc_val1 = PM_GetADC(PortGroup_2.ADC_Channel, ADC_SampleTime_239Cycles5); //�ɼ��¶˵�ƽ
    vol_val1 = PM_ADCRealVol(adc_val1);
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV); //�ӿ����ٶ�
    Delay_Ms(50);

    /* step3 */
    //���ݷŵ�
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, LOW_LV);
    Delay_Us(60); //�ŵ�IO���ڲ��洢�ĵ�
    adc_val2 = PM_GetADC(PortGroup_1.ADC_Channel, ADC_SampleTime_239Cycles5); //�ɼ��϶˵�ƽ
    vol_val2 = PM_ADCRealVol(adc_val2);
    LOG_INFO("1<->2:%d | %d\r\n", vol_val1, vol_val2);

    if ((vol_val1 > CAP_LOWER_LIMIT_VOLTAGE) && (vol_val2 > CAP_LOWER_LIMIT_VOLTAGE)) //���ʱ���¶˲ɼ�����ƽ�����ڳ����������ŵ�ʱ���϶˲ɼ��ĵ�ƽ�����ڷŵ������
    {
        ComponentMeasureResult.ComponentType = COMPONENT_CAPACITANCE;
        ComponentMeasureResult.ComponentParam.CapParam.Front = PortGroup_1.Probe_Num;
        ComponentMeasureResult.ComponentParam.CapParam.Rear = PortGroup_2.Probe_Num;
        state = SUCCESS;
        return state;
    }
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /* step4 */
    //���ݳ��
    PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV); //PortGroup_2->PortGroup_3
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, LOW_LV);
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, HIGH_LV);
    adc_val1 = PM_GetADC(PortGroup_3.ADC_Channel, ADC_SampleTime_239Cycles5); //�ɼ��¶˵�ƽ
    vol_val1 = PM_ADCRealVol(adc_val1);
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV); //�ӿ����ٶ�
    Delay_Ms(50);

    /* step5 */
    //���ݷŵ�
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, LOW_LV);
    Delay_Us(60); //�ŵ�IO���ڲ��洢�ĵ�
    adc_val2 = PM_GetADC(PortGroup_2.ADC_Channel, ADC_SampleTime_239Cycles5); //�ɼ��϶˵�ƽ
    vol_val2 = PM_ADCRealVol(adc_val2);
    LOG_INFO("2<->3:%d | %d\r\n", vol_val1, vol_val2);
    if ((vol_val1 > CAP_LOWER_LIMIT_VOLTAGE) && (vol_val2 > CAP_LOWER_LIMIT_VOLTAGE)) //���ʱ���¶˲ɼ�����ƽ�����ڳ����������ŵ�ʱ���϶˲ɼ��ĵ�ƽ�����ڷŵ������
    {
        ComponentMeasureResult.ComponentType = COMPONENT_CAPACITANCE;
        ComponentMeasureResult.ComponentParam.CapParam.Front = PortGroup_2.Probe_Num;
        ComponentMeasureResult.ComponentParam.CapParam.Rear = PortGroup_3.Probe_Num;
        state = SUCCESS;
        return state;
    }
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /* step6 */
    //���ݳ��
    PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV); //PortGroup_1->PortGroup_3
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, LOW_LV);
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, HIGH_LV);
    adc_val1 = PM_GetADC(PortGroup_3.ADC_Channel, ADC_SampleTime_239Cycles5); //�ɼ��¶˵�ƽ
    vol_val1 = PM_ADCRealVol(adc_val1);
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV); //�ӿ����ٶ�
    Delay_Ms(50);

    /* step7 */
    //���ݷŵ�
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, LOW_LV);
    Delay_Us(60); //�ŵ�IO���ڲ��洢�ĵ�
    adc_val2 = PM_GetADC(PortGroup_1.ADC_Channel, ADC_SampleTime_239Cycles5); //�ɼ��϶˵�ƽ
    vol_val2 = PM_ADCRealVol(adc_val2);
    LOG_INFO("1<->3:%d | %d\r\n", vol_val1, vol_val2);

    if ((vol_val1 > CAP_LOWER_LIMIT_VOLTAGE) && (vol_val2 > CAP_LOWER_LIMIT_VOLTAGE)) //���ʱ���¶˲ɼ�����ƽ�����ڳ����������ŵ�ʱ���϶˲ɼ��ĵ�ƽ�����ڷŵ������
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
 * @brief   ����ֵ����
 *
 * @param   none
 *
 * @return  FAILURE or SUCCESS
*******************************************************************************/
u8 PM_MeasureCapacitor(void)
{
    u16 adc_val;            //�ɼ�ADCֵ
    u32 vol_val;            //ʵ�ʵ�ѹֵmV
    u16 count;              //��ʱ������ֵ
    u32 overflow;           //��ʱ���������ֵ
    u32 time;               //���ʱ��
    u8 state = FAILURE;     //���ݲ���״̬
    u32 i = 0;

    //���ֽӷ������жϲ���
    if ((ComponentMeasureResult.ComponentParam.CapParam.Front == 1) && (ComponentMeasureResult.ComponentParam.CapParam.Rear == 3))
    {
        /* step1 */
        PM_ComponentDischarge();
        Delay_Ms(50);

        /* step2 */
        //���ݳ��
        i = 0;
        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV); //VCC---470K-|-x---GND
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV); //ʹ��470K��������Ƶ�������Ϊ�����ϵͿ��Ժ���IO�������MOS�ܵ�ѹ��
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //������ʹ��

        while (i < CAP_UPPER_LIMIT_CHARGE_TIME) //ÿ��ѭ��Լ77614ns
        {
            adc_val = PM_DMA_GetADCAverage_ForCAP(PortGroup_1.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val > CAP_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //������ʧ��
        PM_TimerCountClear();

        if (i > 0)
        {
            vol_val = PM_ADCRealVol(adc_val);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.CapParam.CapVal = time / (470000.0 * log(3300.0 / (3300.0 - vol_val))); //�������ֵ nF
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
        //���ݳ��
        i = 0;
        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV); //VCC---470K-|-x---GND
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV); //ʹ��470K��������Ƶ�������Ϊ�����ϵͿ��Ժ���IO�������MOS�ܵ�ѹ��
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //������ʹ��

        while (i < CAP_UPPER_LIMIT_CHARGE_TIME) //ÿ��ѭ��Լ77614ns
        {
            adc_val = PM_DMA_GetADCAverage_ForCAP(PortGroup_1.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val > CAP_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //������ʧ��
        PM_TimerCountClear();

        if (i > 0)
        {
            vol_val = PM_ADCRealVol(adc_val);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.CapParam.CapVal = time / (470000.0 * log(3300.0 / (3300.0 - vol_val))); //�������ֵ nF
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
        //���ݳ��
        i = 0;
        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV); //VCC---470K-|-x---GND
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV); //ʹ��470K��������Ƶ�������Ϊ�����ϵͿ��Ժ���IO�������MOS�ܵ�ѹ��
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, HIGH_LV);
        TIM_Cmd(TIM2, ENABLE); //������ʹ��

        while (i < CAP_UPPER_LIMIT_CHARGE_TIME) //ÿ��ѭ��Լ77614ns
        {
            adc_val = PM_DMA_GetADCAverage_ForCAP(PortGroup_2.ADC_Channel, ADC_SampleTime_7Cycles5, &count, &overflow);
            if (adc_val > CAP_UPPER_LIMIT_ADC)
            {
                break;
            }
            ++i;
        }
        TIM_Cmd(TIM2, DISABLE); //������ʧ��
        PM_TimerCountClear();

        if (i > 0)
        {
            vol_val = PM_ADCRealVol(adc_val);
            time = PM_TimerCountTimeNS(count, overflow); //ns
            ComponentMeasureResult.ComponentParam.CapParam.CapVal = time / (470000.0 * log(3300.0 / (3300.0 - vol_val))); //�������ֵ nF
            LOG_INFO("470K | CapVal = %lf\r\n", ComponentMeasureResult.ComponentParam.CapParam.CapVal);
            state = SUCCESS;
        }
    }

    return state;
}


/******************************** endfile @ pm_capacitor ******************************/
