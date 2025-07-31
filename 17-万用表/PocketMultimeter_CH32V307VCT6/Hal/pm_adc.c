/********************************** (C) COPYRIGHT *****************************
* File Name          : pm_adc.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
#include "pm_adc.h"


/******************************************************************************
 *                             GLOBAL VARIABLES
 ******************************************************************************/
s16 Calibrattion_Val = 0;           //ADCУ׼ֵ
u16 ADC_Buf[ADC_BUF_MAX_LEN];       //ADC�ɼ�ֵ������


/*******************************************************************************
 * @fn      PM_ADCInit
 *
 * @brief   ADC1��ʼ����GPIOͨ������������ʼ��Ϊģ�����룬DMA1��ʼ��
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
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);                                       //ADCʱ�Ӳ��ܳ���14M,���÷�Ƶ����Ϊ8,96M/8=12M

    ADC_DeInit(ADC1);                                                       //��λADC1,������ ADC1��ȫ���Ĵ�������Ϊȱʡֵ
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                      //ADC1�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                           //�ر�ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                     //�ر�����ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;     //ת��������������ⲿ��������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                  //ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;                                 //����ת����ADCͨ������Ŀ
    ADC_Init(ADC1, &ADC_InitStructure);                                     //ADC1��ʼ��

    ADC_Cmd(ADC1, ENABLE);      //ʹ��ADC1

    ADC_BufferCmd(ADC1, DISABLE);                           //ʹ��У׼ǰҪ�ر�Buffer
    ADC_ResetCalibration(ADC1);                             //ʹ�ܸ�λУ׼
    while(ADC_GetResetCalibrationStatus(ADC1));             //�ȴ���λУ׼����
    ADC_StartCalibration(ADC1);                             //����ADУ׼
    while(ADC_GetCalibrationStatus(ADC1));                  //�ȴ�У׼����
    Calibrattion_Val = Get_CalibrationValue(ADC1);          //��ȡADC1У׼ֵ

    DMA_DeInit(DMA1_Channel1);                                                  //��λDMA Channel1
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->RDATAR;              //��ADC�Ĺ������ݼĴ�����ȡ����
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_Buf;                        //ADC���ݽ��ջ�����
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //����Դ������ADC
    DMA_InitStructure.DMA_BufferSize = ADC_BUF_MAX_LEN;                         //�������Ĵ�СΪ32
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //ADC����Դ�ĵ�ַ��Ҫ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //�������ĵ�ַҪ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�������ݴ�СΪ����
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //���������ݴ�СΪ����
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               //DMA�洢ģʽ��Ҫѭ��
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                     //ͨ�����ȼ�Ϊ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //���ǻ�������������
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
 * @brief   ��ȡADC(���βɼ�)
 *
 * @param   ch - ����ͨ��ѡ��
 *          sample_time - ����ʱ��ѡ��
 *
 * @return  ADC��ֵ
 *******************************************************************************/
u16 PM_GetADC(u8 ch, u8 sample_time)
{
    u16 val;

    ADC_RegularChannelConfig(ADC1, ch, 1, sample_time);     //����ָ��ADC�Ĺ�����ͨ����һ������
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);                      //���EOC��־�Է�ǰһ��ת��δ����ȡ
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                 //ʹ��ָ����ADC1�����ת����������
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));          //�ȴ�ת������
    val = ADC_GetConversionValue(ADC1);                     //ADC1�������ת�����
    val = PM_ADCCalibrattion((s16)val);                     //��ֵУ׼

    return val;
}


/*******************************************************************************
 * @fn      PM_GetADCAverage
 *
 * @brief   ��ȡADC(���ȡƽ��ֵ)
 *
 * @param   ch - ����ͨ��ѡ��
 *          times - �ɼ�����
 *          sample_time - ����ʱ��ѡ��
 *
 * @return  (u16)ADC��ֵ
 *******************************************************************************/
u16 PM_GetADCAverage(u8 ch, u8 times, u8 sample_time)
{
    u32 val_sum = 0;
    u16 val = 0;
    u8 i;

    PM_GetADC(ch, ADC_SampleTime_239Cycles5);               //��һ��ת��ֵ����
    Delay_Us(50);

    ADC_RegularChannelConfig(ADC1, ch, 1, sample_time);     //����ָ��ADC�Ĺ�����ͨ����һ������
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);                      //���EOC��־�Է�ǰһ��ת��δ����ȡ
    for(i=0; i<times; ++i)
    {
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);             //ʹ��ָ����ADC1�����ת����������
        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));      //�ȴ�ת������
        val_sum += ADC_GetConversionValue(ADC1);            //ADC1�������ת�����
        Delay_Us(50);
    }

    val = (val_sum * 10 / times + 5) / 10;                  //����ƽ��ֵ����������
    val = PM_ADCCalibrattion((s16)val);                     //��ֵУ׼

    return val;
}


/*******************************************************************************
 * @fn      PM_DMA_GetADCAverage
 *
 * @brief   ��ȡADC(���ȡƽ��ֵ)��ʹ��DMA
 *
 * @param   ch - ����ͨ��ѡ��
 *          times - �ɼ�����
 *          sample_time - ����ʱ��ѡ��
 *
 * @return  (u16)ADC��ֵ
 *******************************************************************************/
