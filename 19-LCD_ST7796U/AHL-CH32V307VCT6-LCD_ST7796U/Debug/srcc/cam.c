//=====================================================================
//文件名称：ov7670.c
//功能概要：ov7670摄像头底层驱动构件文件
//版    本：2020-08-09
//使用芯片：STM32L433xx
//=====================================================================

//包含头文件
#include "includes.h"
#include "cam.h"

//=================内部调用函数声明=====================================
void cam_gpio_init();
void cam_config_window(uint16_t startx,uint16_t starty,uint16_t width, uint16_t height);
uint8_t rgb565togray_256 (uint16_t color);
void cam_reg_write(uint8_t regID, uint8_t regDat);
void cam_reg_read(uint8_t regID, uint8_t *regDat);
void cam_reg_init();
void Delay_us(uint16_t u16ms);
static uint16_t image16[56*56]={0};
//=====================================================================
//函数名称：cam_init
//函数返回：0：配置成功
//          1：失败
//          2：读取到数据但是不正确，重新初始化一次
//参数说明：无
//功能概要：cam初始化相关配置
//=====================================================================
uint8_t cam_init()
{
    uint8_t state;

    //初始化引脚
    cam_gpio_init();
    Delay_us(50);

    //初始化SCCB通信协议
    SCCB_Init();
    Delay_us(1);
    cam_reg_write(0x12, 0x80);  //读取0X12寄存器
    Delay_us(50);

//    初始化ov7670寄存器
    cam_reg_init();
    Delay_us(50);

    //设置摄像头输出的窗口格式为320*240
    cam_config_window(12,174,240,320);
    cam_reg_read(0x79, &state);

    //判断是否初始化成功
    if(state==0x26)
    {
      return 0;                   //成功
    }
    else if(state==0xff)
    {
      return 1;                   //读取不到数据 硬件接线问题
    }
    else
    {
      return 2;                   //读取到数据但是不正确 重新初始化一次
    }
}

//=====================================================================
//函数名称：cam_getimg3d
//函数返回：图像数组指针
//参数说明：无
//功能概要：获得56*56大小的RGB图像
//=====================================================================
uint16_t* cam_getimg_5656()
{

//    //(1)初始化相关变量
    register uint16_t j,i;
    register uint16_t color=0;

    uint16_t count=0;


    //声明port_ptr为GPIO结构体类型指针（首地址）
    //直接通过寄存器地址操作GPIO B相关端口
    //gpio_ptr->BSR = (uint32_t)(1u<<10); 对RCK口置低电平操作
    //gpio_ptr->BSHR = (uint32_t)(1u<<10); 对RCK口置高电平操作
    //为了提升图像传输速度这里直接使用
//    GPIO_TypeDef *gpio_ptr;
//    gpio_ptr=(GPIO_TypeDef *)GPIOB_BASE;
//
//
//    //(2)得到FIFO缓冲区缓冲结束信号
    if(cam_refresh_end)
    {

//        (2.1)停止ov7670输出图像
        cam_FIFO_OE_0();

//        (2.2)FIFO缓冲区读使能
        cam_FIFO_RRST_0();
        cam_FIFO_RCK_0();
        cam_FIFO_RCK_1();
        cam_FIFO_RCK_0();
        cam_FIFO_RRST_1();
        cam_FIFO_RCK_1();

        //图片显示大小168*168
        //Address_set(2,1,168,168);//Address_set(40,25,206,192);
        Address_set(40,25,206,192);
        //(3)读取FIFO缓冲区的像素数据并显示
        for(i=0;i<240;i++)
        {

            for(j=0;j<320;j++)
            {
                GPIOA->BCR = (uint32_t)(1u<<15);     //时钟口置低电平

                if(i>36&&i<204&&j>76&&j<244)           //168*168if(i>36&&i<204&&j>76&&j<244)
                {
                    //(3.1)首先读取RGB像素点的高八位
                    color=(GPIOD->INDR & 0x000000FF);

                    GPIOA->BSHR = (uint32_t)(1u<<15);//时钟口置高电平

                    //(3.2)像素点左移8位读取低八位数据
                    color<<=8;
                    __asm("NOP");

                    //(3.3)读取RGB像素点的低八位并合成16位RGB数据
                    GPIOA->BCR = (uint32_t)(1u<<15); //时钟口置低电平
                    __asm("NOP");


                    //读取到完整的16位彩色像素数据
                    color|=(GPIOD->INDR & 0x000000FF);

                    //(3.4)直接通过对SPI进行基地址操作来实现快速LCD画点 替换LCD_WR_DATA(color);

                    GPIOE->BSHR = (uint32_t)(1u<<0); //lcd_dc 即e0 置1
                    GPIOE->BCR = (uint32_t)(1u<<1);  //lcd cs 即e1 置0

                    spi_send1(LCD_SPI,color>>8);
                    spi_send1(LCD_SPI,color);
                    GPIOE->BSHR = (uint32_t)(1u<<1); //lcd cs 即b6 置1


                }
                else
                {
                    GPIOA->BSHR = (uint32_t)(1u<<15);
                    GPIOA->BCR = (uint32_t)(1u<<15);
                }

                GPIOA->BSHR = (uint32_t)(1u<<15);

                //(3.4)存储56*56个像素点数据
                if(i%3==2&&j%3==2&&i>36&&i<204&&j>76&&j<244)
                {


                    //保存数组用作图像处理用
                    image16[count]=color;
                    count++;
                }
            }
        }

        //(4)重新复位ov7670输出使能
        cam_FIFO_OE_1();
        cam_FIFO_RCK_0();
        cam_FIFO_RCK_1();

        //(5)清除操作标志
        cam_ready=0;
        cam_fifo_ready=0;
        cam_refresh_end=0;
        //(6)使能像素帧中断
        gpio_enable_int(cam_VSYNC,FALLING_EDGE);
    }
    //（7）返回56*56大小的彩色像素数组
    return image16;
}

