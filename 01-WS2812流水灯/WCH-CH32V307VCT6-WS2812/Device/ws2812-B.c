
#include "ws2812-B.h"    //包含ws2812头文件


//====================内部函数声明=======================================
void WS_Reset();    //重启，使改变生效。
//====================内部函数声明=======================================


//====================接口函数声明=======================================
//======================================================================
//函数名称：ws_Init
//函数参数：port_pin：控制ws2812的GPIO
//函数返回：无
//功能概要：初始化ws2812的gpio口
//======================================================================
void WS_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(WS2812B_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = WS2812B_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(WS2812B_GPIO_PORT, &GPIO_InitStructure);
    Delay_Ms(10);
}

//======================================================================
//函数名称：ws_sendOnePix
//函数参数：;ptr：代表灯珠颜色的字节数组;num:灯珠数量
//函数返回：无
//功能概要：设置ws2812灯珠的颜色
//======================================================================
void WS_SendOnePix(uint8_t *ptr,uint8_t num)
{
    uint8_t i,j,temp;
    for(j=0;j<num*3;j++)//一个灯珠需要3个字节的颜色数据
    {
        temp=ptr[j];
        for(i=0;i<8;i++)
        {
            if(temp&0x80)    //从高位开始发送
            {
                GPIO_SetBits(WS2812B_GPIO_PORT, WS2812B_GPIO_PIN);  //发送“1”码,高电平750ns-1.6us,低电平220ns-420ns
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                 __asm volatile ("nop");
                 __asm volatile ("nop");
                 __asm volatile ("nop");
                 __asm volatile ("nop");
                 __asm volatile ("nop");
                 __asm volatile ("nop");

                GPIO_ResetBits(WS2812B_GPIO_PORT, WS2812B_GPIO_PIN);

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
            }
            else             //发送“0”码，高电平220ns-420ns，低电平750ns-1.6us
            {
                GPIO_SetBits(WS2812B_GPIO_PORT, WS2812B_GPIO_PIN);
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");

                GPIO_ResetBits(WS2812B_GPIO_PORT, WS2812B_GPIO_PIN);
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");


            }
            temp=(temp<<1);    //左移一位，发送下一字节
        }
    }
    GPIO_ResetBits(WS2812B_GPIO_PORT, WS2812B_GPIO_PIN);
    Delay_Us(500);
}
//======================================================================
//函数名称：ws2812_reset
//函数参数：port_pin：控制ws2812的GPIO
//函数返回：无
//功能概要：重启ws2812，使改变生效。
//======================================================================
void WS_Reset()
{
    GPIO_ResetBits(WS2812B_GPIO_PORT, WS2812B_GPIO_PIN);
    Delay_Us(500);
}

