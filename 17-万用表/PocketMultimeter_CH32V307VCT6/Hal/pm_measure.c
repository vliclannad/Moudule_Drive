/********************************** (C) COPYRIGHT *****************************
* File Name          : pm_measure.c
* Author             : WCH
* Version            : V1.0
* Date               : 2021/08/20
* Description        :
*******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
#include "pm_measure.h"
#include "pm_probe.h"
#include "pm_adc.h"
#include "pm_timer.h"


#define CONFIG_PM_MEASURE_DEBUG
#ifdef CONFIG_PM_MEASURE_DEBUG
#define LOG_INFO(...)       printf(__VA_ARGS__)
#else
#define LOG_INFO(...)
#endif


/*******************************************************************************
 *                           GLOBAL VARIABLES
 *******************************************************************************/
/* ��ʲ����� */
#ifdef BOARD_CHITU
/* ������1/���1 */
const PM_MeasurePortGroup PortGroup_1 = { {GPIOA, GPIO_Pin_0, {13.27, -0.6527}, {11.694, 0.0164}},  //PA1_3.3V (�޵���),PMOS:Uds = 22.457 * I - 13.604, NMOS:Uds = 20.427 * I -14.879
                                          {GPIOD, GPIO_Pin_3, {24.766, -1.4316}, {21.52, -0.0433}}, //PD3_5V   (680��),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          {GPIOE, GPIO_Pin_15, {24.766, -1.4316}, {21.52, -0.0433}},//PE15_5V  (470K),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          ADC_Channel_0,                                            //ADCͨ����ADC1
                                          1                                                         //��ʺ�
                                        };
/* ������2/���2 */
const PM_MeasurePortGroup PortGroup_2 = { {GPIOB, GPIO_Pin_0, {13.27, -0.6527}, {11.694, 0.0164}},  //PB0_3.3V (�޵���),PMOS:Uds = 22.457 * I - 13.604, NMOS:Uds = 20.427 * I -14.879
                                          {GPIOE, GPIO_Pin_14, {24.766, -1.4316}, {21.52, -0.0433}},//PE14_5V  (680��),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          {GPIOE, GPIO_Pin_13, {24.766, -1.4316}, {21.52, -0.0433}},//PE13_5V  (470K),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          ADC_Channel_8,                                            //ADCͨ����ADC8
                                          2                                                         //��ʺ�
                                        };
/* ������3/���3 */
const PM_MeasurePortGroup PortGroup_3 = { {GPIOB, GPIO_Pin_1, {13.27, -0.6527}, {11.694, 0.0164}},  //PB1_3.3V (�޵���),PMOS:Uds = 22.457 * I - 13.604, NMOS:Uds = 20.427 * I -14.879
                                          {GPIOD, GPIO_Pin_8, {24.766, -1.4316}, {21.52, -0.0433}}, //PD8_5V   (680��),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          {GPIOD, GPIO_Pin_9, {24.766, -1.4316}, {21.52, -0.0433}}, //PD9_5V   (470K),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          ADC_Channel_9,                                            //ADCͨ����ADC9
                                          3                                                         //��ʺ�
                                        };
#elif defined BOARD_XUELONG
/* ������1/���1 */
const PM_MeasurePortGroup PortGroup_1 = { {GPIOA, GPIO_Pin_4, {13.27, -0.6527}, {11.694, 0.0164}},  //PA4_3.3V (�޵���),PMOS:Uds = 22.457 * I - 13.604, NMOS:Uds = 20.427 * I -14.879
                                          {GPIOA, GPIO_Pin_9, {24.766, -1.4316}, {21.52, -0.0433}}, //PA9_5V   (680��),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          {GPIOA, GPIO_Pin_7, {13.27, -0.6527}, {11.694, 0.0164}},  //PA7_3.3V (470K),PMOS:Uds = 22.457 * I - 13.604, NMOS:Uds = 20.427 * I -14.879
                                          ADC_Channel_4,                                            //ADCͨ����ADC4
                                          1                                                         //��ʺ�
                                        };
