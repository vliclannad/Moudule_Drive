/********************************** (C) COPYRIGHT *******************************
* File Name          : pm_lcd.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2023/06/06
* Description        : This file contains the headers of the TFTLCD.
*******************************************************************************/
#include "pm_lcd.h"
#include "stdlib.h"
#include "font.h"
#include "stdarg.h"


#define CONFIG_PM_LCD_DEBUG
#ifdef CONFIG_PM_LCD_DEBUG
#define LOG_INFO(...)       printf(__VA_ARGS__)
#else
#define LOG_INFO(...)
#endif

/*******************************************************************************
 *                           GLOBAL VARIABLES
 *******************************************************************************/
/* 背景色和前面色设置 */
u16 BACK_COLOR = BLACK, FORE_COLOR = WHITE;


/*******************************************************************************
 * @fn      TIM1_PWMOut_Init
 *
 * @brief   Initializes TIM1 PWM output.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *          ccp - the pulse value.
 *
 * @return  none
*******************************************************************************/
void TIM1_PWMOut_Init( u16 arr, u16 psc, u16 ccp )
{
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    TIM_TimeBaseInitStructure.TIM_Period = arr;                         //自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                      //预分频系数, 计数器的时钟频率=TIMxCLK/(psc+1), TIMxCLK=APB2=96MHz
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //该应用中未使用到,输入捕获时滤波用
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;     //向上计数模式
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure );               //定时器1初始化

#if (PWM_MODE == PWM_MODE1)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                   //向上计数时,TIMx_CNT < TIMx_CH1CVR 输出有效电平OC1REF=1，否则输出无效电平OC1REF=0

#elif (PWM_MODE == PWM_MODE2)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
#endif

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;      //输出失能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;     //互补输出使能
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;      //空闲状态时，OC2N输出低电平
    TIM_OCInitStructure.TIM_Pulse = ccp;                                //设置待装入捕获比较寄存器的比较值 TIMx_CH1CVR
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;          //输出极性，低电平有效，应该取反，实际效果是高电平有效
    TIM_OC2Init( TIM1, &TIM_OCInitStructure );                          //通道2初始化

    TIM_CtrlPWMOutputs( TIM1, ENABLE );                                 //主输出使能
    TIM_OC2PreloadConfig( TIM1, TIM_OCPreload_Disable );                //比较捕获寄存器2预装载失能,写入比较捕获寄存器中的值会立即起作用
    TIM_ARRPreloadConfig( TIM1, ENABLE );                               //自动重装载使能
    TIM_Cmd( TIM1, ENABLE );                                            //计数器使能
}


/*******************************************************************************
 * @fn      LCD_GPIO_Init
 *
 * @brief   LCD背光控制GPIO初始化
 *
 * @param   none
 *
 * @return  none
*******************************************************************************/
void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //wait at least 100ms for reset
    DelayMs(100);

    TIM1_PWMOut_Init( 100, 96-1, 50 ); //计数周期为100us,10khZ,占空比为50%
}