u16 PM_DMA_GetADCAverage(u8 ch, u8 times, u8 sample_time)
{
    u32 val_sum = 0;
    u16 val = 0;
    u8 i;

    PM_GetADC(ch, ADC_SampleTime_239Cycles5);               //��һ��ת��ֵ����
    Delay_Us(50);

    if(times > ADC_BUF_MAX_LEN) //�ɼ��������ܳ���DMA�������Ĵ�С
    {
        times = ADC_BUF_MAX_LEN;
    }
    DMA_SetCurrDataCounter(DMA1_Channel1, times);           //����DMA�������ݵĸ���,DMA��ʹ�ܲ��ܹ�����
    DMA_Cmd( DMA1_Channel1, ENABLE );                       //ʹ��DMA
    ADC_RegularChannelConfig(ADC1, ch, 1, sample_time );    //����ָ��ADC�Ĺ�����ͨ����һ������
    PM_ADCContinuousConvModeCmd(ADC1, ENABLE);              //����ADC������ת��ģʽ
    ADC_DMACmd(ADC1, ENABLE);                               //ʹ��DMAģʽ
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                 //�������ADCת��

    while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);       //�ȴ�DMA��������
    DMA_Cmd( DMA1_Channel1, DISABLE );                      //ʧ��DMA
    DMA_ClearFlag(DMA1_FLAG_TC1);                           //�������������־
    PM_ADCContinuousConvModeCmd(ADC1, DISABLE);             //�ر�ADC������ת��ģʽ,����ADC������ֹͣת��
    ADC_DMACmd(ADC1, DISABLE);                              //�ر�DMAģʽ��������һ��DMA�����յ��������һ��ת����ɵ�����

    for(i=0; i<times; ++i)
    {
        val_sum += ADC_Buf[i];
    }
    val = (val_sum * 10 / times + 5) / 10;                  //����ƽ��ֵ����������
    val = PM_ADCCalibrattion((s16)val);                     //��ֵУ׼

    return val;
}


/*******************************************************************************
 * @fn      PM_DMA_GetADCAverage_ForCAP
 *
 * @brief   ��ȡADC��ʹ��DMA
 *
 * @param   ch - ����ͨ��ѡ��
 *          sample_time - ����ʱ��ѡ��
 *          *p_count - ��ʱ������ֵ
 *          *p_overflow - ��ʱ���������ֵ
 *
 * @return  (u16)ADC��ֵ
 *******************************************************************************/
u16 PM_DMA_GetADCAverage_ForCAP(u8 ch, u8 sample_time, u16 *p_count, u32 *p_overflow)
{
    u16 val = 0;

    PM_GetADC(ch, ADC_SampleTime_239Cycles5);               //��һ��ת��ֵ����
    Delay_Us(50);

    DMA_SetCurrDataCounter(DMA1_Channel1, 1);               //����DMA�������ݵĸ���,DMA��ʹ�ܲ��ܹ�����
    DMA_Cmd( DMA1_Channel1, ENABLE );                       //ʹ��DMA
    ADC_RegularChannelConfig(ADC1, ch, 1, sample_time );    //����ָ��ADC�Ĺ�����ͨ����һ������
    PM_ADCContinuousConvModeCmd(ADC1, ENABLE);              //����ADC������ת��ģʽ
    ADC_DMACmd(ADC1, ENABLE);                               //ʹ��DMAģʽ
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                 //�������ADCת��
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);       //�ȴ�DMA��������

    *p_overflow = TimerOverflowNum;
    *p_count = TIM_GetCounter(TIM2);
    if(*p_overflow != TimerOverflowNum)
    {
        *p_count = TIM_GetCounter(TIM2);
        *p_overflow = TimerOverflowNum;
    }

    DMA_Cmd( DMA1_Channel1, DISABLE );                      //ʧ��DMA
    DMA_ClearFlag(DMA1_FLAG_TC1);                           //�������������־
    PM_ADCContinuousConvModeCmd(ADC1, DISABLE);             //�ر�ADC������ת��ģʽ,����ADC������ֹͣת��
    ADC_DMACmd(ADC1, DISABLE);                              //�ر�DMAģʽ��������һ��DMA�����յ��������һ��ת����ɵ�����

    val = PM_ADCCalibrattion((s16)ADC_Buf[0]);                     //��ֵУ׼

    return val;
}


/*******************************************************************************
 * @fn      PM_ADCRealVol
 *
 * @brief   ADCʵ�ʵ�ѹֵ����
 *
 * @param   val - ADC��ֵ
 *
 * @return  ʵ�ʵ�ѹֵ,��λΪmV
 *******************************************************************************/
u32 PM_ADCRealVol(u16 val)
{
    return (((((u32) val * 8250) >> 10) + 5) / 10); // val / 4096 * 3300 + 0.5 ��������
}


/******************************** endfile @ pm_adc ******************************/
