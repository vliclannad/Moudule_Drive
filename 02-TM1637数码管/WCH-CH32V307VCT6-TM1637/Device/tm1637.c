/*
GND
CLK
DIO
5V
*/

#include "tm1637.h"    //包含TM1637头文件
//文件级全局变量

uint8_t tab[22] =
{
        0x3F,    //0
        0x06,    //1
        0x5B,    //2
        0x4F,    //3
        0x66,    //4
        0x6D,    //5
        0x7D,    //6
        0x07,    //7
        0x7F,    //8
        0x6F,    //9
        0x77,    //10/A
        0x7C,    //11/B
        0x58,    //12/C
        0x5E,    //13/D
        0x79,    //14/E
        0x71,    //15/F
        0x76,    //16/H
        0x38,    //17/L
        0x54,    //18/U
        0x73,    //19/P
        0x3E,    //20/U
        0x00,    //21/空
};
uint16_t CLK_PIN;
uint16_t DIO_PIN;
//内部函数声明
void TM1637_start(void);
uint8_t TM1637_ack(void);
void TM1637_stop(void);
void TM1637_Write(uint8_t Data);

//====================接口函数声明=========================================
//======================================================================
//函数名称：TM1637_Init
//函数参数：tm1637用到的两个引脚
//函数返回：无
//功能概要：向TM1637初始化
//======================================================================
void TM1637_Init()
{

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(TM1637_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = TM1637_CKL_PIN|TM1637_DIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TM1637_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(TM1637_PORT, TM1637_CKL_PIN, SET);
    GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,SET);
}
//======================================================================
//函数名称：tm1637_Display
//函数参数：a,b,c,d分别表示在第1,2,3,4位数码管上显示的数字
//       a1,b1,c1,d1分别表示是否显示数字之后的点。1表示显示，0表示不显示
//函数返回：无
//功能概要：向TM1637写入传输的一个字节的数据
//======================================================================
void TM1637_Display(uint8_t a,uint8_t a1,uint8_t b,uint8_t b1,uint8_t c,
		uint8_t c1,uint8_t d,uint8_t d1)
{
	TM1637_start();
	TM1637_Write(0x40);    //写数据到显示寄存器+自动地址加1+普通模式
	TM1637_ack();
	TM1637_stop();
	TM1637_start();
	TM1637_Write(0xc0);    //设置显示首地址即第一个LED
	TM1637_ack();

	TM1637_Write(tab[a]|a1<<7);
	TM1637_ack();
	TM1637_Write(tab[b]|b1<<7);    //h为1时显示时钟中间的两点
	TM1637_ack();
	TM1637_Write(tab[c]|c1<<7);
	TM1637_ack();
	TM1637_Write(tab[d]|d1<<7);
	TM1637_ack();

	TM1637_stop();
	TM1637_start();
	TM1637_Write(0x89);    //显示控制：开显示，2/16亮度
	TM1637_ack();
	TM1637_stop();
}
//==========================内部函数声明===================================
//======================================================================
//函数名称：TM1637_start
//函数参数：无
//函数返回：无
//功能概要：TM1637开始传输数据。CLK为高，DIO由高变低，数据输入开始
//======================================================================
void TM1637_start(void)
{
     GPIO_WriteBit(TM1637_PORT, TM1637_CKL_PIN, SET);
     GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,SET);
     Delay_Ms(2);
	 GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,RESET);//CLK为高，DIO由高变低，数据输入开始
}

//======================================================================
//函数名称：TM1637_ack
//函数参数：无
//函数返回：
//功能概要：TM1637应答数据
//======================================================================
uint8_t TM1637_ack(void)
{
    uint8_t ackBit;
    //GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,SET);
    GPIO_WriteBit(TM1637_PORT,TM1637_CKL_PIN,RESET);
    Delay_Ms(2);
    GPIO_WriteBit(TM1637_PORT,TM1637_CKL_PIN,SET);
    ackBit = GPIO_ReadInputDataBit(TM1637_PORT, TM1637_DIO_PIN);
    Delay_Ms(2);
    GPIO_WriteBit(TM1637_PORT,TM1637_CKL_PIN,RESET);
    Delay_Ms(2);
    return ackBit;
}

//======================================================================
//函数名称：TM1637_stop
//函数参数：无
//函数返回：无
//功能概要：TM1637传输一位数据完成。CLK为高时，DIO由低变高
//======================================================================
void TM1637_stop(void)
{
    GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,RESET);
    Delay_Ms(2);
    GPIO_WriteBit(TM1637_PORT,TM1637_CKL_PIN,SET);
    Delay_Ms(2);
	GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,SET);
	Delay_Ms(2);
}

//======================================================================
//函数名称：TM1637_Write
//函数参数：Data:要显示的一个字节的数据
//函数返回：无
//功能概要：向TM1637写入传输的一个字节的数据
//======================================================================
void TM1637_Write(uint8_t Data)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
            //CLK为低时，DIO上才可以从传数据
	    GPIO_WriteBit(TM1637_PORT,TM1637_CKL_PIN,RESET);
		if(Data & 0x01)
		    GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,SET);
		else
		    GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,RESET);
		Delay_Ms(2);
		Data=Data>>1;
		GPIO_WriteBit(TM1637_PORT,TM1637_CKL_PIN,SET);
		Delay_Ms(2);
	}
}