/* ������2/���2 */
const PM_MeasurePortGroup PortGroup_2 = { {GPIOA, GPIO_Pin_3, {13.27, -0.6527}, {11.694, 0.0164}},  //PA3_3.3V (�޵���),PMOS:Uds = 22.457 * I - 13.604, NMOS:Uds = 20.427 * I -14.879
                                          {GPIOA, GPIO_Pin_5, {13.27, -0.6527}, {11.694, 0.0164}},  //PA5_3.3V (680��),PMOS:Uds = 22.457 * I - 13.604, NMOS:Uds = 20.427 * I -14.879
                                          {GPIOA, GPIO_Pin_2, {13.27, -0.6527}, {11.694, 0.0164}},  //PA2_3.3V (470K),PMOS:Uds = 22.457 * I - 13.604, NMOS:Uds = 20.427 * I -14.879
                                          ADC_Channel_3,                                            //ADCͨ����ADC3
                                          2                                                         //��ʺ�
                                        };
/* ������3/���3��,,, */
const PM_MeasurePortGroup PortGroup_3 = { {GPIOA, GPIO_Pin_6, {13.27, -0.6527}, {11.694, 0.0164}},  //PA6_3.3V (�޵���),PMOS:Uds = 22.457 * I - 13.604, NMOS:Uds = 20.427 * I -14.879
                                          {GPIOA, GPIO_Pin_8, {24.766, -1.4316}, {21.52, -0.0433}}, //PA8_5V   (680��),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          {GPIOA, GPIO_Pin_10, {24.766, -1.4316}, {21.52, -0.0433}},//PA10_5V  (470K),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          ADC_Channel_6,                                            //ADCͨ����ADC6
                                          3                                                         //��ʺ�
                                        };



#elif defined BOX_CH32V307
/* ������1/���1 */
const PM_MeasurePortGroup PortGroup_1 = { {GPIOA, GPIO_Pin_0, {13.27, -0.6527}, {11.694, 0.0164}},  //PA1_3.3V (�޵���),PMOS:Uds = 22.457 * I - 13.604, NMOS:Uds = 20.427 * I -14.879
                                          {GPIOD, GPIO_Pin_3, {24.766, -1.4316}, {21.52, -0.0433}}, //PD3_5V   (680��),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          {GPIOE, GPIO_Pin_15, {24.766, -1.4316}, {21.52, -0.0433}},//PE15_5V  (470K),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          ADC_Channel_0,                                            //ADCͨ����ADC1
                                          1                                                         //��ʺ�
                                        };
/* ������2/���2 */
const PM_MeasurePortGroup PortGroup_2 = { {GPIOB, GPIO_Pin_0, {13.27, -0.6527}, {11.694, 0.0164}},  //PB0_3.3V (�޵���),PMOS:Uds = 22.457 * I - 13.604, NMOS:Uds = 20.427 * I -14.879
                                          {GPIOE, GPIO_Pin_14, {24.766, -1.4316}, {21.52, -0.0433}},//PE14_5V  (680��),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          {GPIOE, GPIO_Pin_13, {24.766, -1.4316}, {21.52, -0.0433}},//PE13_5V  (470K),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          ADC_Channel_8,                                            //ADCͨ����ADC8
                                          2                                                         //��ʺ�
                                        };
/* ������3/���3 */
const PM_MeasurePortGroup PortGroup_3 = { {GPIOB, GPIO_Pin_1, {13.27, -0.6527}, {11.694, 0.0164}},  //PB1_3.3V (�޵���),PMOS:Uds = 22.457 * I - 13.604, NMOS:Uds = 20.427 * I -14.879
                                          {GPIOD, GPIO_Pin_8, {24.766, -1.4316}, {21.52, -0.0433}}, //PD8_5V   (680��),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          {GPIOD, GPIO_Pin_9, {24.766, -1.4316}, {21.52, -0.0433}}, //PD9_5V   (470K),PMOS:Uds = 25.749 * I - 14.612, NMOS:Uds = 21.082 * I -11.422
                                          ADC_Channel_9,                                            //ADCͨ����ADC9
                                          3                                                         //��ʺ�
                                        };

#endif

/* ��ͬ�����Ĳ������� */
u8 RES_FEA[3]  = {0,0,2};   //���衢����(��δ��ȫ�Ÿɾ�)����С�DMOS������
u8 CAP_FEA[3]  = {0,0,0};   //�����������ݡ�δ���ɶ����ܵ�EMOS������
u8 DIO_FEA[3]  = {0,0,1};   //�����ܺͼ��ɶ����ܵ�EMOS������
u8 BJT_FEA[3]  = {0,1,1};   //BJT������
u8 JFET_FEA[3] = {1,1,2};   //JFET������

/* �������������ٷ����� */
PM_ComponentFeature ComponentFeature = {0};

/* Ԫ��������� */
PM_ComponentMeasureResult ComponentMeasureResult = {0};

