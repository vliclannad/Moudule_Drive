/*********************************************************************
 * File Name          : lcd.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/8/15
 * Description        : file for lcd screen.
 *********************************************************************************
* Copyright (c) 2023 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#include "lcd.h"
#include "debug.h"

static uint16_t width;
static uint16_t height;

uint16_t POINT_COLOR= 0x0000;

#define LCD_SOFT_RESET          (0x01)
#define LCD_READ_ID             (0x04)
#define LCD_READ_DISPLAY_STATUS (0x09)
#define LCD_ON                  (0x28)
#define LCD_OFF                 (0x29)

#define LCD_SET_X               (0x2a)
#define LCD_SET_Y               (0x2B)
#define LCD_MEM_WRITE           (0x2C)
#define LCD_MEM_READ            (0x2e)

static uint16_t color_buffer[2][FILL_BUFFER_COUNT] = {0};

/*********************************************************************
 * @fn      LCD_WR_REG
 *
 * @brief   Write an 8-bit command to the LCD screen
 * 
 * @param   data - Command value to be written
 *
 * @return  none
 */
static void LCD_WR_REG(uint8_t data)
{
    LCD_CS_CLR;
    LCD_DC_CLR;
    SPI2_Write(data);
    LCD_CS_SET;
}

/*********************************************************************
 * @fn      LCD_WR_DATA
 *
 * @brief   Write an 8-bit data to the LCD screen
 * 
 * @param   data - the data to write
 * 
 */
static void LCD_WR_DATA(uint8_t data)
{
    LCD_CS_CLR;
    LCD_DC_SET;
    SPI2_Write(data);
    LCD_CS_SET;
}

/*********************************************************************
 * @fn      LCD_WriteReg
 * 
 * @brief   write a data to the register
 * 
 * @param   LCD_Reg - register address
 *          LCD_RegValue - the data to write
 * 
 * @return  none
 */
static void LCD_WriteReg(uint8_t LCD_Reg, uint8_t LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}

/*********************************************************************
 * @fn      Lcd_WriteData_16Bit
 * 
 * @brief   write two bytes to the lcd screen
 * 
 * @param   Data - the data to write
 * 
 * @return  none
 */
void Lcd_WriteData_16Bit(uint16_t Data)
{
    LCD_CS_CLR;
    LCD_DC_SET;
    SPI2_Write(Data >> 8);
    SPI2_Write(Data);
    LCD_CS_SET;
}

/*********************************************************************
 * @fn      LCD_GPIOInit
 * 
 * @brief   Configuring the control gpio for the lcd screen
 * 
 * @return  none
 */
