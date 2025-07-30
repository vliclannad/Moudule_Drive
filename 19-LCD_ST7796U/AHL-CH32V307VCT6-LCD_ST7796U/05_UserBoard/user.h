//======================================================================
//文件名称：user.h（user头文件）
//制作单位：SD-Arm(sumcu.suda.edu.cn)
//更新记录：20200831-20200903
//使用说明：用户使用到的芯片引脚在本文件中进行宏定义，以便06、07文件夹的
//          程序满足不同芯片的可移植性
//======================================================================

#ifndef __USER_H
#define __USER_H

//（1）文件包含
#include "emuart.h"
#include "gpio.h"
#include "flash.h"
#include "uart.h"
#include "adc.h"
#include "wdog.h"
#include "gec.h"
#include "printf.h"
#include "math.h"
#include "Temp_Convert.h"

//（2）宏常数定义
//为了06、07文件夹可复用，main.c及isr.h中用到的宏常数在这里定义


//（3）硬件引脚及模块名称宏定义
//为了06、07文件夹可复用，硬件引脚及模块名称在这里从用户视角进行宏定义
//指示灯引脚定义
#define  LIGHT_RED     (PTC_NUM|0)   //红灯
#define  LIGHT_GREEN   (PTC_NUM|1)   //绿灯
#define  LIGHT_BLUE    (PTC_NUM|2)   //蓝灯
#define  LIGHT_ON    0   //灯亮（灯状态宏定义，由硬件接法决定）
#define  LIGHT_OFF   1   //灯暗
//用户串口UART_User定义
#define UART_User    UART_2  //用户串口（黑-GND;白-TX;绿-RX）
//ADC模块定义
#define AD_MCU_TEMP   16	//内部温度采集通道
#define AD_TEMP       14	//热敏温度采集通道14((PTC_NUM|4))
//触摸区引脚定义
#define  GPIO_TSI      (PTD_NUM|2)  	 //定义GPIO_Tsi引脚

//（4）用户中断服务例程声明及其与缺省中断服务例程的关联
//用户串口UART_User的中断服务例程
extern void UART_User_Handler(void);
#define UART2_Handler UART_User_Handler  //UART_User_Handler在isr.c中实现
extern void Cam_User_Handler(void);
#define Cam_Handler    Cam_User_Handler   //用户摄像头读取标志中断函数
#define TIMER_USER           TIMER7             //用户定时器TIMER
extern void TIMER_USER_Handler(void);
#define TIMER_Handler   TIMER_USER_Handler    //用户定时器中断函数






#endif 