/* Ԫ��������ʼ��־ */
volatile u8 MeasureStartFlg = 0;

/* Ԫ���������� */
volatile PM_ComponentMeasureStep MeasureStep = STEP_MEASURE_START;

/* ��ʱ������������� */
volatile u16 TimerOverflowNum = 0;

/* �������ĵ��裨��Ҫ����ʵ��������£� */
float PM_Probe1Res = 0.1; //���1�ĵ���
float PM_Probe2Res = 0.1; //���2�ĵ���
float PM_Probe3Res = 0.1; //���3�ĵ���


/*******************************************************************************
 * @fn      PM_ComponentQuickClassify
 *
 * @brief   Ԫ�������ж����ͣ�ͨ����������ͨ·֮���ͨ�������������ж�����������
 *
 * @param   none
 *
 * @return  ���ٷ��������ο�PM_ComponentMeasureStep
 *******************************************************************************/
u8 PM_ComponentQuickClassify(void)
{
    PM_ComponentFeature fea1, fea2;     //���ٷ�����
    u16 adc_val;                        //�ɼ�ADCֵ
    u32 vol_val;                        //ʵ�ʵ�ѹֵmV
    u8 step = STEP_MEASURE_OVER;        //��һ��Ӧ�ò�ȡ�Ĳ���

    /* step1 */
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /****** �����ж�·���ڵĵ�3����������͵�ƽ ******/
    /* step2 */
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, LOW_LV); //���ѡ��680�������ϴ�ĵ���ʱ680����ķ�ѹ̫С
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, LOW_LV); //ʹ��470K��ѹ��ʱ����Ҫ�ȴ�һ������ADC�ϵĵ��ݳ��
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, HIGH_LV); //PortGroup_1->PortGroup_2
    Delay_Ms(50); //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step3 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //ADC�ɼ�
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("fea1.Path_1to2: %d\r\n", vol_val);
    fea1.Path_1to2 = (vol_val > QUICK_CLASSIFY_VOLTAGE) ? CLOSED : OPEN; //CLOSED:��ͨ,OPEN:����ͨ.100mV�жϵ�ͨ��ֵ���Ե���
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /* step4 */
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, HIGH_LV); //PortGroup_2->PortGroup_1
    Delay_Ms(50);  //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step5 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //ADC�ɼ�
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("fea1.Path_2to1: %d\r\n", vol_val);
    fea1.Path_2to1 = (vol_val > QUICK_CLASSIFY_VOLTAGE) ? CLOSED : OPEN; //CLOSED:��ͨ,OPEN:����ͨ.100mV�жϵ�ͨ��ֵ���Ե���
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /* step6 */
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, HIGH_LV); //PortGroup_2->PortGroup_3
    Delay_Ms(50); //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step7 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //ADC�ɼ�
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("fea1.Path_2to3 = %d\r\n", vol_val);
    fea1.Path_2to3 = (vol_val > QUICK_CLASSIFY_VOLTAGE) ? CLOSED : OPEN; //CLOSED:��ͨ,OPEN:����ͨ.100mV�жϵ�ͨ��ֵ���Ե���
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /* step8 */
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV); //PortGroup_3->PortGroup_2
    Delay_Ms(50); //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step9 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //ADC�ɼ�
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("fea1.Path_3to2 = %d\r\n", vol_val);
    fea1.Path_3to2 = (vol_val > QUICK_CLASSIFY_VOLTAGE) ? CLOSED : OPEN; //CLOSED:��ͨ,OPEN:����ͨ.100mV�жϵ�ͨ��ֵ���Ե���
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /* step10 */
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, HIGH_LV); //PortGroup_1->PortGroup_3
    Delay_Ms(50); //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step11 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //ADC�ɼ�
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("fea1.Path_1to3: %d\r\n", vol_val);
    fea1.Path_1to3 = (vol_val > QUICK_CLASSIFY_VOLTAGE) ? CLOSED : OPEN; //CLOSED:��ͨ,OPEN:����ͨ.100mV�жϵ�ͨ��ֵ���Ե���
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /* step12 */
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV); //PortGroup_3->PortGroup_1
    Delay_Ms(50); //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step13 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //ADC�ɼ�
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("fea1.Path_3to1: %d\r\n", vol_val);
    fea1.Path_3to1 = (vol_val > QUICK_CLASSIFY_VOLTAGE) ? CLOSED : OPEN; //CLOSED:��ͨ,OPEN:����ͨ.100mV�жϵ�ͨ��ֵ���Ե���
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /****** �����ж�·���ڵĵ�3����������ߵ�ƽ ******/
    /* step14 */
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, HIGH_LV);
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, HIGH_LV); //PortGroup_1->PortGroup_2
    Delay_Ms(50); //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step15 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //ADC�ɼ�
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("fea2.Path_1to2: %d\r\n", vol_val);
    fea2.Path_1to2 = (vol_val > QUICK_CLASSIFY_VOLTAGE) ? CLOSED : OPEN; //CLOSED:��ͨ,OPEN:����ͨ.100mV�жϵ�ͨ��ֵ���Ե���
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /* step16 */
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, HIGH_LV);
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, HIGH_LV); //PortGroup_2->PortGroup_1
    Delay_Ms(50); //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step17 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //ADC�ɼ�
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("fea2.Path_2to1: %d\r\n", vol_val);
    fea2.Path_2to1 = (vol_val > QUICK_CLASSIFY_VOLTAGE) ? CLOSED : OPEN; //CLOSED:��ͨ,OPEN:����ͨ.100mV�жϵ�ͨ��ֵ���Ե���
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /* step18 */
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, HIGH_LV);
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, HIGH_LV); //PortGroup_2->PortGroup_3
    Delay_Ms(50); //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step19 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //ADC�ɼ�
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("fea2.Path_2to3 = %d\r\n", vol_val);
    fea2.Path_2to3 = (vol_val > QUICK_CLASSIFY_VOLTAGE) ? CLOSED : OPEN; //CLOSED:��ͨ,OPEN:����ͨ.100mV�жϵ�ͨ��ֵ���Ե���
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /* step20 */
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, HIGH_LV);
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV); //PortGroup_3->PortGroup_2
    Delay_Ms(50); //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step21 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_2.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //ADC�ɼ�
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("fea2.Path_3to2 = %d\r\n", vol_val);
    fea2.Path_3to2 = (vol_val > QUICK_CLASSIFY_VOLTAGE) ? CLOSED : OPEN; //CLOSED:��ͨ,OPEN:����ͨ.100mV�жϵ�ͨ��ֵ���Ե���
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /* step22 */
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, HIGH_LV);
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, HIGH_LV); //PortGroup_1->PortGroup_3
    Delay_Ms(50); //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step23 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_3.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //ADC�ɼ�
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("fea2.Path_1to3: %d\r\n", vol_val);
    fea2.Path_1to3 = (vol_val > QUICK_CLASSIFY_VOLTAGE) ? CLOSED : OPEN; //CLOSED:��ͨ,OPEN:����ͨ.100mV�жϵ�ͨ��ֵ���Ե���
    PM_ComponentDischarge(); //�ŵ�
    Delay_Ms(50);

    /* step24 */
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_680, LOW_LV);
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_680, HIGH_LV);
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, HIGH_LV); //PortGroup_3->PortGroup_1
    Delay_Ms(50); //�ȴ�ADC�ڲ��Ĳ������ݳ��

    /* step25 */
    adc_val = PM_DMA_GetADCAverage(PortGroup_1.ADC_Channel, 16, ADC_SampleTime_239Cycles5); //ADC�ɼ�
    vol_val = PM_ADCRealVol(adc_val);
    LOG_INFO("fea2.Path_3to1: %d\r\n", vol_val);
    fea2.Path_3to1 = (vol_val > QUICK_CLASSIFY_VOLTAGE) ? CLOSED : OPEN; //CLOSED:��ͨ,OPEN:����ͨ.100mV�жϵ�ͨ��ֵ���Ե���

    /****** ͳ��Ԫ������ֵ ******/
    fea1.Feature[0] = fea1.Path_1to2 + fea1.Path_2to1; //��һ�����������ֵ
    fea1.Feature[1] = fea1.Path_2to3 + fea1.Path_3to2;
    fea1.Feature[2] = fea1.Path_1to3 + fea1.Path_3to1;
    fea2.Feature[0] = fea2.Path_1to2 + fea2.Path_2to1; //�ڶ������������ֵ
    fea2.Feature[1] = fea2.Path_2to3 + fea2.Path_3to2;
    fea2.Feature[2] = fea2.Path_1to3 + fea2.Path_3to1;
    LOG_INFO("feature1: %d%d%d\r\n",fea1.Feature[0], fea1.Feature[1], fea1.Feature[2]);
    LOG_INFO("feature1: %d%d%d\r\n",fea2.Feature[0], fea2.Feature[1], fea2.Feature[2]);

    /****** ��ȡԪ����Ч����ֵ ******/
    if(fea1.Feature[0] <= fea2.Feature[0])
    {
        ComponentFeature.Path_1to2 = fea1.Path_1to2;
        ComponentFeature.Path_2to1 = fea1.Path_2to1;
        ComponentFeature.Feature[0] = fea1.Feature[0];
    }
    else
    {
        ComponentFeature.Path_1to2 = fea2.Path_1to2;
        ComponentFeature.Path_2to1 = fea2.Path_2to1;
        ComponentFeature.Feature[0] = fea2.Feature[0];
    }

    if(fea1.Feature[1] <= fea2.Feature[1])
    {
        ComponentFeature.Path_2to3 = fea1.Path_2to3;
        ComponentFeature.Path_3to2 = fea1.Path_3to2;
        ComponentFeature.Feature[1] = fea1.Feature[1];
    }
    else
    {
        ComponentFeature.Path_2to3 = fea2.Path_2to3;
        ComponentFeature.Path_3to2 = fea2.Path_3to2;
        ComponentFeature.Feature[1] = fea2.Feature[1];
    }

    if(fea1.Feature[2] <= fea2.Feature[2])
    {
        ComponentFeature.Path_1to3 = fea1.Path_1to3;
        ComponentFeature.Path_3to1 = fea1.Path_3to1;
        ComponentFeature.Feature[2] = fea1.Feature[2];
    }
    else
    {
        ComponentFeature.Path_1to3 = fea2.Path_1to3;
        ComponentFeature.Path_3to1 = fea2.Path_3to1;
        ComponentFeature.Feature[2] = fea2.Feature[2];
    }
    LOG_INFO("ComponentFeature.Path_1to2:%d | ComponentFeature.Path_2to1:%d\r\n", ComponentFeature.Path_1to2, ComponentFeature.Path_2to1);
    LOG_INFO("ComponentFeature.Path_2to3:%d | ComponentFeature.Path_3to2:%d\r\n", ComponentFeature.Path_2to3, ComponentFeature.Path_3to2);
    LOG_INFO("ComponentFeature.Path_1to3:%d | ComponentFeature.Path_3to1:%d\r\n", ComponentFeature.Path_1to3, ComponentFeature.Path_3to1);
    LOG_INFO("ComponentFeature: %d%d%d\r\n",ComponentFeature.Feature[0], ComponentFeature.Feature[1], ComponentFeature.Feature[2]);

    if(PM_ComponentFeatureCompare(ComponentFeature.Feature, RES_FEA, 3)) //���������ֵ�Ա�
    {
        step = STEP_CHECK_IS_CAP; //�ж��Ƿ�Ϊ����
    }
    else if (PM_ComponentFeatureCompare(ComponentFeature.Feature, CAP_FEA, 3)) //�͵��ݵ�����ֵ�Ա�
    {
        step = STEP_CHECK_IS_CAP; //�ж��Ƿ�Ϊ����
    }
    else if (PM_ComponentFeatureCompare(ComponentFeature.Feature, DIO_FEA, 3)) //������ܵ�����ֵ�Ա�
    {
        step = STEP_MEASURE_DIO; //�����ܲ�������
        ComponentMeasureResult.ComponentType = COMPONENT_DIODE;
        if (ComponentFeature.Feature[0] == 1)
        {
            if (ComponentFeature.Path_2to1 == 1)
            {
                ComponentMeasureResult.ComponentParam.DiodeParam.Positive = PortGroup_2.Probe_Num;
                ComponentMeasureResult.ComponentParam.DiodeParam.Negative = PortGroup_1.Probe_Num;
            }
            else
            {
                ComponentMeasureResult.ComponentParam.DiodeParam.Positive = PortGroup_1.Probe_Num;
                ComponentMeasureResult.ComponentParam.DiodeParam.Negative = PortGroup_2.Probe_Num;
            }
        }
        else if (ComponentFeature.Feature[1] == 1)
        {
            if (ComponentFeature.Path_2to3 == 1)
            {
                ComponentMeasureResult.ComponentParam.DiodeParam.Positive = PortGroup_2.Probe_Num;
                ComponentMeasureResult.ComponentParam.DiodeParam.Negative = PortGroup_3.Probe_Num;
            }
            else
            {
                ComponentMeasureResult.ComponentParam.DiodeParam.Positive = PortGroup_3.Probe_Num;
                ComponentMeasureResult.ComponentParam.DiodeParam.Negative = PortGroup_2.Probe_Num;
            }
        }
        else
        {
            if (ComponentFeature.Path_1to3 == 1)
            {
                ComponentMeasureResult.ComponentParam.DiodeParam.Positive = PortGroup_1.Probe_Num;
                ComponentMeasureResult.ComponentParam.DiodeParam.Negative = PortGroup_3.Probe_Num;
            }
            else
            {
                ComponentMeasureResult.ComponentParam.DiodeParam.Positive = PortGroup_3.Probe_Num;
                ComponentMeasureResult.ComponentParam.DiodeParam.Negative = PortGroup_1.Probe_Num;
            }
        }
    }
    else if (PM_ComponentFeatureCompare(ComponentFeature.Feature, BJT_FEA, 3)) //��BJT������ֵ�Ա�
    {
        step = STEP_MEASURE_BJT; //BJT��������
        ComponentMeasureResult.ComponentType = COMPONENT_BJT;
        if (ComponentFeature.Feature[0] == 0)
        {
            ComponentMeasureResult.ComponentParam.BJTParam.B = PortGroup_3.Probe_Num; //PortGroup_3Ϊ����
            if (ComponentFeature.Path_3to2 == 1)
            {
                ComponentMeasureResult.ComponentParam.BJTParam.Channel = P_CHANNEL; //NPN
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.Channel = N_CHANNEL; //PNP
            }
        }
        else if (ComponentFeature.Feature[1] == 0)
        {
            ComponentMeasureResult.ComponentParam.BJTParam.B = PortGroup_1.Probe_Num; //PortGroup_1Ϊ����
            if (ComponentFeature.Path_1to3 == 1)
            {
                ComponentMeasureResult.ComponentParam.BJTParam.Channel = P_CHANNEL; //NPN
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.Channel = N_CHANNEL; //PNP
            }
        }
        else
        {
            ComponentMeasureResult.ComponentParam.BJTParam.B = PortGroup_2.Probe_Num; //PortGroup_2Ϊ����
            if (ComponentFeature.Path_2to1 == 1)
            {
                ComponentMeasureResult.ComponentParam.BJTParam.Channel = P_CHANNEL; //NPN
            }
            else
            {
                ComponentMeasureResult.ComponentParam.BJTParam.Channel = N_CHANNEL; //PNP
            }
        }
    }

    return step;
}


