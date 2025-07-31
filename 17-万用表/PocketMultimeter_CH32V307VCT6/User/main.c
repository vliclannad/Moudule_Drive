/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#include "debug.h"
#include "pm_measure.h"
#include "pm_adc.h"
#include "pm_key.h"
#include "pm_timer.h"
#include "pm_resistance.h"
#include "pm_lcd.h"
#include "pm_capacitor.h"
#include "pm_diode.h"
#include "pm_bjt.h"
#include "pm_display.h"
#include "string.h"
#include "pm_ch9141.h"
#include "ES8388.h"
#include "led.h"
#include "keyboard.h"


#define CONFIG_MAIN_DEBUG
#ifdef CONFIG_MAIN_DEBUG
#define LOG_INFO(format, ...)       printf("<%s:%d> "format, __FILE__, __LINE__, ##__VA_ARGS__)
//#define LOG_INFO(...)       printf(__VA_ARGS__)
#else
#define LOG_INFO(format, ...)
#endif

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(115200);	
    Keyboard_Init(); //按键初始化
    LED_Init(); //LED初始化
    
    ES8388_close(); //关闭ES8388音频芯片
	LOG_INFO("%s\r\n", __TIME__);
	LOG_INFO("SystemClk:%d\r\n", SystemCoreClock);
	LOG_INFO("ChipID:%08x\r\n", DBGMCU_GetCHIPID());

    LED_GREEN_ON(); //绿灯亮

    //PM_LCDInit();       //LCD初始化
   // PM_CH9141Init();    //CH9141初始化
	PM_ADCInit();       //ADC初始化
    //PM_KeyInit();       //按键初始化
    PM_TimerInit(TIMER_COUNT_PERIOD, TIMER_PRESCALER); //定时器初始化
   // PM_DisplayReady();  //设备初始化结束显示

	while(1)
    {
	    if(IS_KEY_PRESSED(KEY_11))
	    {
            printf("KEY_11 detected! 开始测量\r\n");
            
            TIM_Cmd(TIM2, DISABLE);                     //计数器失能
            TIM_SetCounter(TIM2, 0);                    //清计数器值
            TimerOverflowNum = 0;                       //清溢出计数值
            TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除TIM2的中断标志
	        //PM_DisplayTesting();
            
            // 清除KEY_11的按下状态，但保留其他按键状态用于退出检测
            gkeyboard_status.key_pressed &= ~(1 << KEY_11);
            MeasureStep = STEP_MEASURE_START;
            ComponentMeasureResult.ComponentType = COMPONENT_NONE;

	        while(gkeyboard_status.key_pressed == 0) //如果有任何按键被按下，退出循环
	        {
                switch(MeasureStep)
                {
                case STEP_MEASURE_START:
                    LOG_INFO("STEP_MEASURE_START\r\n");
                    MeasureStep = PM_ComponentQuickClassify();
                    break;

                case STEP_CHECK_IS_CAP:
                    LOG_INFO("STEP_CHECK_IS_CAP\r\n");
                    if (PM_CheckIsCapacitor() == SUCCESS)
                    {
                        MeasureStep = STEP_MEASURE_CAP;
                    }
                    else
                    {
                        if(ComponentFeature.Feature[0] == 2)
                        {
                            MeasureStep = STEP_MEASURE_RES; //电阻参数测量
                            ComponentMeasureResult.ComponentType = COMPONENT_RESISTANCE;
                            ComponentMeasureResult.ComponentParam.ResParam.Front = PortGroup_1.Probe_Num; //无极性，默认以标号小的放在前面
                            ComponentMeasureResult.ComponentParam.ResParam.Rear = PortGroup_2.Probe_Num;
                        }
                        else if(ComponentFeature.Feature[1] == 2)
                        {
                            MeasureStep = STEP_MEASURE_RES; //电阻参数测量
                            ComponentMeasureResult.ComponentType = COMPONENT_RESISTANCE;
                            ComponentMeasureResult.ComponentParam.ResParam.Front = PortGroup_2.Probe_Num; //无极性，默认以标号小的放在前面
                            ComponentMeasureResult.ComponentParam.ResParam.Rear = PortGroup_3.Probe_Num;
                        }
                        else if(ComponentFeature.Feature[2] == 2)
                        {
                            MeasureStep = STEP_MEASURE_RES; //电阻参数测量
                            ComponentMeasureResult.ComponentType = COMPONENT_RESISTANCE;
                            ComponentMeasureResult.ComponentParam.ResParam.Front = PortGroup_1.Probe_Num; //无极性，默认以标号小的放在前面
                            ComponentMeasureResult.ComponentParam.ResParam.Rear = PortGroup_3.Probe_Num;
                        }
                        else
                        {
                            MeasureStep = STEP_CHECK_IS_RES; //判断是否为电阻
                        }
                    }
                    break;

                case STEP_MEASURE_CAP:
                    LOG_INFO("STEP_MEASURE_CAP\r\n");
                    if (PM_MeasureCapacitor() != SUCCESS)
                    {
                        ComponentMeasureResult.ComponentType = COMPONENT_NONE;
                    }
                    MeasureStep = STEP_MEASURE_OVER;
                    break;

                case STEP_CHECK_IS_RES:
                    LOG_INFO("STEP_CHECK_IS_RES\r\n");
                    if (PM_CheckIsResistance() == SUCCESS)
                    {
                        MeasureStep = STEP_MEASURE_RES;
                    }
                    else
                    {
                        MeasureStep = STEP_MEASURE_OVER;
                    }
                    break;

                case STEP_MEASURE_RES:
                    LOG_INFO("STEP_MEASURE_RES\r\n");
                    if (PM_MeasureResistance() != SUCCESS)
                    {
                        ComponentMeasureResult.ComponentType = COMPONENT_NONE;
                    }
                    MeasureStep = STEP_MEASURE_OVER;
                    break;

                case STEP_MEASURE_DIO:
                    LOG_INFO("STEP_MEASURE_DIO\r\n");
                    if (PM_MeasureDiode()!= SUCCESS)
                    {
                        ComponentMeasureResult.ComponentType = COMPONENT_NONE;
                    }
                    MeasureStep = STEP_MEASURE_OVER;
                    break;

                case STEP_MEASURE_BJT:
                    LOG_INFO("STEP_MEASURE_BJT\r\n");
                    if (PM_MeasureBJT()!= SUCCESS)
                    {
                        ComponentMeasureResult.ComponentType = COMPONENT_NONE;
                    }
                    MeasureStep = STEP_MEASURE_OVER;
                    break;

                default:
                    LOG_INFO("Error\r\n");
                    ComponentMeasureResult.ComponentType = COMPONENT_NONE;
                    MeasureStep = STEP_MEASURE_OVER;
                    break;
                }

                if(MeasureStep == STEP_MEASURE_OVER) //如果测量结束，退出循环
                {
                    LOG_INFO("STEP_MEASURE_OVER\r\n");
                    switch (ComponentMeasureResult.ComponentType)
                    {
                    case COMPONENT_RESISTANCE:
                        //PM_DisplayResistance(&ComponentMeasureResult);
                        printf("Resistance: Front=%d, Rear=%d\r\n",
                               ComponentMeasureResult.ComponentParam.ResParam.Front,
                               ComponentMeasureResult.ComponentParam.ResParam.Rear);
                        break;
                    case COMPONENT_CAPACITANCE:
                        //PM_DisplayCapacitor(&ComponentMeasureResult);
                        printf("Capacitance: Front=%d, Rear=%d\r\n",
                               ComponentMeasureResult.ComponentParam.CapParam.Front,
                               ComponentMeasureResult.ComponentParam.CapParam.Rear);
                        
                        break;
                    case COMPONENT_DIODE:
                        //PM_DisplayDiode(&ComponentMeasureResult);
                        printf("Diode: Positive=%d, Negative=%d\r\n",
                               ComponentMeasureResult.ComponentParam.DiodeParam.Positive,
                               ComponentMeasureResult.ComponentParam.DiodeParam.Negative);
                        break;
                    case COMPONENT_BJT:
                        //PM_DisplayBJT(&ComponentMeasureResult);
                        printf("BJT: Ube=%d\r\n",
                               ComponentMeasureResult.ComponentParam.BJTParam.Ube);
                        break;
                    default:
                        //PM_DisplayError();
                        printf("Error: Unsupported component type %d\r\n", ComponentMeasureResult.ComponentType);
                        break;
                    }
                    break;
                }
	        }
	    }
        
        // 清除所有按键状态，准备下一次检测
        Keyboard_ClearAllStatus();
        Delay_Ms(100); //防止按键抖动
	}
    printf("Exiting main loop\r\n");

}