/*******************************************************************************
 * @fn      LCD_FSMC_Init
 *
 * @brief   FSMC初始化
 *
 * @param   none
 *
 * @return  none
*******************************************************************************/
void LCD_FSMC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure = {0};
    FSMC_NORSRAMTimingInitTypeDef readWriteTiming = {0};
    FSMC_NORSRAMTimingInitTypeDef writeTiming = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE,ENABLE);

    /* PD0:FSMC_D2, PD1:FSMC_D3, PD14:FSMC_D0, PD15:FSMC_D1 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* PE7:FSMC_D4, PE8:FSMC_D5, PE9:FSMC_D6, PE10_FSMC_D7 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* PD4:FSMC_RD, PD5:FSMC_WR, PD7:FSMC_NE, PD12:FSMC_A17 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    readWriteTiming.FSMC_AddressSetupTime = 0x01;       //定义HCLK周期数来配置地址建立时间的持续时间。该参数取值范围为0 ~ 0xF。
    readWriteTiming.FSMC_AddressHoldTime = 0x00;        //定义配置地址保持时间的HCLK周期数。该参数取值范围为0 ~ 0xF。
    readWriteTiming.FSMC_DataSetupTime = 0x0f;          //定义HCLK周期数来数据建立时间的持续时间
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;  //定义HCLK周期的数量，以配置总线周转的持续时间。
    readWriteTiming.FSMC_CLKDivision = 0x00;            //定义CLK时钟输出信号的周期，以HCLK周期数表示。
    readWriteTiming.FSMC_DataLatency = 0x00;            //定义在获取第一个数据之前向内存发送数据的内存时钟周期数。
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;//异步访问模式

    writeTiming.FSMC_AddressSetupTime = 0x00;
    writeTiming.FSMC_AddressHoldTime = 0x00;
    writeTiming.FSMC_DataSetupTime = 0x03;
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;                              //存储块1使能
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;            //地址数据复用失能
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;                        //存储器类型
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;               //数据总线宽度
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;           //不使能批量模式
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;        //等待信号为低有效，批量模式的时候有效
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;          //禁用异步传输期间等待信号，异步传输有效
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;                        //不支持直接的非对齐批量操作
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;//NWAIT 信号在等待状态前的一个数据周期有效
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;             //写操作使能
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;                    //禁用NWAIT信号
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;                 //扩展模式使能允许写使用FSMC_BWTR寄存器
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;                    //写操作处于异步模式
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}


/*******************************************************************************
 * @fn      LCD_WriteCmd
 *
 * @brief   LCD写命令
 *
 * @param   cmd - 命令
 *
 * @return  none
*******************************************************************************/
void LCD_WriteCmd(const u8 cmd)
{
    *(__IO u8*)LCD_CMD = cmd;
}


/*******************************************************************************
 * @fn      LCD_WriteData
 *
 * @brief   LCD写数据
 *
 * @param   data - 数据
 *
 * @return  none
*******************************************************************************/
void LCD_WriteData(const u8 data)
{
    *(__IO u8*)LCD_DATA=data;
}


/*******************************************************************************
 * @fn      LCD_WriteHalfWord
 *
 * @brief   LCD写半字数据
 *
 * @param   data - 半字数据
 *
 * @return  none
*******************************************************************************/
void LCD_WriteHalfWord(const u16 da)
{
    *(__IO u8*)LCD_DATA=(u8)(da>>8);
    *(__IO u8*)LCD_DATA=(u8)da;
}


/*******************************************************************************
 * @fn      LCD_ReadData
 *
 * @brief   LCD读数据
 *
 * @param   none
 *
 * @return  none
*******************************************************************************/
u8 LCD_ReadData(void)
{
    vu8 ram;

    ram = *(__IO u8*)LCD_DATA;

    return ram;
}


/*******************************************************************************
 * @fn      LCD_ReadReg
 *
 * @brief   LCD读寄存器
 *
 * @param   reg - 寄存器
 *
 * @return  寄存器值
*******************************************************************************/
u8 LCD_ReadReg(const u8 reg)
{
    LCD_WriteCmd(reg);
    DelayUs(5);

    return LCD_ReadData();
}