/*******************************************************************************
 * @fn      PM_ComponentDischarge
 *
 * @brief   Ԫ���ŵ�
 *
 * @param   none
 *
 * @return  none
 *******************************************************************************/
void PM_ComponentDischarge(void)
{
    PM_MeasurePortInit(PortGroup_1, PORT_OUTPUT_WITH_NONE, LOW_LV);
    PM_MeasurePortInit(PortGroup_2, PORT_OUTPUT_WITH_NONE, LOW_LV);
    PM_MeasurePortInit(PortGroup_3, PORT_OUTPUT_WITH_NONE, LOW_LV);
}

/*******************************************************************************
 * @fn      PM_ComponentFeatureCompare
 *
 * @brief   �������ݶԱȣ�������ȣ�������˳��
 *
 * @param   pArr1�����Ա�����
 *          pArr2���Ѵ�С�����������������
 *          len�����鳤��
 *
 * @return  1������������ͬ
 *          0���������ݲ�ͬ
 *******************************************************************************/
u8 PM_ComponentFeatureCompare(u8 *pArr1, u8 *pArr2, u8 len)
{
    u16 i, j;
    u8 temp;
    u8 arr[255];

    for (i = 0; i < len; ++i)
    {
        arr[i] = pArr1[i];
    }

    for (i = len - 1; i > 0; --i) //ð�����򣬴�С����
    {
        for (j = 0; j < i; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

//    LOG_INFO("pArr1: %d%d%d\r\n", pArr1[0], pArr1[1], pArr1[2]);
//    LOG_INFO("arr: %d%d%d\r\n", arr[0], arr[1], arr[2]);
//    LOG_INFO("pArr2: %d%d%d\r\n", pArr2[0], pArr2[1], pArr2[2]);

    for (i = 0; i < len; ++i)
    {
        if (arr[i] != pArr2[i])
            break; //���ζԱȣ�ֻҪ�������������е�ֵ��ͬ���ͽ����Ա�
    }

    if (i == len)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/******************************** endfile @ pm_measure ******************************/
