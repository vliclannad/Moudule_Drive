/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_display.c
* Author             : WCH
* Version            : V1.0
* Date               : 2023/06/06
* Description        : 
*******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include "pm_display.h"
#include "pm_lcd.h"
#include "pm_ch9141.h"
#include "string.h"


/*********************************************************************************
 *                                    MACROS
 *********************************************************************************/
/* BLE通知信息 长度*/
#define CH9141_BUF_LEN                  ((u8)40)

/******************************************************************************
 *                            GLOBAL VARIABLES
 ******************************************************************************/
u8 CH9141_BUF[CH9141_BUF_LEN] = {0};    //蓝牙发送内容


/*******************************************************************************
* Function Name  : BLE_BufClear
* Description    : 清空BLENOTIBUF
* Input          : None
* Return         : None
*******************************************************************************/
void BLE_BufClear(void)
{
    u8 i;

    for (i = 0; i < CH9141_BUF_LEN; ++i)
    {
        CH9141_BUF[i] = 0;
    }
}


/*******************************************************************************
* Function Name  : Ready_Display
* Description    : 显示准备就绪界面
* Input          : None
* Return         : None
*******************************************************************************/
void PM_DisplayCH9141Reset(void)
{
    LCD_ShowString(72, 0, 32, "Pocket");
    LCD_ShowString(40, 32, 32, "Multimeter");
    LCD_DrawLine(0, 63, 249-1, 63);
    LCD_SetColor(BLACK, YELLOW);
    LCD_ShowBWImage(88, 76, 64, 64, WARNING_PIC);
    LCD_Fill(0, 152, 240-1, 240-1, DARKGRAY);
    LCD_SetColor(DARKGRAY, WHITE);
    LCD_ShowString(0, 160, 24, "Please power off CH9141 to update the device name!");
}


/*******************************************************************************
* Function Name  : Ready_Display
* Description    : 显示准备就绪界面
* Input          : None
* Return         : None
*******************************************************************************/
void PM_DisplayReady(void)
{
    LCD_Clear(BLACK);
    LCD_ShowString(72, 0, 32, "Pocket");
    LCD_ShowString(40, 32, 32, "Multimeter");
    LCD_DrawLine(0, 63, 249-1, 63);
    LCD_SetColor(BLACK, GREEN);
    LCD_ShowBWImage(88, 92, 64, 64, READY_PIC);
    LCD_Fill(56, 184, 184-1, 232-1, DARKGRAY);
    LCD_SetColor(DARKGRAY, WHITE);
    LCD_ShowString(72, 192, 32, "Ready!");
}


/*******************************************************************************
* Function Name  : Testing_Display
* Description    : 显示正在测试界面
* Input          : None
* Return         : None
*******************************************************************************/
void PM_DisplayTesting(void)
{
    LCD_Fill(0, 64, 240-1, 240-1, BLACK);
    LCD_SetColor(BLACK, YELLOW);
    LCD_ShowBWImage(88, 92, 64, 64, TESTING_PIC);
    LCD_Fill(24, 184, 216-1, 232-1, DARKGRAY);
    LCD_SetColor(DARKGRAY, WHITE);
    LCD_ShowString(40, 192, 32, "Testing...");
}

/*******************************************************************************
* Function Name  : Error_Report
* Description    : 报错
* Input          : None
* Return         : None
*******************************************************************************/
void PM_DisplayError(void)
{
    LCD_Fill(0, 64, 240-1, 240-1, BLACK);
    LCD_SetColor(BLACK, RED);
    LCD_ShowBWImage(88, 92, 64, 64, ERROR_PIC);
    LCD_Fill(0, 176, 240-1, 240-1, DARKGRAY);
    LCD_SetColor(DARKGRAY, WHITE);
    LCD_ShowString(0, 184, 24, "Unknow, out of range, or damaged part!");

    //APP显示
    BLE_BufClear();
    CH9141_BUF[0] = BLE_BUF_START;
    CH9141_BUF[1] = 0x02;
    CH9141_BUF[2] = COMPONENT_TYPE_NONE;
    CH9141_BUF[3] = BLE_BUF_END;
    PM_UARTSendString(CH9141_BUF, CH9141_BUF_LEN);
}