/*******************************************************************************
 * @fn      PM_LCDInit
 *
 * @brief   LCD初始化
 *
 * @param   none
 *
 * @return  none
*******************************************************************************/
void PM_LCDInit(void)
{
    u16 id = 0;

    LCD_GPIO_Init();
    LCD_FSMC_Init();

    LCD_WriteCmd(0X04);
    id = LCD_ReadData();      //dummy read
    id = LCD_ReadData();      //读到0X85
    id = LCD_ReadData();      //读取0X85
    id <<= 8;
    id |= LCD_ReadData();     //读取0X52

    LOG_INFO("LCD_ID = %x\r\n", id);
    DelayMs(50);

    /* Memory Data Access Control */
    LCD_WriteCmd(0x36);
    LCD_WriteData(0x00);
    /* RGB 5-6-5-bit  */
    LCD_WriteCmd(0x3A);
    LCD_WriteData(0x05);
    /* Porch Setting */
    LCD_WriteCmd(0xB2);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x33);
    LCD_WriteData(0x33);
    /*  Gate Control */
    LCD_WriteCmd(0xB7);
    LCD_WriteData(0x00);
    /* VCOM Setting */
    LCD_WriteCmd(0xBB);
    LCD_WriteData(0x3F);
    /* LCM Control */
    LCD_WriteCmd(0xC0);
    LCD_WriteData(0x2C);
    /* VDV and VRH Command Enable */
    LCD_WriteCmd(0xC2);
    LCD_WriteData(0x01);
    /* VRH Set */
    LCD_WriteCmd(0xC3);
    LCD_WriteData(0x0D);
    /* VDV Set */
    LCD_WriteCmd(0xC4);
    LCD_WriteData(0x20);

    /* Frame Rate Control in Normal Mode */
    LCD_WriteCmd(0xC6);
    LCD_WriteData(0x03);    //60Hz-0x0F   82Hz-0x07  99Hz-0x03

    /* Power Control 1 */
    LCD_WriteCmd(0xD0);
    LCD_WriteData(0xA4);
    LCD_WriteData(0xA1);
    /* Positive Voltage Gamma Control */
    LCD_WriteCmd(0xE0);
    LCD_WriteData(0xF0);
    LCD_WriteData(0x03);
    LCD_WriteData(0x09);
    LCD_WriteData(0x03);
    LCD_WriteData(0x03);
    LCD_WriteData(0x10);
    LCD_WriteData(0x2D);
    LCD_WriteData(0x43);
    LCD_WriteData(0x3F);
    LCD_WriteData(0x33);
    LCD_WriteData(0x0D);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x29);
    LCD_WriteData(0x32);
    /* Negative Voltage Gamma Control */
    LCD_WriteCmd(0xE1);
    LCD_WriteData(0xF0);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x10);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x2D);
    LCD_WriteData(0x33);
    LCD_WriteData(0x45);
    LCD_WriteData(0x3A);
    LCD_WriteData(0x14);
    LCD_WriteData(0x19);
    LCD_WriteData(0x31);
    LCD_WriteData(0x37);
    /* Display Inversion On */
    LCD_WriteCmd(0x21);
    /* Sleep Out */
    LCD_WriteCmd(0x11);
    /* wait for power stability */
    DelayMs(100);

    LCD_Clear(BLACK);

    /* display on */
    GPIO_SetBits(GPIOB,GPIO_Pin_14);
    LCD_WriteCmd(0x29);
}


/*******************************************************************************
 * @fn      LCD_Clear
 *
 * @brief   LCD fill color
 *
 * @param   color
 *
 * @return  none
*******************************************************************************/
void LCD_Clear(u16 color)
{
    u16 i, j;
    u8 data[2] = {0};

    data[0] = color >> 8;
    data[1] = color;
    LCD_AddressSet(0, 0, LCD_W - 1, LCD_H - 1);

    /* 5760 = 240*240/20 */

    for (i = 0; i < LCD_W; i++)
    {
        for (j = 0; j < LCD_H; j++)
        {
            *(__IO u8*)LCD_DATA = data[0];
            *(__IO u8*)LCD_DATA = data[1];
        }
    }
}


/*******************************************************************************
 * @fn      LCD_AddressSet
 *
 * @brief   Set drawing area
 *
 * @param   x1 - start of x position
 *          y1 - start of y position
 *          x2 - end of x position
 *          y2 - end of y position
 *
 * @return  none
*******************************************************************************/
void LCD_AddressSet(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_WriteCmd(0x2a);
    LCD_WriteData(x1 >> 8);
    LCD_WriteData(x1);
    LCD_WriteData(x2 >> 8);
    LCD_WriteData(x2);

    LCD_WriteCmd(0x2b);
    LCD_WriteData(y1 >> 8);
    LCD_WriteData(y1);
    LCD_WriteData(y2 >> 8);
    LCD_WriteData(y2);

    LCD_WriteCmd(0x2C);
}


/*******************************************************************************
 * @fn      LCD_SetBrightness
 *
 * @brief   Set LCD brightness
 *
 * @param   brightness - the pulse value.
 *
 * @return  none
*******************************************************************************/
void LCD_SetBrightness(u8 brightness)
{
    if (brightness > 100)
    {
        brightness = 100;
    }
    TIM_SetCompare2( TIM1, brightness );
}


