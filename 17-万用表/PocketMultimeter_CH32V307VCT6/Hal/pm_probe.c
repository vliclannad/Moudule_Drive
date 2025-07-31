/********************************** (C) COPYRIGHT *****************************
* File Name          : pm_probe.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        :
*******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include "pm_probe.h"


/*********************************************************************
 * @fn      PM_GPIOInit
 *
 * @brief   GPIOx - where x can be (A..G) to select the GPIO peripheral.
 *
 * @param   GPIO_InitStruct - pointer to a GPIO_InitTypeDef structure that
 *          contains the configuration information for the specified GPIO peripheral.
 *          (为减少执行代码，以库函数为基础做了删减)
 *
 * @return  none
 *********************************************************************/
void PM_GPIOInit(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct)
{
    u32 currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
    u32 tmpreg = 0x00, pinmask = 0x00;

    currentmode = ((u32)GPIO_InitStruct->GPIO_Mode) & ((u32)0x0F);

    if((((u32)GPIO_InitStruct->GPIO_Mode) & ((u32)0x10)) != 0x00)
    {
        currentmode |= (u32)GPIO_InitStruct->GPIO_Speed;
    }

    if(((u32)GPIO_InitStruct->GPIO_Pin & ((u32)0x00FF)) != 0x00)
    {
        tmpreg = GPIOx->CFGLR;

        for(pinpos = 0x00; pinpos < 0x08; pinpos++)
        {
            pos = ((u32)0x01) << pinpos;
            currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;

            if(currentpin == pos)
            {
                pos = pinpos << 2;
                pinmask = ((u32)0x0F) << pos;
                tmpreg &= ~pinmask;
                tmpreg |= (currentmode << pos);
                break;
            }
        }
        GPIOx->CFGLR = tmpreg;
    }
    else
    {
        tmpreg = GPIOx->CFGHR;

        for(pinpos = 0x00; pinpos < 0x08; pinpos++)
        {
            pos = (((u32)0x01) << (pinpos + 0x08));
            currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);

            if(currentpin == pos)
            {
                pos = pinpos << 2;
                pinmask = ((u32)0x0F) << pos;
                tmpreg &= ~pinmask;
                tmpreg |= (currentmode << pos);
                break;
            }
        }
        GPIOx->CFGHR = tmpreg;
    }
}


/*******************************************************************************
 * @fn      PM_GPIOModeCfg
 *
 * @brief   GPIO模式配置
 *
 * @param   port: 端口选择
 *          mode: 输入输出模式
 *
 * @return  None
 *******************************************************************************/
void PM_GPIOModeCfg(PM_PortDef port, GPIOMode_TypeDef mode)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin = port.GPIO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = mode;
    PM_GPIOInit(port.GPIOx, &GPIO_InitStructure);
}


/*******************************************************************************
 * @fn      PM_MeasurePortInit
 *
 * @brief   初始化测试端口模式
 *
 * @param   p_group: 端口选择
 *          mode: 端口模式选择
 *          level: 引脚输出电平设置，端口模式为模拟输入时无意义
 *
 * @return  None
 *******************************************************************************/
void PM_MeasurePortInit(PM_MeasurePortGroup p_group, PM_PortMode mode, PM_PortLevel level)
{
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE); //目前使用到GPIOA,GPIOB,GPIOD,GPIOE

    switch (mode)
    {
        case PORT_OUTPUT_WITH_NONE:
            PM_GPIOModeCfg(p_group.PortWith680, GPIO_Mode_AIN);
            PM_GPIOModeCfg(p_group.PortWith470K, GPIO_Mode_AIN);

            if (level != LOW_LV)
            {
                GPIO_SetBits(p_group.PortWithNone.GPIOx, p_group.PortWithNone.GPIO_Pin);
            }
            else
            {
                GPIO_ResetBits(p_group.PortWithNone.GPIOx, p_group.PortWithNone.GPIO_Pin);
            }
            PM_GPIOModeCfg(p_group.PortWithNone, GPIO_Mode_Out_PP);
            break;

        case PORT_OUTPUT_WITH_680:
            PM_GPIOModeCfg(p_group.PortWithNone, GPIO_Mode_AIN);
            PM_GPIOModeCfg(p_group.PortWith470K, GPIO_Mode_AIN);

            if (level != LOW_LV)
            {
                GPIO_SetBits(p_group.PortWith680.GPIOx, p_group.PortWith680.GPIO_Pin);
            }
            else
            {
                GPIO_ResetBits(p_group.PortWith680.GPIOx, p_group.PortWith680.GPIO_Pin);
            }
            PM_GPIOModeCfg(p_group.PortWith680, GPIO_Mode_Out_PP);
            break;

        case PORT_OUTPUT_WITH_470K:
            PM_GPIOModeCfg(p_group.PortWithNone, GPIO_Mode_AIN);
            PM_GPIOModeCfg(p_group.PortWith680, GPIO_Mode_AIN);
            if (level != LOW_LV)
            {
                GPIO_SetBits(p_group.PortWith470K.GPIOx, p_group.PortWith470K.GPIO_Pin);
            }
            else
            {
                GPIO_ResetBits(p_group.PortWith470K.GPIOx, p_group.PortWith470K.GPIO_Pin);
            }
            PM_GPIOModeCfg(p_group.PortWith470K, GPIO_Mode_Out_PP);
            break;

        case PORT_ANALOG_INPUT_ALL:
            PM_GPIOModeCfg(p_group.PortWithNone, GPIO_Mode_AIN);
            PM_GPIOModeCfg(p_group.PortWith680, GPIO_Mode_AIN);
            PM_GPIOModeCfg(p_group.PortWith470K, GPIO_Mode_AIN);
            break;

        default:
            break;
    }
}


/******************************** endfile @ pm_probe ******************************/