/*******************************************************************************
* Function Name  : Resistance_Display
* Description    : 电阻参数显示(OLED+BLE)
* Input          : ComParam: 元件参数
* Return         : None
*******************************************************************************/
void PM_DisplayResistance(PM_ComponentMeasureResult *ComParam)
{
    double res_val;
    u8 res_buf[10];
    u8 i, len = 0;
    u8 UnitFlag = 0;

    res_val = ComParam->ComponentParam.ResParam.ResVal;
    if (res_val > 10000000.0) //如果电阻值大于10M,就显示以MΩ为单位的值
    {
        res_val /= 1000000.0;
        UnitFlag = 2; //单位显示MΩ
    }
    else if (res_val > 10000.0) //如果电阻值大于10K,就显示以KΩ为单位的值
    {
        res_val /= 1000.0;
        UnitFlag = 1; //单位显示KΩ
    }
    len = (u8)sprintf((char *)res_buf, "%.1lf", res_val);

    LCD_Fill(0, 64, 240-1, 240-1, BLACK);
    LCD_Fill(0, 64, 240-1, 144-1, DARKGRAY);
    LCD_SetColor(DARKGRAY, WHITE);
    LCD_ShowNum(16, 72, ComParam->ComponentParam.ResParam.Front, 32);
    LCD_ShowBWImage(32, 72, 64, 32, RES_PIC);
    LCD_ShowNum(96, 72, ComParam->ComponentParam.ResParam.Rear, 32);
    LCD_ShowString(192, 72, 32, "[R]");

    for(i=0; i<len; ++i)
    {
        LCD_ShowChar(16+16*i, 104, res_buf[i], 32);
    }

    if(UnitFlag == 2)
    {
        LCD_ShowChar(16+16*len, 104, 'M', 32);
        LCD_ShowBWImage(32+16*len, 104, 32, 32, OHM_UNIT);
    }
    else if(UnitFlag == 1)
    {
        LCD_ShowChar(16+16*len, 104, 'K', 32);
        LCD_ShowBWImage(32+16*len, 104, 32, 32, OHM_UNIT);
    }
    else
    {
        LCD_ShowBWImage(16+16*len, 104, 32, 32, OHM_UNIT);
    }

    //APP显示
    BLE_BufClear();
    CH9141_BUF[0] = BLE_BUF_START;
    CH9141_BUF[1] = 6+len;
    CH9141_BUF[2] = COMPONENT_TYPE_RES;
    CH9141_BUF[3] = ComParam->ComponentParam.ResParam.Front;
    CH9141_BUF[4] = ComParam->ComponentParam.ResParam.Rear;
    CH9141_BUF[5] = 1+len;
    memcpy(&CH9141_BUF[6], res_buf, len);
    if(UnitFlag == 2)
    {
        CH9141_BUF[6+len] = 'M';
    }
    else if(UnitFlag == 1)
    {
        CH9141_BUF[6+len] = 'K';
    }
    else
    {
        CH9141_BUF[6+len] = 'R';
    }
    CH9141_BUF[7+len] = BLE_BUF_END;
    PM_UARTSendString(CH9141_BUF, CH9141_BUF_LEN);
}


/*******************************************************************************
* Function Name  : Resistance_Display
* Description    : 电阻参数显示(OLED+BLE)
* Input          : ComParam: 元件参数
* Return         : None
*******************************************************************************/
void PM_DisplayCapacitor(PM_ComponentMeasureResult *ComParam)
{
    double cap_val;
    u8 cap_buf[10];
    u8 i, len = 0;
    u8 UnitFlag = 0;

    cap_val = ComParam->ComponentParam.CapParam.CapVal;
    if (cap_val > 10000000.0) //如果电阻值大于10mF,就显示以mF为单位的值
    {
        cap_val /= 1000000.0;
        UnitFlag = 2; //单位显示mF
    }
    else if (cap_val > 10000.0) //如果电阻值大于10uF,就显示以uF为单位的值
    {
        cap_val /= 1000.0;
        UnitFlag = 1; //单位显示uF
    }
    len = (u8)sprintf((char *)cap_buf, "%.2lf", cap_val);

    LCD_Fill(0, 64, 240-1, 240-1, BLACK);
    LCD_Fill(0, 64, 240-1, 144-1, DARKGRAY);
    LCD_SetColor(DARKGRAY, WHITE);
    LCD_ShowNum(16, 72, ComParam->ComponentParam.CapParam.Front, 32);
    LCD_ShowBWImage(32, 72, 64, 32, CAP_PIC);
    LCD_ShowNum(96, 72, ComParam->ComponentParam.CapParam.Rear, 32);
    LCD_ShowString(192, 72, 32, "[C]");

    for(i=0; i<len; ++i)
    {
        LCD_ShowChar(16+16*i, 104, cap_buf[i], 32);
    }

    if(UnitFlag == 2)
    {
        LCD_ShowString(16+16*len, 104, 32, "mF");
    }
    else if(UnitFlag == 1)
    {
        LCD_ShowString(16+16*len, 104, 32, "uF");
    }
    else
    {
        LCD_ShowString(16+16*len, 104, 32, "nF");
    }

    //APP显示
    BLE_BufClear();
    CH9141_BUF[0] = BLE_BUF_START;
    CH9141_BUF[1] = 7+len;
    CH9141_BUF[2] = COMPONENT_TYPE_CAP;
    CH9141_BUF[3] = ComParam->ComponentParam.CapParam.Front;
    CH9141_BUF[4] = ComParam->ComponentParam.CapParam.Rear;
    CH9141_BUF[5] = 2+len;
    memcpy(&CH9141_BUF[6], cap_buf, len);
    if(UnitFlag == 2)
    {
        CH9141_BUF[6+len] = 'm';
    }
    else if(UnitFlag == 1)
    {
        CH9141_BUF[6+len] = 'u';
    }
    else
    {
        CH9141_BUF[6+len] = 'n';
    }
    CH9141_BUF[7+len] = 'F';
    CH9141_BUF[8+len] = BLE_BUF_END;
    PM_UARTSendString(CH9141_BUF, CH9141_BUF_LEN);
}


