/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_resistance.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
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
 * @brief   �ж������Ƿ�Ϊ����,ͨ���ɼ����ݳ��ͷŵ���̵ĵ���
 *
 * @param   none

 * @return  FAILURE or SUCCESS
*******************************************************************************/
u8 PM_CheckIsResistance(void)
{
    u16 adc_val;            //�ɼ�ADCֵ
    u32 vol_val;            //ʵ�ʵ�ѹֵmV
    u8 state = FAILURE;     //�����ж�״̬

    /* step1 */
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /* step2 */
    //���ݳ��
    PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV); //PortGroup_1->PortGroup_2
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, LOW_LV);
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, HIGH_LV);
    Delay_Ms(50); //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step3 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //�ɼ�470K��ѹ
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("1<->2:%d | %d\r\n", vol_val, vol_val);

    if (vol_val > RES_LOWER_LIMIT_VOLTAGE)  //470K�ϴ��ڷ�ѹ
    {
        ComponentMeasureResult.ComponentType = COMPONENT_RESISTANCE;
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
    Delay_Ms(50); //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step5 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //�ɼ�470K��ѹ
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("2<->3:%d | %d\r\n", vol_val, vol_val);

    if (vol_val > RES_LOWER_LIMIT_VOLTAGE)  //470K�ϴ��ڷ�ѹ
    {
        ComponentMeasureResult.ComponentType = COMPONENT_RESISTANCE;
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
    Delay_Ms(50); //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step7 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //�ɼ��¶˵�ƽ
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("1<->3:%d | %d\r\n", vol_val, vol_val);

    if (vol_val > RES_LOWER_LIMIT_VOLTAGE)  //470K�ϴ��ڷ�ѹ
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
 * @brief   ����ֵ����
 *
 * @param   none
 *
 * @return  FAILURE or SUCCESS
*******************************************************************************/
u8 PM_MeasureResistance(void)
{
    u16 adc_val1, adc_val2, adc_val3, adc_val4;     //ADC�ɼ�ֵ
    u32 vol_val1, vol_val2, vol_val3, vol_val4;     //ADC��Ӧ��ʵ�ʵ�ѹֵ
    double res_val1=0, res_val2=0, res_val3=0, res_val4=0;     //����ֵ
    u8 ResMeasureFlg = 0x00;                        //FLAG,�����ж���Щ����ķ�ѹֵ������
    u8 state = FAILURE;

    if ((ComponentMeasureResult.ComponentParam.ResParam.Front == 1) && (ComponentMeasureResult.ComponentParam.ResParam.Rear == 3))
    {
        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV); //��һ�β�����GND---x-|-680---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, HIGH_LV);
        Delay_Ms(50);
        adc_val1 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val1 = PM_ADCRealVol(adc_val1);
        LOG_INFO("1<->3 adc_val1:%d | vol_val1:%d\r\n", adc_val1, vol_val1);

        if (vol_val1 > RES_UPPER_LIMIT_VOLTAGE) //����������(>44K).�������ֵ,���������ϴ�ʱ������680��ѹ����
        {
            ResMeasureFlg |= 0x01;
        }
        else
        {
            res_val1 = (vol_val1 - PortGroup_1.PortWithNone.NMOS.Intercept) * (680.0 + PortGroup_3.PortWith680.PMOS.Slope) / (3300.0 - PortGroup_3.PortWith680.PMOS.Intercept - vol_val1)
                       - PortGroup_1.PortWithNone.NMOS.Slope - PM_Probe1Res; //��ȥ���1�ĵ��裬��MOS�ܵĵ�ѹ����
        }

        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV); //�ڶ��β�����GND---680-|-x---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV);
        Delay_Ms(50);
        adc_val2 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val2 = PM_ADCRealVol(adc_val2);
        LOG_INFO("1<->3 adc_val2:%d | vol_val2:%d\r\n", adc_val2, vol_val2);

        if (vol_val2 < RES_LOWER_LIMIT_VOLTAGE) //����������(>44K)
        {
            ResMeasureFlg |= 0x01;
        }
        else
        {
            res_val2 = (3300.0 - PortGroup_3.PortWithNone.PMOS.Intercept - vol_val2) * (680.0 + PortGroup_1.PortWith680.NMOS.Slope) / (vol_val2 - PortGroup_1.PortWith680.NMOS.Intercept)
                       - PortGroup_3.PortWithNone.PMOS.Slope - PM_Probe3Res; //��ȥ���3�ĵ��裬��MOS�ܵĵ�ѹ����
        }

        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV); //�����β�����GND---x-|-470k---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, HIGH_LV);
        Delay_Ms(50);
        adc_val3 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val3 = PM_ADCRealVol(adc_val3);
        LOG_INFO("1<->3 adc_val3:%d | vol_val3:%d\r\n", adc_val3, vol_val3);

        if ((vol_val3 < RES_LOWER_LIMIT_VOLTAGE) || (vol_val3 > RES_UPPER_LIMIT_VOLTAGE)) //�������������(>30M)���С(<7K)
        {
            ResMeasureFlg |= 0x02;
        }
        else
        {
            res_val3 = (vol_val3 - PortGroup_1.PortWithNone.NMOS.Intercept) * (470000.0 + PortGroup_3.PortWith470K.PMOS.Slope) / (3300.0 - PortGroup_3.PortWith470K.PMOS.Intercept - vol_val3)
                       - PortGroup_1.PortWithNone.NMOS.Slope - PM_Probe1Res; //��ȥ���1�ĵ��裬��MOS�ܵĵ�ѹ����
        }

        PM_MeasurePortInit(PortGroup_2, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, LOW_LV); //���Ĵβ�����GND---470K-|-x---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV);
        Delay_Ms(50);
        adc_val4 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val4 = PM_ADCRealVol(adc_val4);
        LOG_INFO("1<->3 adc_val4:%d | vol_val4:%d\r\n", adc_val4, vol_val4);

        if ((vol_val4 < RES_LOWER_LIMIT_VOLTAGE) || (vol_val4 > RES_UPPER_LIMIT_VOLTAGE)) //�������������(>30M)���С(<7K)
        {
            ResMeasureFlg |= 0x02;
        }
        else
        {

            res_val4 = (3300.0 - PortGroup_3.PortWithNone.PMOS.Intercept - vol_val4) * (470000.0 + PortGroup_1.PortWith470K.NMOS.Slope) / (vol_val4 - PortGroup_1.PortWith470K.NMOS.Intercept)
                       - PortGroup_3.PortWithNone.PMOS.Slope - PM_Probe3Res; //��ȥ���3�ĵ��裬��MOS�ܵĵ�ѹ����
        }
    }
    else if ((ComponentMeasureResult.ComponentParam.ResParam.Front == 1) && (ComponentMeasureResult.ComponentParam.ResParam.Rear == 2))
    {
        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV); //��һ�β�����GND---x-|-680---VCC
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, HIGH_LV);
        Delay_Ms(50);
        adc_val1 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val1 = PM_ADCRealVol(adc_val1);
        LOG_INFO("1<->2 adc_val1:%d | vol_val1:%d\r\n", adc_val1, vol_val1);

        if (vol_val1 > RES_UPPER_LIMIT_VOLTAGE) //����������(>44K).�������ֵ,���������ϴ�ʱ������680��ѹ����
        {
            ResMeasureFlg |= 0x01;
        }
        else
        {
            res_val1 = (vol_val1 - PortGroup_1.PortWithNone.NMOS.Intercept) * (680.0 + PortGroup_2.PortWith680.PMOS.Slope) / (3300.0 - PortGroup_2.PortWith680.PMOS.Intercept - vol_val1)
                       - PortGroup_1.PortWithNone.NMOS.Slope - PM_Probe1Res; //��ȥ���1�ĵ��裬��MOS�ܵĵ�ѹ����
        }

        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV); //�ڶ��β�����GND---680-|-x---VCC
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, HIGH_LV);
        Delay_Ms(50);
        adc_val2 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val2 = PM_ADCRealVol(adc_val2);
        LOG_INFO("1<->2 adc_val2:%d | vol_val2:%d\r\n", adc_val2, vol_val2);

        if (vol_val2 < RES_LOWER_LIMIT_VOLTAGE) //����������(>44K)
        {
            ResMeasureFlg |= 0x01;
        }
        else
        {
            res_val2 = (3300.0 - PortGroup_2.PortWithNone.PMOS.Intercept - vol_val2) * (680.0 + PortGroup_1.PortWith680.NMOS.Slope) / (vol_val2 - PortGroup_1.PortWith680.NMOS.Intercept)
                       - PortGroup_2.PortWithNone.PMOS.Slope - PM_Probe2Res; //��ȥ���2�ĵ��裬��MOS�ܵĵ�ѹ����
        }

        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV); //�����β�����GND---x-|-470k---VCC
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, HIGH_LV);
        Delay_Ms(50);
        adc_val3 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val3 = PM_ADCRealVol(adc_val3);
        LOG_INFO("1<->2 adc_val3:%d | vol_val3:%d\r\n", adc_val3, vol_val3);

        if ((vol_val3 < RES_LOWER_LIMIT_VOLTAGE) || (vol_val3 > RES_UPPER_LIMIT_VOLTAGE)) //�������������(>30M)���С(<7K)
        {
            ResMeasureFlg |= 0x02;
        }
        else
        {
            res_val3 = (vol_val3 - PortGroup_1.PortWithNone.NMOS.Intercept) * (470000.0 + PortGroup_2.PortWith470K.PMOS.Slope) / (3300.0 - PortGroup_2.PortWith470K.PMOS.Intercept - vol_val3)
                       - PortGroup_1.PortWithNone.NMOS.Slope - PM_Probe1Res; //��ȥ���1�ĵ��裬��MOS�ܵĵ�ѹ����
        }

        PM_MeasurePortInit(PortGroup_3, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_470K, LOW_LV); //���Ĵβ�����GND---470K-|-x---VCC
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, HIGH_LV);
        Delay_Ms(50);
        adc_val4 = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val4 = PM_ADCRealVol(adc_val4);
        LOG_INFO("1<->2 adc_val4:%d | vol_val4:%d\r\n", adc_val4, vol_val4);

        if ((vol_val4 < RES_LOWER_LIMIT_VOLTAGE) || (vol_val4 > RES_UPPER_LIMIT_VOLTAGE)) //�������������(>30M)���С(<7K)
        {
            ResMeasureFlg |= 0x02;
        }
        else
        {
            res_val4 = (3300.0 - PortGroup_2.PortWithNone.PMOS.Intercept - vol_val4) * (470000.0 + PortGroup_1.PortWith470K.NMOS.Slope) / (vol_val4 - PortGroup_1.PortWith470K.NMOS.Intercept)
                       - PortGroup_2.PortWithNone.PMOS.Slope - PM_Probe2Res; //��ȥ���2�ĵ��裬��MOS�ܵĵ�ѹ����
        }
    }
    else if ((ComponentMeasureResult.ComponentParam.ResParam.Front == 2) && (ComponentMeasureResult.ComponentParam.ResParam.Rear == 3))
    {
        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV); //��һ�β�����GND---x-|-680---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, HIGH_LV);
        Delay_Ms(50);
        adc_val1 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val1 = PM_ADCRealVol(adc_val1);
        LOG_INFO("2<->3 adc_val1:%d | vol_val1:%d\r\n", adc_val1, vol_val1);

        if (vol_val1 > RES_UPPER_LIMIT_VOLTAGE) //����������(>44K).�������ֵ,���������ϴ�ʱ������680��ѹ����
        {
            ResMeasureFlg |= 0x01;
        }
        else
        {
            res_val1 = (vol_val1 - PortGroup_2.PortWithNone.NMOS.Intercept) * (680.0 + PortGroup_3.PortWith680.PMOS.Slope) / (3300.0 - PortGroup_3.PortWith680.PMOS.Intercept - vol_val1)
                       - PortGroup_2.PortWithNone.NMOS.Slope - PM_Probe2Res; //��ȥ���2�ĵ��裬��MOS�ܵĵ�ѹ����
        }

        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, LOW_LV); //�ڶ��β�����GND---680-|-x---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV);
        Delay_Ms(50);
        adc_val2 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val2 = PM_ADCRealVol(adc_val2);
        LOG_INFO("2<->3 adc_val2:%d | vol_val2:%d\r\n", adc_val2, vol_val2);

        if (vol_val2 < RES_LOWER_LIMIT_VOLTAGE) //����������(>44K)
        {
            ResMeasureFlg |= 0x01;
        }
        else
        {
            res_val2 = (3300.0 - PortGroup_3.PortWithNone.PMOS.Intercept - vol_val2) * (680.0 + PortGroup_2.PortWith680.NMOS.Slope) / (vol_val2 - PortGroup_2.PortWith680.NMOS.Intercept)
                       - PortGroup_3.PortWithNone.PMOS.Slope - PM_Probe3Res; //��ȥ���3�ĵ��裬��MOS�ܵĵ�ѹ����
        }

        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV); //�����β�����GND---x-|-470k---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_470K, HIGH_LV);
        Delay_Ms(50);
        adc_val3 = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val3 = PM_ADCRealVol(adc_val3);
        LOG_INFO("2<->3 adc_val3:%d | vol_val3:%d\r\n", adc_val3, vol_val3);

        if ((vol_val3 < RES_LOWER_LIMIT_VOLTAGE) || (vol_val3 > RES_UPPER_LIMIT_VOLTAGE)) //�������������(>30M)���С(<7K)
        {
            ResMeasureFlg |= 0x02;
        }
        else
        {
            res_val3 = (vol_val3 - PortGroup_2.PortWithNone.NMOS.Intercept) * (470000.0 + PortGroup_3.PortWith470K.PMOS.Slope) / (3300.0 - PortGroup_3.PortWith470K.PMOS.Intercept - vol_val3)
                       - PortGroup_2.PortWithNone.NMOS.Slope - PM_Probe2Res; //��ȥ���2�ĵ��裬��MOS�ܵĵ�ѹ����
        }

        PM_MeasurePortInit(PortGroup_1, PORT_ANALOG_INPUT_ALL, LOW_LV);
        PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_470K, LOW_LV); //���Ĵβ�����GND---470K-|-x---VCC
        PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV);
        Delay_Ms(50);
        adc_val4 = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5);
        vol_val4 = PM_ADCRealVol(adc_val4);
        LOG_INFO("2<->3 adc_val4:%d | vol_val4:%d\r\n", adc_val4, vol_val4);

        if ((vol_val4 < RES_LOWER_LIMIT_VOLTAGE) || (vol_val4 > RES_UPPER_LIMIT_VOLTAGE)) //�������������(>30M)���С(<7K)
        {
            ResMeasureFlg |= 0x02;
        }
        else
        {
            res_val4 = (3300.0 - PortGroup_3.PortWithNone.PMOS.Intercept - vol_val4) * (470000.0 + PortGroup_2.PortWith470K.NMOS.Slope) / (vol_val4 - PortGroup_2.PortWith470K.NMOS.Intercept)
                       - PortGroup_3.PortWithNone.PMOS.Slope - PM_Probe3Res; //��ȥ���3�ĵ��裬��MOS�ܵĵ�ѹ����
        }
    }

    switch (ResMeasureFlg)
    {
        case 0x00: //����680����470k��������ֵ
            LOG_INFO("[680+470K] res_val1:%lf | res_val2:%lf | res_val3:%lf | res_val4:%lf\r\n", res_val1, res_val2, res_val3, res_val4);
            ComponentMeasureResult.ComponentParam.ResParam.ResVal = (res_val1 + res_val2 + res_val3 + res_val4) / 4;
            LOG_INFO("ResVal = %lf\r\n", ComponentMeasureResult.ComponentParam.ResParam.ResVal);
            state = SUCCESS;
            break;

        case 0x01: //ֻ����470k��������ֵ
            LOG_INFO("[470K] res_val3:%lf | res_val4:%lf\r\n", res_val3, res_val4);
            ComponentMeasureResult.ComponentParam.ResParam.ResVal = (res_val3 + res_val4) / 2;
            LOG_INFO("ResVal = %lf\r\n", ComponentMeasureResult.ComponentParam.ResParam.ResVal);
            state = SUCCESS;
            break;

        case 0x02: //ֻ����680��������ֵ
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