void LCD_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LCD_LED_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(LCD_DC_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(LCD_RST_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(LCD_CS_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = LCD_LED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_LED_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = LCD_DC_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_DC_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = LCD_RST_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_RST_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = LCD_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_CS_PORT, &GPIO_InitStructure);
}

/*********************************************************************
 * @fn      LCD_RESET
 * 
 * @brief   reset the lcd screen by the rst pin
 * 
 * @return  none
 */
void LCD_RESET(void)
{
    LCD_RST_CLR;
    Delay_Ms(100);
    LCD_RST_SET;
    Delay_Ms(50);
}

/*********************************************************************
 * @fn      LCD_Init
 * 
 * @brief   Initialization LCD screen
 * 
 * @return  none
 */
void LCD_Init(void)
{
    SPI2_Init();
    LCD_GPIOInit();
    LCD_RESET();
    LCD_LED_SET;

    #ifdef ILI9488
    LCD_WR_REG(0XF7);
	LCD_WR_DATA(0xA9);
	LCD_WR_DATA(0x51);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x82);
	LCD_WR_REG(0xC0);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x09);
	LCD_WR_REG(0xC1);
	LCD_WR_DATA(0x41);
	LCD_WR_REG(0XC5);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x80);
	LCD_WR_REG(0xB1);
	LCD_WR_DATA(0xB0);
	LCD_WR_DATA(0x11);
	LCD_WR_REG(0xB4);
	LCD_WR_DATA(0x02);
	LCD_WR_REG(0xB6);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x42);
	LCD_WR_REG(0xB7);
	LCD_WR_DATA(0xc6);
	LCD_WR_REG(0xBE);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x04);
	LCD_WR_REG(0xE9);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0x36);
	LCD_WR_DATA((1<<3)|(0<<7)|(1<<6)|(1<<5));
	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x66);
	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x41);
	LCD_WR_DATA(0x89);
	LCD_WR_DATA(0x4B);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x1B);
	LCD_WR_DATA(0x0F);
	LCD_WR_REG(0XE1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x1A);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x45);
	LCD_WR_DATA(0x43);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x0F);
	LCD_WR_REG(0x11);
	Delay_Ms(120);
	LCD_WR_REG(0x29);
    #endif
    #ifdef ST7796U
    LCD_WR_REG(0X11);
    LCD_WR_REG(0X36);
    LCD_WR_DATA(0X48);
    LCD_WR_REG(0X3A);
    LCD_WR_DATA(0X55);
    LCD_WR_REG(0XF0);
    LCD_WR_DATA(0XC3);
    LCD_WR_REG(0XF0);
    LCD_WR_DATA(0X96);
    LCD_WR_REG(0XB4);
    LCD_WR_DATA(0X01);
    LCD_WR_REG(0XB7);
    LCD_WR_DATA(0XC6);
    LCD_WR_REG(0XC0);
    LCD_WR_DATA(0X80);
    LCD_WR_DATA(0X45);
    LCD_WR_REG(0XC1);
    LCD_WR_DATA(0X13);
    LCD_WR_REG(0XC2);
    LCD_WR_DATA(0XA7);
    LCD_WR_REG(0XC5);
    LCD_WR_DATA(0X0A);
    LCD_WR_REG(0xE8);     
    LCD_WR_DATA(0x40);
    LCD_WR_DATA(0x8A);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x19);
    LCD_WR_DATA(0xA5);
    LCD_WR_DATA(0x33);
    LCD_WR_REG(0xE0);
    LCD_WR_DATA(0xD0);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x30);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x47);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x2B);
    LCD_WR_DATA(0x31);
    LCD_WR_REG(0xE1);
    LCD_WR_DATA(0xD0);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x0B);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x2F);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x47);
    LCD_WR_DATA(0x38);
    LCD_WR_DATA(0x15);
    LCD_WR_DATA(0x16);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x32);
    LCD_WR_REG(0xF0);     
    LCD_WR_DATA(0x3C);   
    LCD_WR_REG(0xF0);     
    LCD_WR_DATA(0x69);   
    LCD_WR_REG(0x21);     
    LCD_WR_REG(0x29);
    #endif

    #ifdef ST7796S
    LCD_WR_REG(0xF0);
    LCD_WR_DATA(0xC3);
    LCD_WR_REG(0xF0);
    LCD_WR_DATA(0x96);
    LCD_WR_REG(0x36);
    LCD_WR_DATA(0x68);
    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x05);
    LCD_WR_REG(0xB0);
    LCD_WR_DATA(0x80);
    LCD_WR_REG(0xB6);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x02);
    LCD_WR_REG(0xB5);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x04);
    LCD_WR_REG(0xB1);
    LCD_WR_DATA(0x80);
    LCD_WR_DATA(0x10);
    LCD_WR_REG(0xB4);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0xB7);
    LCD_WR_DATA(0xC6);
    LCD_WR_REG(0xC5);
    LCD_WR_DATA(0x24);
    LCD_WR_REG(0xE4);
    LCD_WR_DATA(0x31);
    LCD_WR_REG(0xE8);
    LCD_WR_DATA(0x40);
    LCD_WR_DATA(0x8A);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x19);
    LCD_WR_DATA(0xA5);
    LCD_WR_DATA(0x33);
    LCD_WR_REG(0xC2);
    LCD_WR_REG(0xA7);
    LCD_WR_REG(0xE0);
    LCD_WR_DATA(0xF0);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x12);
    LCD_WR_DATA(0x12);
    LCD_WR_DATA(0x2B);
    LCD_WR_DATA(0x3C);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x4B);
    LCD_WR_DATA(0x1B);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x1D);
    LCD_WR_DATA(0x21);
    LCD_WR_REG(0XE1);
    LCD_WR_DATA(0xF0);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x0C);
    LCD_WR_DATA(0x0D);
    LCD_WR_DATA(0x27);
    LCD_WR_DATA(0x3B);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x4D);
    LCD_WR_DATA(0x0B);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x1D);
    LCD_WR_DATA(0x21);
    LCD_WR_REG(0X36);
    LCD_WR_DATA(0xEC);
    LCD_WR_REG(0xF0);
    LCD_WR_DATA(0xC3);
    LCD_WR_REG(0xF0);
    LCD_WR_DATA(0x69);
    Delay_Ms(120);
    LCD_WR_REG(0X13);
    LCD_WR_REG(0X11);
    LCD_WR_REG(0X29);
    #endif
    #ifdef ST7789V
    //************* Start Initial Sequence **********//
    LCD_WR_REG(0x11);
    Delay_Ms(120); //Delay 120ms
    //--------display and color format setting-----------//
    LCD_WR_REG(0x36);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0x3a);
    LCD_WR_DATA(0x05);
    //--------ST7789V Frame rate setting------------//
    LCD_WR_REG(0xb2);
    LCD_WR_DATA(0x0c);
    LCD_WR_DATA(0x0c);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x33);
    LCD_WR_REG(0xb7);
    LCD_WR_DATA(0x35);
    //-----------ST7789V Power setting---------------//
    LCD_WR_REG(0xbb);
    LCD_WR_DATA(0x28);
    LCD_WR_REG(0xc0);
    LCD_WR_DATA(0x2c);
    LCD_WR_REG(0xc2);
    LCD_WR_DATA(0x01);
    LCD_WR_REG(0xc3);
    LCD_WR_DATA(0x0b);
    LCD_WR_REG(0xc4);
    LCD_WR_DATA(0x20);
    LCD_WR_REG(0xc6);
    LCD_WR_DATA(0x0f);
    LCD_WR_REG(0xd0);
    LCD_WR_DATA(0xa4);
    LCD_WR_DATA(0xa1);
    //------------ST7789V gamma setting-------------//
    LCD_WR_REG(0xe0);
    LCD_WR_DATA(0xd0);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x0f);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x2a);
    LCD_WR_DATA(0x36);
    LCD_WR_DATA(0x55);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x3a);
    LCD_WR_DATA(0x0b);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x20);
    LCD_WR_REG(0xe1);
    LCD_WR_DATA(0xd0);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x0a);
    LCD_WR_DATA(0x0b);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x34);
    LCD_WR_DATA(0x43);
    LCD_WR_DATA(0x4a);
    LCD_WR_DATA(0x2b);
    LCD_WR_DATA(0x1b);
    LCD_WR_DATA(0x1c);
    LCD_WR_DATA(0x22);
    LCD_WR_DATA(0x1f);
    LCD_WR_REG(0x29);
    #endif




    LCD_direction(USE_HORIZONTAL);  
}

