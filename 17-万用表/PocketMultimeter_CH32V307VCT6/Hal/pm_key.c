/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_key.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        : 
*******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
#include "pm_key.h"


/*******************************************************************************
 * @fn      PM_KeyInit
 *
 * @brief   ����GPIO��ʼ���������ⲿ�ж�
 *
 * @param   none
 *
 * @return  none
 *******************************************************************************/
void PM_KeyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(SW1_PCENR | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = SW1_BV;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
    GPIO_Init(SW1_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SW2_BV;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
    GPIO_Init(SW2_GPIO, &GPIO_InitStructure);

    /* GPIOE ----> EXTI_Line4 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //��ռ���ȼ�2��ֵԽС���ȼ�Խ��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //��Ӧ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/******************************** endfile @ pm_key ******************************/
