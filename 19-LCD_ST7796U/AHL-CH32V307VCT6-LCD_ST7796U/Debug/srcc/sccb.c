//=====================================================================
//文件名称：sccb.c
//功能概要：串行摄像机控制总线协议(SCCB)底层驱动构件文件
//版    本：2019-11-10 
//使用芯片：STM32L433xx
//=====================================================================

//包含头文件
#include "sccb.h"

void Delay_ms(uint16_t u16ms);


//*************************以下函数为内部函数******************************
//=====================================================================
//函数名称：sdio_input
//函数返回：无
//参数说明：无
//功能概要：将该引脚改为输入模式
//=====================================================================
void sdio_input()
{
    gpio_init(S_DIO, 0, 1);      //将GPIO设置为输入模式
//  gpio_pull(SDIO,1);
}

//=====================================================================
//函数名称：sdio_output
//函数返回：无
//参数说明：无
//功能概要：将该引脚改为输出模式
//=====================================================================
void sdio_output()
{
    gpio_init(S_DIO, 1, 1);      //将GPIO设置为输出模式
}

//=====================================================================
//函数名称：sccb_start
//函数返回：无
//参数说明：无
//功能概要：SCCB发送开始信号函数
//=====================================================================
void sccb_start()
{
    //信号开始时，SCLK保持高电平，SDIO由高变低
    gpio_set(S_CLK, 1);          //SCLK保持高电平
    gpio_set(S_DIO, 1);          //SDIO保持高电平
    Delay_ms(1);
    gpio_set(S_DIO, 0);          //SDIO拉低
    Delay_ms(1);
    gpio_set(S_CLK, 0);          //SCLK拉低
    Delay_ms(1);
}

//=====================================================================
//函数名称：sccb_stop
//函数返回：无
//参数说明：无
//功能概要：SCCB发送结束信号函数
//=====================================================================
void sccb_stop()
{
    //信号开始时，SCLK保持高电平，SDIO由低变高
    gpio_set(S_CLK, 0);          //SCLK保持低电平
    gpio_set(S_DIO, 0);          //SDIO保持低电平
    Delay_ms(1);
    gpio_set(S_CLK, 1);          //SCLK拉高
    Delay_ms(1);
    gpio_set(S_DIO, 1);          //SDIO拉高
    Delay_ms(1);
}

//=====================================================================
//函数名称：sccb_send1
//函数返回：无
//参数说明：data:待发送数据
//功能概要：SCCB发送函数
//=====================================================================
void sccb_send1(uint8_t data)
{
    uint8_t i=0;
    //采用MSB的方式将数据发送
    for(i=0; i<8; i++)
    {
        if(data & (0x80))
            gpio_set(S_DIO, 1);
        else
            gpio_set(S_DIO, 0);
        data <<= 1;
        Delay_ms(1);
        gpio_set(S_CLK, 1);      //SCLK拉高
        Delay_ms(1);
        gpio_set(S_CLK, 0);      //SCLK拉低
        Delay_ms(1);
    }
}

uint8_t sccb_waitAck()
{
    uint8_t ack;
    gpio_set(S_DIO, 1);          //SDIO拉高
    gpio_set(S_CLK, 0);          //SDIO拉高
    sdio_input();               //将SDIO转换成输入模式
    Delay_ms(10);
    gpio_set(S_CLK, 1);          //SCLK拉高
    ack = gpio_get(S_DIO);       //获取应答位
    Delay_ms(1);
    sdio_output();              //将SDIO转换成输出模式
    gpio_set(S_CLK, 0);          //SCLK拉低
    return ack;
}