/*********************************************************************
 * @fn      LCD_SetWindows
 * 
 * @brief   Setting LCD display window
 * 
 * @param   xStar -  the bebinning x coordinate of the LCD display window
 *          yStar -  the bebinning y coordinate of the LCD display window
 *          xEnd -  the endning x coordinate of the LCD display window
 *          yEnd -  the endning y coordinate of the LCD display window
 * 
 * @return  none
 */
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
    LCD_WR_REG(LCD_SET_X);
    LCD_WR_DATA(xStar >> 8);
    LCD_WR_DATA(0x00FF & xStar);
    LCD_WR_DATA(xEnd >> 8);
    LCD_WR_DATA(0x00FF & xEnd);

    LCD_WR_REG(LCD_SET_Y);
    LCD_WR_DATA(yStar >> 8);
    LCD_WR_DATA(0x00FF & yStar);
    LCD_WR_DATA(yEnd >> 8);
    LCD_WR_DATA(0x00FF & yEnd);
    LCD_WR_REG(LCD_MEM_WRITE);  
}

void LCD_SetCursor(uint16_t Xpose,uint16_t Ypose)
{
    LCD_SetWindows(Xpose,Ypose,Xpose,Ypose);
}

//绘制一个白色的点
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
    LCD_SetCursor(x,y);
    Lcd_WriteData_16Bit(0xFFFF);
}

//绘制一个指定颜色的点
void LCD_DrawPixel(uint16_t x,uint16_t y,uint16_t color)
{
    LCD_SetCursor(x,y);
    Lcd_WriteData_16Bit(color);
}

