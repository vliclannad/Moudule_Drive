


#include "wdog.h"

// ===========================================================================
// 函数名称：wdog_start
// 函数参数：timeout:设置重装载的时间
// 函数返回：无
// 功能概要： 启动看门狗模块
// ===========================================================================
void wdog_start(uint32_t timeout)
{
    IWDG->CTLR = IWDG_CTLR_WRITE_ACCESS_ENABLE;    // 对R16_IWDG_PSCR和 R16_IWDG_RLDR 寄存器写使能
    IWDG->PSCR = IWDG_PRESCALER_32;                //设置IWDG的预分频值
    IWDG->RLDR = timeout;                          //设置重装载时间
    IWDG->CTLR = IWDG_CTLE_RELOAD;                  //重装载看门狗计数器
    IWDG->CTLR = IWDG_CTLE_ENABLE;                  //看门狗使能
}




//=========================================================================
//函数名称：wdog_feed
//功能概要：喂狗，清看门狗计数器
//参数说明：无
//函数返回：无
//=========================================================================
void wdog_feed(void)
{
    IWDG->CTLR = IWDG_CTLE_RELOAD;                  //重装载看门狗计数器
}


void wdog_stop(void)
{


}