//=====================================================================
//函数名称：cam_open
//函数返回：无
//参数说明：无
//功能概要：打开摄像头
//=====================================================================
void cam_open()
{
   gpio_enable_int(cam_VSYNC,FALLING_EDGE);
}

//=====================================================================
//函数名称：cam_close
//函数返回：无
//参数说明：无
//功能概要：关闭摄像头
//=====================================================================
void cam_close()
{
    gpio_disable_int(cam_VSYNC);
}

//=====================================================================
//函数名称：cam_FIFO_WRST_1
//函数返回：无
//参数说明：无
//功能概要：FIFO读使能复位口高电平输出
//=====================================================================
inline void cam_FIFO_WRST_1()
{
    gpio_init(cam_FIFO_WRST,1,1);
}

//=====================================================================
//函数名称：cam_FIFO_WRST_0
//函数返回：无
//参数说明：无
//功能概要：FIFO读使能复位口低电平输出
//=====================================================================
inline void cam_FIFO_WRST_0()
{
    gpio_init(cam_FIFO_WRST,1,0);
}

//=====================================================================
//函数名称：cam_FIFO_WR_1
//函数返回：无
//参数说明：无
//功能概要：FIFO写使能口高电平输出
//=====================================================================
inline void cam_FIFO_WR_1()
{
    gpio_init(cam_FIFO_WR,1,1);
}

//=====================================================================
//函数名称：cam_FIFO_WR_0
//函数返回：无
//参数说明：无
//功能概要：FIFO写使能口低电平输出
//=====================================================================
inline void cam_FIFO_WR_0()
{
    gpio_init(cam_FIFO_WR,1,0);
}

//=====================================================================
//函数名称：cam_FIFO_RCK_0
//函数返回：无
//参数说明：无
//功能概要：FIFO时钟口低电平输出
//=====================================================================
inline void cam_FIFO_RCK_0()
{
    gpio_set(cam_FIFO_RCK,0);
}

//=====================================================================
//函数名称：cam_FIFO_RCK_1
//函数返回：无
//参数说明：无
//功能概要：FIFO时钟口高电平输出
//=====================================================================
inline void cam_FIFO_RCK_1()
{
    gpio_set(cam_FIFO_RCK,1);
}

//=====================================================================
//函数名称：cam_FIFO_RRST_0
//函数返回：无
//参数说明：无
//功能概要：FIFO写使能复位低电平输出
//=====================================================================
inline void cam_FIFO_RRST_0()
{
    gpio_init(cam_FIFO_RRST,1,0);
}

//=====================================================================
//函数名称：cam_FIFO_RRST_1
//函数返回：无
//参数说明：无
//功能概要：FIFO写使能复位高电平输出
//=====================================================================
inline void cam_FIFO_RRST_1()
{
    gpio_init(cam_FIFO_RRST,1,1);
}


