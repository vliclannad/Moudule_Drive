/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_measure.h
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*******************************************************************************/
#ifndef __PM_MEASURE_H
#define __PM_MEASURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ch32v30x.h"

/*********************************************************************************
 *                                  MACRO
 *********************************************************************************/
/* Component quick classify voltage */
#define QUICK_CLASSIFY_VOLTAGE              ((u32)100) //100mV
#define BOX_CH32V307 


/*********************************************************************************
 *                                  ENUM
 *********************************************************************************/
/* ��·��ͨ״̬ */
typedef enum
{
    OPEN = 0,       //��·
    CLOSED          //��·
}PM_CircuitState;

/* Ԫ�����Խ�� */
typedef enum
{
    FAILURE = 0,    //ʧ��
    SUCCESS         //�ɹ�
}PM_State;

/* ����ܹ������� */
typedef enum
{
    N_CHANNEL = 0,  //N����
    P_CHANNEL       //P����
}PM_TransistorChannelType;

/* Ԫ������ */
typedef enum
{
    COMPONENT_NONE = 0,         //��Ԫ��
    COMPONENT_RESISTANCE,       //����
    COMPONENT_CAPACITANCE,      //����
    COMPONENT_DIODE,            //������
    COMPONENT_BJT,              //˫�����;����BJT
    COMPONENT_JFET,             //JFET
    COMPONENT_EMOS,             //EMOS
    COMPONENT_DMOS,             //DMOS
}PM_ComponentType;

/* Ԫ���������� */
typedef enum
{
    STEP_MEASURE_START = 0,         //������ʼ
    STEP_MEASURE_OVER,              //��������
    STEP_MEASURE_RES,               //��������
    STEP_MEASURE_CAP,               //��������
    STEP_MEASURE_DIO,               //����������
    STEP_MEASURE_BJT,               //����BJT
    STEP_CHECK_IS_CAP,              //����Ƿ�Ϊ����
    STEP_CHECK_IS_RES,              //����Ƿ�Ϊ����
}PM_ComponentMeasureStep;


/*********************************************************************************
 *                                  STRUCT
 *********************************************************************************/
/* GPIO�������ѹ����Ϲ�ʽ   Vol = Slope * Circuit + Intercept*/
typedef struct
{
    float Slope;        //б��
    float Intercept;    //�ؾ�
}IO_VolDropParam;

/* �˿ڶ��� */
typedef struct
{
    GPIO_TypeDef *GPIOx;
    u16 GPIO_Pin;
    IO_VolDropParam PMOS;   //GPIO PMOSѹ����ʽ
    IO_VolDropParam NMOS;   //GPIO NMOSѹ����ʽ
}PM_PortDef;

/* �����˿����(�����������Խ�) */
typedef struct
{
    PM_PortDef PortWithNone;    //δ������Ķ˿ڶ���
    PM_PortDef PortWith680;     //����680������˿ڶ���
    PM_PortDef PortWith470K;    //����470K������˿ڶ���
    u8 ADC_Channel;             //ʹ�õ���ADCͨ��
    u8 Probe_Num;               //��ʺ�
}PM_MeasurePortGroup;

/* ���ٷ����� */
typedef struct
{
    u8 Path_1to2;      //���1�����2
    u8 Path_2to1;      //���2�����1
    u8 Path_2to3;      //���2�����3
    u8 Path_3to2;      //���3�����2
    u8 Path_1to3;      //���1�����3
    u8 Path_3to1;      //���3�����1
    u8 Feature[3];
}PM_ComponentFeature;

/* ������� */
typedef struct
{
    u8 Front;
    u8 Rear;
    double ResVal; //����ֵ,��
}PM_ResParam;

/* ���ݲ��� */
typedef struct
{
    u8 Front;
    u8 Rear;
    double CapVal; //����ֵ,nF
}PM_CapParam;

/* �����ܲ��� */
typedef struct
{
    u8 Positive; //����
    u8 Negative; //����
    u32 Uon;     //��ͨ��ѹ
    double Cj;    //�������
}PM_DiodeParam;

/* BJT���� */
typedef struct
{
    u8 Channel; //����
    u8 C;       //���缫��ͨ��
    u8 B;       //����ͨ��
    u8 E;       //���伫ͨ��
    u16 hFE;
    s32 Ube;
}PM_BJTParam;

/* JFET���� */
typedef struct
{
    u8 Channel; //����
    u8 D;       //©��ͨ��
    u8 G;       //դ��ͨ��
    u8 S;       //Դ��ͨ��
    u32 Uoff;   //�жϵ�ѹ
    double Idss; //����©������
}PM_JFETParam;

/* EMOS���� */
typedef struct
{
    u8 Channel; //����
    u8 D;       //©��ͨ��
    u8 G;       //դ��ͨ��
    u8 S;       //Դ��ͨ��
    u32 Uds;    //©����Դ��֮���ѹ
    u32 Ud;     //�ڲ������ܵĵ�ͨ��ѹ
}PM_EMOSFETParam;

/* DMOS���� */
typedef struct
{
    u8 Channel; //����
    u8 D;       //©��ͨ��
    u8 G;       //դ��ͨ��
    u8 S;       //Դ��ͨ��
    u32 Uoff;   //�жϵ�ѹ
    double Idss; //����©������
    u32 Ud;     //�ڲ������ܵĵ�ͨ��ѹ
}PM_DMOSFETParam;

/* Ԫ�������������� */
typedef union
{
    PM_ResParam         ResParam;       //�������
    PM_CapParam         CapParam;       //���ݲ���
    PM_DiodeParam       DiodeParam;     //�����ܲ���
    PM_BJTParam         BJTParam;       //BJT����
    PM_JFETParam        JFETParam;      //JFET����
    PM_EMOSFETParam     EMOSFETParam;   //EMOS����
    PM_DMOSFETParam     DMOSFETParam;   //DMOS����
}PM_ComponentParameter;

/* Ԫ��������� */
typedef struct
{
    PM_ComponentType        ComponentType;  //Ԫ������
    PM_ComponentParameter   ComponentParam; //Ԫ������
}PM_ComponentMeasureResult;


/*******************************************************************************
 *                           GLOBAL VARIABLES
 *******************************************************************************/
/* Ԫ��������ʼ��־ */
extern volatile u8 MeasureStartFlg;

/* Ԫ���������� */
extern volatile PM_ComponentMeasureStep MeasureStep;

/* ��ʱ������������� */
extern volatile u16 TimerOverflowNum;

/* ��ʲ����� */
extern const PM_MeasurePortGroup PortGroup_1;
extern const PM_MeasurePortGroup PortGroup_2;
extern const PM_MeasurePortGroup PortGroup_3;

/* Ԫ��������� */
extern PM_ComponentMeasureResult ComponentMeasureResult;

/* �������������ٷ����� */
extern PM_ComponentFeature ComponentFeature;

/* �������ĵ��裨��Ҫ����ʵ��������£� */
extern float PM_Probe1Res; //���1�ĵ���
extern float PM_Probe2Res; //���2�ĵ���
extern float PM_Probe3Res; //���3�ĵ���


/********************************************************************************
 *                                   FUNCTIONS
 ********************************************************************************/
u8 PM_ComponentQuickClassify(void);
void PM_ComponentDischarge(void);
u8 PM_ComponentFeatureCompare(u8 *pArr1, u8 *pArr2, u8 len);


/********************************************************************************
*********************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