/*******************************************************************************
 * @fn      LCD_SetColor
 *
 * @brief   Set background color and foreground color
 *
 * @param   back - background color
 *          fore - fore color
 *
 * @return  none
*******************************************************************************/
void LCD_SetColor(u16 back, u16 fore)
{
    BACK_COLOR = back;
    FORE_COLOR = fore;
}


/*******************************************************************************
 * @fn      LCD_DisplayOn
 *
 * @brief   LCD display on
 *
 * @param   none
 *
 * @return  none
*******************************************************************************/
void LCD_DisplayOn(void)
{
    TIM_CtrlPWMOutputs(TIM1, ENABLE );
}


/*******************************************************************************
 * @fn      LCD_DisplayOff
 *
 * @brief   LCD display off
 *
 * @param   none
 *
 * @return  none
*******************************************************************************/
void LCD_DisplayOff(void)
{
    TIM_CtrlPWMOutputs(TIM1, DISABLE ); //定时器1输出失能，进入Idle状态
}


/*******************************************************************************
 * @fn      LCD_EnterSleep
 *
 * @brief   LCD enter the minimum power consumption mode and backlight off
 *
 * @param   none
 *
 * @return  none
*******************************************************************************/
void LCD_EnterSleep(void)
{
    TIM_CtrlPWMOutputs(TIM1, DISABLE ); //定时器1输出失能，进入Idle状态
    DelayMs(5);
    LCD_WriteCmd(0x10);
}


/*******************************************************************************
 * @fn      LCD_ExitSleep
 *
 * @brief   LCD turn off sleep mode and backlight on
 *
 * @param   none
 *
 * @return  none
*******************************************************************************/
void LCD_ExitSleep(void)
{
    TIM_CtrlPWMOutputs(TIM1, ENABLE ); //定时器1输出使能
    DelayMs(5);
    LCD_WriteCmd(0x11);
    DelayMs(120);
}


/*******************************************************************************
 * @fn      LCD_DrawPoint
 *
 * @brief   Display a point on the LCD
 *
 * @param   x - x position
 *          y - y position
 *
 * @return  none
*******************************************************************************/
void LCD_DrawPoint(u16 x, u16 y)
{
    LCD_AddressSet(x, y, x, y);
    LCD_WriteHalfWord(FORE_COLOR);
}


/*******************************************************************************
 * @fn      LCD_DrawPointColor
 *
 * @brief   Display a point on the LCD using the given color
 *
 * @param   x - x position
 *          y - y position
 *          color - color of point
 *
 * @return  none
*******************************************************************************/
void LCD_DrawPointColor(u16 x, u16 y, u16 color)
{
    LCD_AddressSet(x, y, x, y);
    LCD_WriteHalfWord(color);
}


/*******************************************************************************
 * @fn      LCD_Fill
 *
 * @brief   Fill color on the LCD
 *
 * @param   x_start - start of x position
 *          y_start - start of y position
 *          x_end - end of x position
 *          y_end - end of y position
 *          color - Fill color
 *
 * @return  none
*******************************************************************************/
void LCD_Fill(u16 x_start, u16 y_start, u16 x_end, u16 y_end, u16 color)
{
    u16 i = 0, j = 0;
//    u32 size = 0, size_remain = 0;

//    size = (x_end - x_start) * (y_end - y_start) * 2;

//    if (size > LCD_CLEAR_SEND_NUMBER)
//    {
//        /* the number of remaining to be filled */
//        size_remain = size - LCD_CLEAR_SEND_NUMBER;
//        size = LCD_CLEAR_SEND_NUMBER;
//    }

    LCD_AddressSet(x_start, y_start, x_end, y_end);

    for (i = y_start; i <= y_end; i++)
    {
        for (j = x_start; j <= x_end; j++)
        {
            LCD_WriteHalfWord(color);
        }
    }
}