//=====================================================================
//函数名称：cam_FIFO_OE_0
//函数返回：无
//参数说明：无
//功能概要：FIFO输出使能低电平输出
//=====================================================================
inline void cam_FIFO_OE_0()
{
    gpio_init(cam_FIFO_OE,1,0);
}

//=====================================================================
//函数名称：cam_FIFO_OE_0
//函数返回：无
//参数说明：无
//功能概要：FIFO输出使能高电平输出
//=====================================================================
inline void cam_FIFO_OE_1()
{
    gpio_init(cam_FIFO_OE,1,1);
}

//----------------------以下为内部函数存放处----------------------------

//=====================================================================
//函数名称：cam_gpio_init
//函数返回：无
//参数说明：无
//功能概要：初始化ov7670使用的gpio引脚
//=====================================================================
void cam_gpio_init()
{
    //初始化控制脚
    gpio_init(cam_FIFO_RCK,1,0);
    gpio_init(cam_VSYNC, 0, 1);         //初始化帧中断读取引脚
    printf("初始化引脚");

    //初始化图像数据接收8个引脚
    gpio_init(cam_DATA_IO_0, 0, 1);
    gpio_init(cam_DATA_IO_1, 0, 1);
    gpio_init(cam_DATA_IO_2, 0, 1);
    gpio_init(cam_DATA_IO_3, 0, 1);
    gpio_init(cam_DATA_IO_4, 0, 1);
    gpio_init(cam_DATA_IO_5, 0, 1);
    gpio_init(cam_DATA_IO_6, 0, 1);
    gpio_init(cam_DATA_IO_7, 0, 1);

    //初始化摄像头输出使能脚
    cam_FIFO_OE_0();
}

//=====================================================================
//函数名称：cam_config_window
//函数返回：无
//参数说明：startx：设置图像开始读取的x坐标
//          starty：设置图像开始读取的y坐标
//          width：设置显示图像窗口的宽度
//          height：设置显示图像窗口的高度
//功能概要：设置ov7670传输后显示图像的窗口参数
//          (140,16,640,480)适合VGA格式输出的图像
//          (272,16,320,240)适合QVGA格式输出的图像
//=====================================================================
void cam_config_window(uint16_t startx,uint16_t starty,uint16_t width, uint16_t height)
{
    uint16_t endx;                      //图像结束点的x坐标
    uint16_t endy;                      //图像结束点的y坐标
    uint8_t temp_reg1, temp_reg2;

    //设置图像显示结束点的横纵坐标
    endx=(startx+width*2);
    endy=(starty+height+height);

    //参数判断
    if(endy>784) endy-=784;

    //修改图像输出的开始与结束像素点
    cam_reg_read(0x03, &temp_reg1 );
    temp_reg1 &= 0xf0;
    temp_reg1|=((endx&0X03)<<2)|(startx&0X03);
    cam_reg_write(0X03,temp_reg1);
    cam_reg_write(0X19,startx>>2);
    cam_reg_write(0X1A,endx>>2);

    cam_reg_read(0x32, &temp_reg2 );
    temp_reg2 &= 0xc0;
    temp_reg2|=((endy&0x7)<<3)|(starty&0x7);
    cam_reg_write(0x17, starty>>3 );
    cam_reg_write(0x18, endy>>3);
}

//=====================================================================
//函数名称：cam_reg_write
//函数返回：无
//参数说明：regID：寄存器地址
//          regDat：写入数值
//功能概要：更改ov7670指定地址寄存器参数
//=====================================================================
 void cam_reg_write(uint8_t regID, uint8_t regDat)
{
    SCCB_Write(cam_OV7670_ADRESS, regID, regDat);
}

//=====================================================================
//函数名称：cam_reg_read
//函数返回：无
//参数说明：regID：寄存器地址
//          regDat：写入数值
//功能概要：读取ov7670指定地址寄存器参数
//=====================================================================
 void cam_reg_read(uint8_t regID, uint8_t *regDat)
{
    SCCB_Read(cam_OV7670_ADRESS, regID, regDat);
}

