/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_ch9141.c
* Author             : YHR
* Version            : V1.0
* Date               : 2023/06/06
* Description        : 
*******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
#include "pm_ch9141.h"
#include "pm_display.h"
#include "string.h"

#define CONFIG_PM_CH9141_DEBUG
#ifdef CONFIG_PM_CH9141_DEBUG
#define LOG_INFO(...)       printf(__VA_ARGS__)
#else
#define LOG_INFO(...)
#endif


/********************************************************************************
 *                                     MACROS
 *******************************************************************************/
#define DEVICE_NAME_LEN                 13


/*******************************************************************************
 *                           GLOBAL VARIABLES
 *******************************************************************************/
/* CH9141�Ĺ㲥���� */
u8 CH9141_DeviceName[DEVICE_NAME_LEN] = "PM_CHITU_DEMO";

/* CH9141 ATָ�� */
u8 AT_CommandEnd[2] = {0x0D,0xA};
u8 AT_DeviceNameStart[7] = {'A','T','+','N','A','M','E'};
u8 AT_ATMode[5] = {'A','T','.','.','.'};
u8 AT_SuccessRsp[4] = {'O','K',0x0D,0x0A};


/*******************************************************************************
 * @fn      BLE_GPIOInit
 *
 * @brief   ��ʼ��CH9141�Ŀ�������
 *
 * @param   none
 *
 * @return  none
 *******************************************************************************/
void BLE_GPIOInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(BLE_AT_PCENR, ENABLE);

    BLE_AT_DISABLE();
    GPIO_InitStructure.GPIO_Pin = BLE_AT_BV;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(BLE_AT_GPIO, &GPIO_InitStructure);
}


/*******************************************************************************
 * @fn      PM_UARTInit
 *
 * @brief   ����7��ʼ�������ں�CH9141ͨ��
 *
 * @param   baudrate - USART communication baud rate.
 *
 * @return  none
 *******************************************************************************/
void BLE_UARTInit(uint32_t baudrate)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
    USART_Init(UART7, &USART_InitStructure);
    USART_Cmd(UART7, ENABLE);
}

/*******************************************************************************
 * @fn      BLE_ATCommandGenerate
 *
 * @brief   ATָ������
 *
 * @param   cmd - ATָ��
 *          buf - ATָ���λ��
 *
 * @return  ����ָ��ĳ���
 *******************************************************************************/
u8 BLE_ATCommandGenerate(CH9141_AT_COMMAND cmd, u8 *buf)
{
    u8 len = 0;

    switch(cmd)
    {
    case DEVICE_AT_MODE:
        memcpy(buf,  AT_ATMode, 5);
        memcpy(&buf[5],  AT_CommandEnd, 2);
        len = 7;
        break;

    case DEVICE_NAME_READ:
        memcpy(buf,  AT_DeviceNameStart, 7);
        buf[7] = '?';
        memcpy(&buf[8],  AT_CommandEnd, 2);
        len = 10;
        break;

    case DEVICE_NAME_WRITE:
        memcpy(buf,  AT_DeviceNameStart, 7);
        buf[7] = '=';
        memcpy(&buf[8], CH9141_DeviceName, DEVICE_NAME_LEN);
        memcpy(&buf[8+DEVICE_NAME_LEN],  AT_CommandEnd, 2);
        len = 10+DEVICE_NAME_LEN;
        break;

    default:
        break;
    }

    return len;
}

/*******************************************************************************
 * @fn      PM_UARTInit
 *
 * @brief   ����7�����ַ���
 *
 * @param   buf - �ַ�����ָ��
 *          len - �ַ����ĳ���
 *
 * @return  none
 *******************************************************************************/
void PM_UARTSendString(u8 *buf, u16 len)
{
    while(len)
    {
        while(USART_GetFlagStatus(UART7, USART_FLAG_TC) == RESET);
        USART_SendData(UART7, (u16)(*buf++));
        --len;
    }
}


/*******************************************************************************
 * @fn      PM_UARTInit
 *
 * @brief   ����7�����ַ���
 *
 * @param   buf - �ַ�����ָ��
 *          len - �ַ����ĳ���
 *
 * @return  ���ؽ��յ������ݵĳ���
 *******************************************************************************/
u8 PM_UARTReceiveString(u8 *buf)
{
    u8 pre_data = 0;
    u8 len = 0;

    while(1)
    {
        while(USART_GetFlagStatus(UART7, USART_FLAG_RXNE) == RESET);
        *buf = (u8)(USART_ReceiveData(UART7));
        ++len;
        if((pre_data == 0x0D) && (*buf == 0x0A))
        {
            break;
        }
        pre_data = *buf;
        buf++;
    }
    return len;
}


/*******************************************************************************
 * @fn      PM_CH9141Init
 *
 * @brief   Initialize CH9141
 *
 * @param   none
 *
 * @return  none
 *******************************************************************************/
void PM_CH9141Init(void)
{
    u8 at_buf[30];
    u8 len;
    u8 NameFlg = 1;

    BLE_GPIOInit();
    BLE_UARTInit(115200);

    BLE_AT_ENABLE();    //ATģʽʹ��
    Delay_Ms(20);
    len = BLE_ATCommandGenerate(DEVICE_NAME_READ, at_buf);
    LOG_INFO("AT_CMD:");
    for(u8 i=0; i<len; ++i)
    {
        LOG_INFO("%c", at_buf[i]);
    }
//    LOG_INFO("\n");

    USART_ClearFlag(UART7, USART_FLAG_RXNE);
    PM_UARTSendString(at_buf, len);
    len = PM_UARTReceiveString(at_buf);
    LOG_INFO("AT_RSP:");
    for(u8 i=0; i<len; ++i)
    {
        LOG_INFO("%c", at_buf[i]);
    }
//    LOG_INFO("\n");

    if(memcmp(CH9141_DeviceName, at_buf, len-2) != 0)
    {
        LOG_INFO("CH9141 device name has changed\n");
        while(NameFlg)
        {
            len = BLE_ATCommandGenerate(DEVICE_NAME_WRITE, at_buf);
            LOG_INFO("AT_CMD:");
            for(u8 i=0; i<len; ++i)
            {
                LOG_INFO("%c", at_buf[i]);
            }
//            LOG_INFO("\n");

            USART_ClearFlag(UART7, USART_FLAG_RXNE);
            PM_UARTSendString(at_buf, len);
            len = PM_UARTReceiveString(at_buf);
            LOG_INFO("AT_RSP:");
            for(u8 i=0; i<len; ++i)
            {
                LOG_INFO("%c", at_buf[i]);
            }
//            LOG_INFO("\n");

            if (memcmp(at_buf, AT_SuccessRsp, 4) == 0)
            {
                LOG_INFO("CH9141 device name set sucess\n");
                NameFlg = 0;
                PM_DisplayCH9141Reset();
                while(1);
            }
        }
    }
    LOG_INFO("The device name has no change\n");
    BLE_AT_DISABLE();    //ATģʽʧ��
}


/******************************** endfile @ pm_ch9141 ******************************/
