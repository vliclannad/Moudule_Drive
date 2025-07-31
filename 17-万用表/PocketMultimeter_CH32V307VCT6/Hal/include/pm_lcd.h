/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_lcd.h
* Author             : WCH
* Version            : V1.0.0
* Date               : 2023/06/06
* Description        : This file contains the headers of the TFTLCD.
*******************************************************************************/

/*******************************************************************************/
#ifndef __PM_LCD_H
#define __PM_LCD_H

#include "ch32v30x.h"

/*********************************************************************************
 *                                  MACRO
 *********************************************************************************/
/* ÑÓÊ±º¯Êý */
#define  DelayMs(x)  Delay_Ms(x)
#define  DelayUs(x)  Delay_Us(x)

/* PWM Output Mode Definition */
#define PWM_MODE1   0
#define PWM_MODE2   1

/* PWM Output Mode Selection */
#define PWM_MODE PWM_MODE1
//#define PWM_MODE PWM_MODE2

/* ´æ´¢µØÖ· */
#define LCD_CMD         ((u32)0x6001FFFF)
#define LCD_DATA        ((u32)0x60020000)

/* ÆÁÄ»³ß´ç */
#define LCD_W   240
#define LCD_H   240

//POINT_COLOR
#define WHITE            0xFFFF
#define BLACK            0x0000
#define BLUE             0x001F
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE            0X07FF
#define RED              0xF800
#define MAGENTA          0xF81F
#define GREEN            0x07E0
#define CYAN             0x7FFF
#define YELLOW           0xFFE0
#define BROWN            0XBC40
#define BRRED            0XFC07
#define GRAY             0X8430
#define GRAY175          0XAD75
#define GRAY151          0X94B2
#define GRAY187          0XBDD7
#define GRAY240          0XF79E
#define LGRAY            0XC618
#define DARKGRAY         0X5ACB


/*******************************************************************************
 *                                     FUNCTIONS
 *******************************************************************************/
void LCD_Clear(u16 color);
void LCD_AddressSet(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_SetColor(u16 back, u16 fore);

void LCD_DrawPoint(u16 x, u16 y);
void LCD_DrawPointColor(u16 x, u16 y, u16 color);
void LCD_DrawCircle(u16 x0, u16 y0, u8 r);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRecctangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_Fill(u16 x_start, u16 y_start, u16 x_end, u16 y_end, u16 color);

void LCD_ShowNum(u16 x, u16 y, u32 num, u32 size);
void LCD_ShowString(u16 x, u16 y, u32 size, const char *fmt, ...);
void LCD_ShowRGBImage(u16 x, u16 y, u16 length, u16 wide, const u8 *p);
void LCD_ShowBWImage(u16 x, u16 y, u16 length, u16 wide, const u8 *p);
void LCD_ShowChar(u16 x, u16 y, u8 data, u32 size);

void PM_LCDInit(void);
void LCD_SetBrightness(u8 brightness);

void LCD_EnterSleep(void);
void LCD_ExitSleep(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);

/********************************************************************************
********************************************************************************/

#endif  