/*******************************************************************************
 * @fn      LCD_DrawLine
 *
 * @brief   Display a line on the LCD
 *
 * @param   x1 - x1 position
 *          y1 - y1 position
 *          x2 - x2 position
 *          y2 - y2 position
 *
 * @return  none
*******************************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 t;
    u32 i = 0;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;

    if (y1 == y2)
    {
        /* fast draw transverse line */
        LCD_AddressSet(x1, y1, x2, y2);

        u8 line_buf[480] = {0};

        for (i = 0; i < x2 - x1; i++)
        {
            line_buf[2 * i] = FORE_COLOR >> 8;
            line_buf[2 * i + 1] = FORE_COLOR;
        }

        for(i=0;i<(x2-x1)*2;i++)
        {
            *(__IO u8*)LCD_DATA=line_buf[i];
        }

        return ;
    }

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    row = x1;
    col = y1;
    if (delta_x > 0)incx = 1;
    else if (delta_x == 0)incx = 0;
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0;
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)distance = delta_x;
    else distance = delta_y;
    for (t = 0; t <= distance + 1; t++)
    {
        LCD_DrawPoint(row, col);
        xerr += delta_x ;
        yerr += delta_y ;
        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}


/*******************************************************************************
 * @fn      LCD_DrawRecctangle
 *
 * @brief   Display a rectangle on the LCD
 *
 * @param   x1 - x1 position
 *          y1 - y1 position
 *          x2 - x2 position
 *          y2 - y2 position
 *
 * @return  none
*******************************************************************************/
void LCD_DrawRecctangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_DrawLine(x1, y1, x2, y1);
    LCD_DrawLine(x1, y1, x1, y2);
    LCD_DrawLine(x1, y2, x2, y2);
    LCD_DrawLine(x2, y1, x2, y2);
}


/*******************************************************************************
 * @fn      LCD_DrawCircle
 *
 * @brief   Display a circle on the LCD
 *
 * @param   x - x position of Center
 *          y - y position of Center
 *          r - radius
 *
 * @return  none
*******************************************************************************/
void LCD_DrawCircle(u16 x0, u16 y0, u8 r)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);
    while (a <= b)
    {
        LCD_DrawPoint(x0 - b, y0 - a);
        LCD_DrawPoint(x0 + b, y0 - a);
        LCD_DrawPoint(x0 - a, y0 + b);
        LCD_DrawPoint(x0 - b, y0 - a);
        LCD_DrawPoint(x0 - a, y0 - b);
        LCD_DrawPoint(x0 + b, y0 + a);
        LCD_DrawPoint(x0 + a, y0 - b);
        LCD_DrawPoint(x0 + a, y0 + b);
        LCD_DrawPoint(x0 - b, y0 + a);
        a++;
        //Bresenham
        if (di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
        LCD_DrawPoint(x0 + a, y0 + b);
    }
}