//=====================================================================
//函数名称：sccb_rec1
//函数返回：无
//参数说明：data:接受数据首地址
//功能概要：SCCB接收函数
//=====================================================================
void sccb_rec1(uint8_t *data)
{
    uint8_t i=0;
    gpio_set(S_DIO, 1);          //将SDIO拉高
    sdio_input();               //将SDIO转换成输入模式
    for(i=0; i<8; i++)
    {
        gpio_set(S_CLK,1);       //将SCLK拉高
        Delay_ms(1);
        *data <<= 1;
        if(gpio_get(S_DIO))      //获取SDIO上的数据
            *data |= 1;
        gpio_set(S_CLK,0);       //将SCLK拉低
        Delay_ms(1);
     }
     sdio_output();             //将SDIO转换成输出模式
     Delay_ms(1);
}

//=====================================================================
//函数名称：sccb_ack
//函数返回：无
//参数说明：无
//功能概要：SCCB发送ack
//=====================================================================
void sccb_ack()
{
     gpio_set(S_CLK,0);          //将SCLK拉低
     Delay_ms(1);
     gpio_set(S_DIO,0);          //将SDIO拉低
     Delay_ms(1);
     gpio_set(S_CLK,1);          //将SCLK拉高
     Delay_ms(1);
     gpio_set(S_CLK,0);          //将SCLK拉低
     Delay_ms(1);
}

//=====================================================================
//函数名称：sccb_nack
//函数返回：无
//参数说明：无
//功能概要：SCCB发送nack
//=====================================================================
void sccb_nack()
{
     gpio_set(S_CLK,0);          //将SCLK拉低
     Delay_ms(1);
     gpio_set(S_DIO,1);          //将SDIO拉高
     Delay_ms(1);
     gpio_set(S_CLK,1);          //将SCLK拉高
     Delay_ms(1);
     gpio_set(S_CLK,0);          //将SCLK拉低
     Delay_ms(1);
}



//*************************以下函数为对外接口函数***************************
//=====================================================================
//函数名称：sccb_init
//函数返回：无
//参数说明：无
//功能概要：SCCB初始化函数
//=====================================================================
void SCCB_Init()
{
    //设置SCLK和SDIO为输出高电平
    gpio_init(S_CLK, 1, 1);
    gpio_init(S_DIO, 1, 1);
}

//=====================================================================
//函数名称：SCCB_Write
//函数返回：无
//参数说明：无
//功能概要：SCCB向指定寄存器发送一个字节数据
//=====================================================================
void SCCB_Write(uint8_t deceAddress, uint8_t regisetrAdd, uint8_t Data)
{
    sccb_start();               //发送开始信号
    sccb_send1(deceAddress);    //发送设备地址
    sccb_waitAck();             //等待应答位
    sccb_send1(regisetrAdd);    //发送寄存器地址
    sccb_waitAck();             //等待应答位
    sccb_send1(Data);           //发送数据
   sccb_waitAck();             //等待应答位
    sccb_stop();                //发送停止信号
}

//=====================================================================
//函数名称：SCCB_Read
//函数返回：无
//参数说明：无
//功能概要：SCCB向指定寄存器读取一个字节数据
//=====================================================================
void SCCB_Read(uint8_t deceAddress, uint8_t regisetrAdd, uint8_t *Data)
{
    sccb_start();               //发送开始信号
    sccb_send1(deceAddress);    //发送设备地址
    sccb_waitAck();             //等待应答位
    sccb_send1(regisetrAdd);    //发送寄存器地址
    sccb_waitAck();             //等待应答位
    sccb_stop();                //发送停止信号
    sccb_start();               //发送开始信号
    sccb_send1(deceAddress+1);  //发送设备地址
    sccb_waitAck();             //等待应答位
    sccb_rec1(Data);            //接收数据
    sccb_nack();                //发送非应答信号
    sccb_stop();                //发送停止信号
}


//======================================================================
//函数名称：Delay_ms
//函数返回：无
//参数说明：无
//功能概要：延时 - 毫秒级
//======================================================================
void Delay_ms(uint16_t u16ms)
{
        uint32_t u32ctr;
    for(u32ctr = 0; u32ctr < (SystemCoreClock/1000*u16ms/100); u32ctr++)
    {
         __asm ("NOP") ;
    }
}
