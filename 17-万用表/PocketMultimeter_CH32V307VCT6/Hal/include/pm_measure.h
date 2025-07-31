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
/* 电路导通状态 */
typedef enum
{
    OPEN = 0,       //开路
    CLOSED          //闭路
}PM_CircuitState;

/* 元件测试结果 */
typedef enum
{
    FAILURE = 0,    //失败
    SUCCESS         //成功
}PM_State;

/* 晶体管沟道类型 */
typedef enum
{
    N_CHANNEL = 0,  //N沟道
    P_CHANNEL       //P沟道
}PM_TransistorChannelType;

/* 元件类型 */
typedef enum
{
    COMPONENT_NONE = 0,         //无元件
    COMPONENT_RESISTANCE,       //电阻
    COMPONENT_CAPACITANCE,      //电容
    COMPONENT_DIODE,            //二极管
    COMPONENT_BJT,              //双极结型晶体管BJT
    COMPONENT_JFET,             //JFET
    COMPONENT_EMOS,             //EMOS
    COMPONENT_DMOS,             //DMOS
}PM_ComponentType;

/* 元件测量步骤 */
typedef enum
{
    STEP_MEASURE_START = 0,         //测量开始
    STEP_MEASURE_OVER,              //测量结束
    STEP_MEASURE_RES,               //测量电阻
    STEP_MEASURE_CAP,               //测量电容
    STEP_MEASURE_DIO,               //测量二极管
    STEP_MEASURE_BJT,               //测量BJT
    STEP_CHECK_IS_CAP,              //检测是否为电容
    STEP_CHECK_IS_RES,              //检测是否为电阻
}PM_ComponentMeasureStep;


/*********************************************************************************
 *                                  STRUCT
 *********************************************************************************/
/* GPIO推挽输出压降拟合公式   Vol = Slope * Circuit + Intercept*/
typedef struct
{
    float Slope;        //斜率
    float Intercept;    //截距
}IO_VolDropParam;

/* 端口定义 */
typedef struct
{
    GPIO_TypeDef *GPIOx;
    u16 GPIO_Pin;
    IO_VolDropParam PMOS;   //GPIO PMOS压降公式
    IO_VolDropParam NMOS;   //GPIO NMOS压降公式
}PM_PortDef;

/* 测量端口组成(包含三个测试脚) */
typedef struct
{
    PM_PortDef PortWithNone;    //未连电阻的端口定义
    PM_PortDef PortWith680;     //连接680Ω电阻端口定义
    PM_PortDef PortWith470K;    //连接470KΩ电阻端口定义
    u8 ADC_Channel;             //使用到的ADC通道
    u8 Probe_Num;               //表笔号
}PM_MeasurePortGroup;

/* 快速分类结果 */
typedef struct
{
    u8 Path_1to2;      //表笔1到表笔2
    u8 Path_2to1;      //表笔2到表笔1
    u8 Path_2to3;      //表笔2到表笔3
    u8 Path_3to2;      //表笔3到表笔2
    u8 Path_1to3;      //表笔1到表笔3
    u8 Path_3to1;      //表笔3到表笔1
    u8 Feature[3];
}PM_ComponentFeature;

/* 电阻参数 */
typedef struct
{
    u8 Front;
    u8 Rear;
    double ResVal; //电阻值,Ω
}PM_ResParam;

/* 电容参数 */
typedef struct
{
    u8 Front;
    u8 Rear;
    double CapVal; //电容值,nF
}PM_CapParam;

/* 二极管参数 */
typedef struct
{
    u8 Positive; //正极
    u8 Negative; //负极
    u32 Uon;     //导通电压
    double Cj;    //反向电容
}PM_DiodeParam;

/* BJT参数 */
typedef struct
{
    u8 Channel; //沟道
    u8 C;       //集电极极通道
    u8 B;       //基极通道
    u8 E;       //发射极通道
    u16 hFE;
    s32 Ube;
}PM_BJTParam;

/* JFET参数 */
typedef struct
{
    u8 Channel; //沟道
    u8 D;       //漏极通道
    u8 G;       //栅极通道
    u8 S;       //源极通道
    u32 Uoff;   //夹断电压
    double Idss; //饱和漏极电流
}PM_JFETParam;

/* EMOS参数 */
typedef struct
{
    u8 Channel; //沟道
    u8 D;       //漏极通道
    u8 G;       //栅极通道
    u8 S;       //源极通道
    u32 Uds;    //漏极与源极之间电压
    u32 Ud;     //内部二极管的导通电压
}PM_EMOSFETParam;

/* DMOS参数 */
typedef struct
{
    u8 Channel; //沟道
    u8 D;       //漏极通道
    u8 G;       //栅极通道
    u8 S;       //源极通道
    u32 Uoff;   //夹断电压
    double Idss; //饱和漏极电流
    u32 Ud;     //内部二极管的导通电压
}PM_DMOSFETParam;

/* 元件参数的联合体 */
typedef union
{
    PM_ResParam         ResParam;       //电阻参数
    PM_CapParam         CapParam;       //电容参数
    PM_DiodeParam       DiodeParam;     //二极管参数
    PM_BJTParam         BJTParam;       //BJT参数
    PM_JFETParam        JFETParam;      //JFET参数
    PM_EMOSFETParam     EMOSFETParam;   //EMOS参数
    PM_DMOSFETParam     DMOSFETParam;   //DMOS参数
}PM_ComponentParameter;

/* 元件测量结果 */
typedef struct
{
    PM_ComponentType        ComponentType;  //元件类型
    PM_ComponentParameter   ComponentParam; //元件参数
}PM_ComponentMeasureResult;


/*******************************************************************************
 *                           GLOBAL VARIABLES
 *******************************************************************************/
/* 元件测量开始标志 */
extern volatile u8 MeasureStartFlg;

/* 元件测量步骤 */
extern volatile PM_ComponentMeasureStep MeasureStep;

/* 定时器计数溢出次数 */
extern volatile u16 TimerOverflowNum;

/* 表笔测试组 */
extern const PM_MeasurePortGroup PortGroup_1;
extern const PM_MeasurePortGroup PortGroup_2;
extern const PM_MeasurePortGroup PortGroup_3;

/* 元件测量结果 */
extern PM_ComponentMeasureResult ComponentMeasureResult;

/* 待测试器件快速分类结果 */
extern PM_ComponentFeature ComponentFeature;

/* 表笔自身的电阻（需要根据实际情况更新） */
extern float PM_Probe1Res; //表笔1的电阻
extern float PM_Probe2Res; //表笔2的电阻
extern float PM_Probe3Res; //表笔3的电阻


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