//绘制矩形
void LCD_FillRectangle(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t color)
{
    if((x>=LCD_W)||y>=LCD_H) return;
    if((x+w-1)>=LCD_W) w=LCD_W - x;
    if((y+h-1)>=LCD_H) h = LCD_H - x;
    LCD_SetWindows(x,y,x+w-1,y+h-1);
    for(y= h;y>0;y--)
    {
        for(x=w;x>0;x--)
        {
            Lcd_WriteData_16Bit(color);
        }
    }
}
//绘制字母
void LCD_DrawCharS(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size)
 {
    uint8_t line;
    int32_t i, j;
    if ((x >= LCD_W) ||
        (y >= LCD_H) ||
        ((x + 5 * size - 1) < 0) ||
        ((y + 8 * size - 1) < 0))
        return;

    for (i = 0; i < 5; i++) {
        if (i == 5)
            line = 0x0;
        else
            line = Font[(c * 5) + i];
        for (j = 0; j < 8; j++) {
            if (line & 0x1) {
                if (size == 1)
                    LCD_DrawPixel(x + i, y + j, textColor);
                else {
                    LCD_FillRectangle(x + (i * size), y + (j * size), size, size, textColor);
                }
            } else if (bgColor != textColor) {
                if (size == 1) // default size
                    LCD_DrawPixel(x + i, y + j, bgColor);
                else {  // big size
                    LCD_FillRectangle(x + i * size, y + j * size, size, size, bgColor);
                }
            }
            line >>= 1;
        }
    }
}
void LCD_BadApple(uint16_t x,uint16_t y,uint16_t width,uint16_t height,const unsigned char *bitmap,uint8_t size)
{
    uint16_t i,j;
    uint16_t byte;
    uint16_t bytesPerRow=(width+7)/8;//每行字节数

    for(i = 0;i<height;i++)
    {
        for(j=0;j<bytesPerRow;j++)
        {
        byte = bitmap[i*bytesPerRow +j];//每一行数据
        for(uint8_t bit= 0;bit<8;bit++)
        {
            if(byte&(0x80>>bit))
            {
                //LCD_DrawPixel(x+j*8+bit,y+i,LCD_RED);
                LCD_FillRectangle(x + (j*8+bit)*size, y + (i * size), size, size, LCD_BLACK);
            }
        }
        }
    }

}



//绘制字符串
uint32_t LCD_DrawString(uint16_t x, uint16_t y, char *pt, int16_t textColor)
 {
    uint32_t count = 0;
    while (*pt) {
        LCD_DrawCharS(x, y, *pt, textColor, LCD_WHITE, 2);
        pt++;
        x = x + 12;
        count++;
    }
    return count;  // number of characters printed
} 


/*********************************************************************
 * @fn      LCD_direction
 * 
 * @brief   Setting the display direction of LCD screen
 * 
 * @param   direction -   direction:0-0 degree  1-90 degree  2-180 degree  3-270 degree
 * 
 * @return  none
 */
void LCD_direction(uint8_t direction)
{
    switch (direction)
    {
        case 0:
            width  = LCD_W;
            height = LCD_H;
            LCD_WriteReg(0x36, (1 << 3) | (0 << 7) | (1 << 6)|(0<<5));  //BGR==1,MY==0,MX==0,MV==0
            break;
        case 1:
            width  = LCD_H;
            height = LCD_W;
            LCD_WriteReg(0x36, (1 << 3) | (0 << 7) | (1 << 6) | (1 << 5));  //BGR==1,MY==1,MX==0,MV==1
            break;
        case 2:
            width  = LCD_W;
            height = LCD_H;
            LCD_WriteReg(0x36, (1 << 3) | (1 << 7) | (1 << 6)|(0<<5));  //BGR==1,MY==0,MX==0,MV==0
            break;
        case 3:
            width  = LCD_H;
            height = LCD_W;
            LCD_WriteReg(0x36, (1 << 3) | (1 << 7) | (1 << 6)|(5<<0));  //BGR==1,MY==1,MX==0,MV==1
            break;
        default:
            break;
    }
}

/*********************************************************************
 * @fn      LCD_Clear
 * 
 * @brief   Full screen filled LCD screen
 * 
 * @param   Color -   Filled color
 * 
 * @return  none
 */
void LCD_Clear(uint16_t Color)
{
    unsigned int i, m;
    LCD_SetWindows(0, 0, width - 1, height - 1);
    LCD_CS_CLR;
    LCD_DC_SET;
    for (i = 0; i < height; i++)
    {
        for (m = 0; m < width; m++)
        {
            Lcd_WriteData_16Bit(Color);
        }
    }
    LCD_CS_SET;
}