/*******************************************************************************
* Function Name  : Resistance_Display
* Description    : 电阻参数显示(OLED+BLE)
* Input          : ComParam: 元件参数
* Return         : None
*******************************************************************************/
void PM_DisplayDiode(PM_ComponentMeasureResult *ComParam)
{
    u8 uon_buf[10];
    u8 cap_buf[10];
    u8 i, len1 = 0, len2 = 0;

    len1 = (u8)sprintf((char *)uon_buf, "%d", ComParam->ComponentParam.DiodeParam.Uon);

    LCD_Fill(0, 64, 240-1, 240-1, BLACK);
    LCD_Fill(0, 64, 240-1, 144-1, DARKGRAY);
    LCD_SetColor(DARKGRAY, WHITE);
    LCD_ShowNum(16, 72, ComParam->ComponentParam.DiodeParam.Positive, 32);
    LCD_ShowBWImage(32, 72, 64, 32, DIO_PIC);
    LCD_ShowNum(96, 72, ComParam->ComponentParam.DiodeParam.Negative, 32);
    LCD_ShowString(192, 72, 32, "[D]");

    LCD_ShowString(16, 104, 32, "Uon=");
    for(i=0; i<len1; ++i)
    {
        LCD_ShowChar(80+16*i, 104, uon_buf[i], 32);
    }
    LCD_ShowString(80+16*len1, 104, 32, "mV");

    if (ComParam->ComponentParam.DiodeParam.Cj > 0)
    {
        len2 = (u8)sprintf((char *)cap_buf, "%.2lf", ComParam->ComponentParam.DiodeParam.Cj);

        LCD_Fill(0, 144, 240-1, 176-1, DARKGRAY);
        LCD_ShowString(16, 136, 32, "Cj=");
        for(i=0; i<len2; ++i)
        {
            LCD_ShowChar(64+16*i, 136, cap_buf[i], 32);
        }
        LCD_ShowString(64+16*len2, 136, 32, "nF");
    }

    //APP显示
    BLE_BufClear();
    CH9141_BUF[0] = BLE_BUF_START;
    CH9141_BUF[2] = COMPONENT_TYPE_DIO;
    CH9141_BUF[3] = ComParam->ComponentParam.DiodeParam.Positive;
    CH9141_BUF[4] = ComParam->ComponentParam.DiodeParam.Negative;
    CH9141_BUF[5] = 2+len1;
    memcpy(&CH9141_BUF[6], uon_buf, len1);
    CH9141_BUF[6+len1] = 'm';
    CH9141_BUF[7+len1] = 'V';

    if (ComParam->ComponentParam.DiodeParam.Cj > 0)
    {
        CH9141_BUF[1] = 10+len1+len2;
        CH9141_BUF[8+len1] = 2+len2;
        memcpy(&CH9141_BUF[9+len1], cap_buf, len2);
        CH9141_BUF[9+len1+len2] = 'n';
        CH9141_BUF[10+len1+len2] = 'F';
        CH9141_BUF[11+len1+len2] = BLE_BUF_END;
    }
    else
    {
        CH9141_BUF[1] = 8+len1;
        CH9141_BUF[8+len1] = 0;
        CH9141_BUF[9+len1] = BLE_BUF_END;
    }
    PM_UARTSendString(CH9141_BUF, CH9141_BUF_LEN);
}