//=====================================================================
//函数名称：cam_reg_init
//函数返回：无
//参数说明：
//功能概要：参考官方手册配置ov7670寄存器
//=====================================================================
void cam_reg_init()
{
    cam_reg_write(0x3a, 0x04);
    cam_reg_write(0x40, 0xd0);          //565
    cam_reg_write(0x12, 0x14);          //QVGA,RGB输出
    //输出窗口设置
    cam_reg_write(0x32, 0x80);
    cam_reg_write(0x17, 0x16);
    cam_reg_write(0x18, 0x04);
    cam_reg_write(0x19, 0x02);
    cam_reg_write(0x1a, 0x7b);
    cam_reg_write(0x03, 0x06);          //帧竖直方向控制

    cam_reg_write(0x0c, 0x00);
    cam_reg_write(0x15, 0x00);
    cam_reg_write(0x3e, 0x00);
    cam_reg_write(0x70, 0x3a);
    cam_reg_write(0x71, 0x35);
    cam_reg_write(0x72, 0x11);
    cam_reg_write(0x73, 0x00);

    cam_reg_write(0xa2, 0x02);
    cam_reg_write(0x11, 0x81);           //时钟分频设置，0，不分频
    cam_reg_write(0x7a, 0x20);
    cam_reg_write(0x7b, 0x1c);
    cam_reg_write(0x7c, 0x28);

    cam_reg_write(0x7d, 0x3c);
    cam_reg_write(0x7e, 0x55);
    cam_reg_write(0x7f, 0x68);
    cam_reg_write(0x80, 0x76);
    cam_reg_write(0x81, 0x80);

    cam_reg_write(0x82, 0x88);
    cam_reg_write(0x83, 0x8f);
    cam_reg_write(0x84, 0x96);
    cam_reg_write(0x85, 0xa3);
    cam_reg_write(0x86, 0xaf);

    cam_reg_write(0x87, 0xc4);
    cam_reg_write(0x88, 0xd7);
    cam_reg_write(0x89, 0xe8);
    cam_reg_write(0x13, 0xe0);
    cam_reg_write(0x00, 0x00);

    cam_reg_write(0x10, 0x00);
    cam_reg_write(0x0d, 0x30);            //全窗口， 位[5:4]: 01 半窗口，10 1/4窗口，11 1/4窗口
    cam_reg_write(0x14, 0x28);
    cam_reg_write(0xa5, 0x05);
    cam_reg_write(0xab, 0x07);

    cam_reg_write(0x24, 0x75);
    cam_reg_write(0x25, 0x63);
    cam_reg_write(0x26, 0xA5);
    cam_reg_write(0x9f, 0x78);
    cam_reg_write(0xa0, 0x68);

    cam_reg_write(0xa1, 0x03);          //0x0b,
    cam_reg_write(0xa6, 0xdf);          //0xd8,
    cam_reg_write(0xa7, 0xdf);          //0xd8,
    cam_reg_write(0xa8, 0xf0);
    cam_reg_write(0xa9, 0x90);

    cam_reg_write(0xaa, 0x94);
    cam_reg_write(0x13, 0xe5);
    cam_reg_write(0x0e, 0x61);
    cam_reg_write(0x0f, 0x4b);
    cam_reg_write(0x16, 0x02);

    cam_reg_write(0x1e, 0x37);          //图像输出镜像控制.0x07
    cam_reg_write(0x21, 0x02);
    cam_reg_write(0x22, 0x91);
    cam_reg_write(0x29, 0x07);
    cam_reg_write(0x33, 0x0b);

    cam_reg_write(0x35, 0x0b);
    cam_reg_write(0x37, 0x1d);
    cam_reg_write(0x38, 0x71);
    cam_reg_write(0x39, 0x2a);
    cam_reg_write(0x3c, 0x78);

    cam_reg_write(0x4d, 0x40);
    cam_reg_write(0x4e, 0x20);
    cam_reg_write(0x69, 0x00);
    cam_reg_write(0x6b, 0x40);
    cam_reg_write(0x74, 0x19);
    cam_reg_write(0x8d, 0x4f);

    cam_reg_write(0x8e, 0x00);
    cam_reg_write(0x8f, 0x00);
    cam_reg_write(0x90, 0x00);
    cam_reg_write(0x91, 0x00);
    cam_reg_write(0x92, 0x00);

    cam_reg_write(0x96, 0x00);
    cam_reg_write(0x9a, 0x80);
    cam_reg_write(0xb0, 0x84);
    cam_reg_write(0xb1, 0x0c);
    cam_reg_write(0xb2, 0x0e);

    cam_reg_write(0xb3, 0x82);
    cam_reg_write(0xb8, 0x0a);
    cam_reg_write(0x43, 0x14);
    cam_reg_write(0x44, 0xf0);
    cam_reg_write(0x45, 0x34);

    cam_reg_write(0x46, 0x58);
    cam_reg_write(0x47, 0x28);
    cam_reg_write(0x48, 0x3a);
    cam_reg_write(0x59, 0x88);
    cam_reg_write(0x5a, 0x88);

    cam_reg_write(0x5b, 0x44);
    cam_reg_write(0x5c, 0x67);
    cam_reg_write(0x5d, 0x49);
    cam_reg_write(0x5e, 0x0e);
    cam_reg_write(0x64, 0x04);
    cam_reg_write(0x65, 0x20);

    cam_reg_write(0x66, 0x05);
    cam_reg_write(0x94, 0x04);
    cam_reg_write(0x95, 0x08);
    cam_reg_write(0x6c, 0x0a);
    cam_reg_write(0x6d, 0x55);

    cam_reg_write(0x4f, 0x80);
    cam_reg_write(0x50, 0x80);
    cam_reg_write(0x51, 0x00);
    cam_reg_write(0x52, 0x22);
    cam_reg_write(0x53, 0x5e);
    cam_reg_write(0x54, 0x80);

    cam_reg_write(0x09, 0x03);           //驱动能力最大

    cam_reg_write(0x6e, 0x11);
    cam_reg_write(0x6f, 0x9f);
    cam_reg_write(0x55, 0x40);          //亮度控制  30
    cam_reg_write(0x56, 0x40);          //对比度 0x40
    cam_reg_write(0x57, 0x40);

    cam_reg_write(0x6a, 0x40);
    cam_reg_write(0x01, 0x40);
    cam_reg_write(0x02, 0x40);
    cam_reg_write(0x13, 0xe7);
    cam_reg_write(0x15, 0x00);

    cam_reg_write(0x58, 0x9e);

    cam_reg_write(0x41, 0x08);
    cam_reg_write(0x3f, 0x00);
    cam_reg_write(0x75, 0x05);
    cam_reg_write(0x76, 0xe1);
    cam_reg_write(0x4c, 0x00);
    cam_reg_write(0x77, 0x01);
    cam_reg_write(0x3d, 0xc2);
    cam_reg_write(0x4b, 0x09);
    cam_reg_write(0xc9, 0x60);
    cam_reg_write(0x41, 0x38);

    cam_reg_write(0x34, 0x11);
    cam_reg_write(0x3b, 0x02);

    cam_reg_write(0xa4, 0x89);
    cam_reg_write(0x96, 0x00);
    cam_reg_write(0x97, 0x30);
    cam_reg_write(0x98, 0x20);
    cam_reg_write(0x99, 0x30);
    cam_reg_write(0x9a, 0x84);
    cam_reg_write(0x9b, 0x29);
    cam_reg_write(0x9c, 0x03);
    cam_reg_write(0x9d, 0x4c);
    cam_reg_write(0x9e, 0x3f);
    cam_reg_write(0x78, 0x04);

    cam_reg_write(0x79, 0x01);
    cam_reg_write(0xc8, 0xf0);
    cam_reg_write(0x79, 0x0f);
    cam_reg_write(0xc8, 0x00);
    cam_reg_write(0x79, 0x10);
    cam_reg_write(0xc8, 0x7e);
    cam_reg_write(0x79, 0x0a);
    cam_reg_write(0xc8, 0x80);
    cam_reg_write(0x79, 0x0b);
    cam_reg_write(0xc8, 0x01);
    cam_reg_write(0x79, 0x0c);
    cam_reg_write(0xc8, 0x0f);
    cam_reg_write(0x79, 0x0d);
    cam_reg_write(0xc8, 0x20);
    cam_reg_write(0x79, 0x09);
    cam_reg_write(0xc8, 0x80);
    cam_reg_write(0x79, 0x02);
    cam_reg_write(0xc8, 0xc0);
    cam_reg_write(0x79, 0x03);
    cam_reg_write(0xc8, 0x40);
    cam_reg_write(0x79, 0x05);
    cam_reg_write(0xc8, 0x30);
    cam_reg_write(0x79, 0x26);
    cam_reg_write(0x09, 0x00);


}


//======================================================================
//函数名称：Delay_us
//函数返回：无
//参数说明：无
//功能概要：延时 - 微秒级
//======================================================================
void Delay_us(uint16_t u16ms)
{
    uint32_t u32ctr;
    for(u32ctr = 0; u32ctr < (SystemCoreClock/1000*u16ms/100000); u32ctr++)
    {
         __asm ("NOP") ;
    }
}