/*******************************************************************************
 * @fn      LCD_ShowChar
 *
 * @brief   Display character on the LCD
 *
 * @param   x - x position
 *          y - y position
 *          data - character
 *          size - size
 *
 * @return  none
*******************************************************************************/
void LCD_ShowChar(u16 x, u16 y, u8 data, u32 size)
{
    u8 temp;
    u8 num = 0;;
    u8 pos, t;
    u16 colortemp = FORE_COLOR;
    if (x > LCD_W - size / 2 || y > LCD_H - size)return;

    data = data - ' ';

#ifdef ASC2_1608
    if (size == 16)
    {
        LCD_AddressSet(x, y, x + size / 2 - 1, y + size - 1);//(x,y,x+8-1,y+16-1)
        /* fast show char */
        for (pos = 0; pos < size * (size / 2) / 8; pos++)
        {
            temp = asc2_1608[(u16)data * size * (size / 2) / 8 + pos];
            for (t = 0; t < 8; t++)
            {
                if (temp & 0x80)colortemp = FORE_COLOR;
                else colortemp = BACK_COLOR;
                LCD_WriteHalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
    else
#endif

#ifdef ASC2_2412
    if (size == 24)
    {
        LCD_AddressSet(x, y, x + size / 2 - 1, y + size - 1);

        /* fast show char */
        for (pos = 0; pos < (size * 16) / 8; pos++)
        {
            temp = asc2_2412[(u16)data * (size * 16) / 8 + pos];
            if (pos % 2 == 0)
            {
                num = 8;
            }
            else
            {
                num = 4;
            }

            for (t = 0; t < num; t++)
            {
                if (temp & 0x80)colortemp = FORE_COLOR;
                else colortemp = BACK_COLOR;
                LCD_WriteHalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
    else
#endif

#ifdef ASC2_3216
    if (size == 32)
    {
        LCD_AddressSet(x, y, x + size / 2 - 1, y + size - 1);

        /* fast show char */
        for (pos = 0; pos < size * (size / 2) / 8; pos++)
        {
            temp = asc2_3216[(u16)data * size * (size / 2) / 8 + pos];
            for (t = 0; t < 8; t++)
            {
                if (temp & 0x80)colortemp = FORE_COLOR;
                else colortemp = BACK_COLOR;
                LCD_WriteHalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
    else
#endif
    {
        LOG_INFO("There is no any define ASC2_1208 && ASC2_2412 && ASC2_2416 && ASC2_3216 !");
    }
}


/*******************************************************************************
 * @fn      LCD_ShowNum
 *
 * @brief   Display the number on the LCD
 *
 * @param   x - x position
 *          y - y position
 *          size - size of font
 *
 * @return  none
*******************************************************************************/
void LCD_ShowNum(u16 x, u16 y, u32 num, u32 size)
{
    LCD_ShowString(x, y, size, "%d", num);
}


/*******************************************************************************
 * @fn      LCD_ShowString
 *
 * @brief   Display the string on the LCD
 *
 * @param   x - x position
 *          y - y position
 *          size - size of font
 *          p - the string to be display
 *
 * @return  none
*******************************************************************************/
void LCD_ShowString(u16 x, u16 y, u32 size, const char *fmt, ...)
{
#define LCD_STRING_BUF_LEN 128

    va_list args;
    u8 buf[LCD_STRING_BUF_LEN] = {0};
    u8 *p = NULL;

    if (size != 16 && size != 24 && size != 32)
    {
        LOG_INFO("font size(%d) is not support!", size);
    }

    va_start(args, fmt);
    vsnprintf((char *)buf, 100, (const char *)fmt, args);
    va_end(args);

    p = buf;
    while (*p != '\0')
    {
        if (x > LCD_W - size / 2)
        {
            x = 0;
            y += size;
        }
        if (y > LCD_H - size)
        {
            y = x = 0;
            LCD_Clear(RED); //显示溢出屏幕就显示红色
        }
        LCD_ShowChar(x, y, *p, size);
        x += size / 2;
        p++;
    }
}


/*******************************************************************************
 * @fn      LCD_ShowImage
 *
 * @brief   Display the color image on the LCD
 *
 * @param   x - x position
 *          y - y position
 *          length - length of image
 *          wide - wide of image
 *          p - image
 *
 * @return  none
*******************************************************************************/
void LCD_ShowRGBImage(u16 x, u16 y, u16 length, u16 wide, const u8 *p)
{
    u16 i=0;
//    RT_ASSERT(p);

    if (x + length > LCD_W || y + wide > LCD_H)
    {
        LOG_INFO("Error");
    }

    LCD_AddressSet(x, y, x + length - 1, y + wide - 1);

    for(i=0 ; i<length * wide * 2; i++)
    {
        *(__IO u8*)LCD_DATA = p[i];
    }
}

/*******************************************************************************
 * @fn      LCD_ShowImage
 *
 * @brief   Display the monochrome image on the LCD
 *
 * @param   x - x position
 *          y - y position
 *          length - length of image
 *          wide - wide of image
 *          p - image
 *
 * @return  none
*******************************************************************************/
void LCD_ShowBWImage(u16 x, u16 y, u16 length, u16 wide, const u8 *p)
{
    u16 pos;
    u8 temp, i;
    u16 colortemp = FORE_COLOR;

    if (x + length > LCD_W || y + wide > LCD_H)
    {
        LOG_INFO("Error");
    }

    LCD_AddressSet(x, y, x + length - 1, y + wide - 1);

    for (pos = 0; pos < length * wide / 8; pos++)
    {
        temp = p[pos];
        for (i=0; i<8; ++i)
        {
            if (temp & 0x80)
            {
                colortemp = FORE_COLOR;
            }
            else
            {
                colortemp = BACK_COLOR;
            }
            LCD_WriteHalfWord(colortemp);
            temp <<= 1;
        }
    }
}