/*******************************************************************************
* Function Name  : Resistance_Display
* Description    : 电阻参数显示(OLED+BLE)
* Input          : ComParam: 元件参数
* Return         : None
*******************************************************************************/
void PM_DisplayBJT(PM_ComponentMeasureResult *ComParam)
{
    u8 uon_buf[10];
    u8 hfe_buf[10];
    u8 i, len1 = 0, len2 = 0;

    len1 = (u8)sprintf((char *)hfe_buf, "%d", ComParam->ComponentParam.BJTParam.hFE);
    len2 = (u8)sprintf((char *)uon_buf, "%d", ComParam->ComponentParam.BJTParam.Ube);

//    LCD_Fill(0, 64, 240-1, 240-1, BLACK);
    LCD_Fill(0, 64, 240-1, 240-1, DARKGRAY);
    LCD_SetColor(DARKGRAY, WHITE);
    if (ComParam->ComponentParam.BJTParam.Channel == P_CHANNEL)
    {
        LCD_ShowBWImage(24, 72, 40, 64, PBJT_PIC);
        LCD_ShowString(88, 72, 32, "[NPN-BJT]");
    }
    else
    {
        LCD_ShowBWImage(24, 72, 40, 64, NBJT_PIC);
        LCD_ShowString(88, 72, 32, "[PNP-BJT]");
    }
    LCD_ShowNum(8, 88, ComParam->ComponentParam.BJTParam.B, 32);
    LCD_ShowNum(60, 72, ComParam->ComponentParam.BJTParam.C, 32);
    LCD_ShowNum(60, 104, ComParam->ComponentParam.BJTParam.E, 32);

    LCD_ShowString(0, 136, 32, "hFE=");
    for(i=0; i<len1; ++i)
    {
        LCD_ShowChar(64+16*i, 136, hfe_buf[i], 32);
    }
    LCD_ShowString(0, 168, 32, "Ube=");
    for(i=0; i<len2; ++i)
    {
        LCD_ShowChar(64+16*i, 168, uon_buf[i], 32);
    }
    LCD_ShowString(64+16*len2, 168, 32, "mV");

    LCD_ShowString(0, 200, 32, "Pin ");
    LCD_ShowNum(64, 200, ComParam->ComponentParam.BJTParam.B, 32);
    LCD_ShowString(80, 200, 32, "=B ");
    LCD_ShowNum(128, 200, ComParam->ComponentParam.BJTParam.C, 32);
    LCD_ShowString(144, 200, 32, "=C ");
    LCD_ShowNum(192, 200, ComParam->ComponentParam.BJTParam.E, 32);
    LCD_ShowString(208, 200, 32, "=E");

    //APP显示
    BLE_BufClear();
    CH9141_BUF[0] = BLE_BUF_START;
    CH9141_BUF[1] = 11+len2;
    CH9141_BUF[2] = COMPONENT_TYPE_BJT;
    CH9141_BUF[3] = ComParam->ComponentParam.BJTParam.C;
    CH9141_BUF[4] = ComParam->ComponentParam.BJTParam.B;
    CH9141_BUF[5] = ComParam->ComponentParam.BJTParam.E;
    if (ComParam->ComponentParam.BJTParam.Channel == P_CHANNEL)
    {
        CH9141_BUF[6] = COMPONENT_TYPE_BJT_NPN;
    }
    else
    {
        CH9141_BUF[6] = COMPONENT_TYPE_BJT_PNP;
    }
    CH9141_BUF[7] = (u8)(ComParam->ComponentParam.BJTParam.hFE>>8);
    CH9141_BUF[8] = (u8)ComParam->ComponentParam.BJTParam.hFE;
    CH9141_BUF[9] = 2+len2;
    memcpy(&CH9141_BUF[10], uon_buf, len2);
    CH9141_BUF[10+len2] = 'm';
    CH9141_BUF[11+len2] = 'V';
    CH9141_BUF[12+len2] = BLE_BUF_END;
    PM_UARTSendString(CH9141_BUF, CH9141_BUF_LEN);
}

/******************************** endfile @ pm_display ******************************/
