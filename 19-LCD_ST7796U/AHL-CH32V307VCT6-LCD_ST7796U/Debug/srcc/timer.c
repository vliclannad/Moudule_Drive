//=====================================================================
//文件名称：timer.c
//功能概要：timer底层驱动构件源文件
//制作单位：苏州大学嵌入式系统与物联网研究所(sumcu.suda.edu.cn)
//版    本： 2021-10-31
//适用芯片：
//=====================================================================
#include "timer.h"

//=======================================================================
//函数名称：timer_init
//函数返回：无
//参数说明：timer_No：时钟模块号（使用宏定义TIMER1、TIMER2、…）
//          time_ms：定时器中断的时间间隔，单位为毫秒，合理范围：1~2^16ms
//功能概要：时钟模块初始化，其中Timer1、Timer8为高级定时器，Timer2、Timer3为通用定时器，
//          TIM6、TIM7为基本定时器
//=======================================================================

void timer_init(uint8_t timer_No,uint32_t time_ms)
{

    uint16_t tmpcr1 = 0;
    switch(timer_No)
    {
        case 1:
        {
            RCC->APB2PCENR |= RCC_TIM1EN;//使能定时器时钟
            tmpcr1 = TIM1->CTLR1;
            tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_DIR | TIM_CMS)));//中央对齐，向上增的计数模式
            tmpcr1 |= 0X0000;//指定计数器模式。
            tmpcr1 &= (uint16_t)(~((uint16_t)TIM_CTLR1_CKD));
            tmpcr1 |= (uint32_t)(0X0000);//设置时间分频
            TIM1->CTLR1 = tmpcr1;
            TIM1->ATRLR = (uint16_t)(time_ms-1) ;//设置周期
            TIM1->PSC = (uint16_t)(144000-1);//设置预分频
            TIM1->RPTCR= 0x0000;//指定重复计数器的值
            TIM1->SWEVGR = 0x0001;//立即加载预分频器（事件产生寄存器）
            TIM1->CTLR1 |= TIM_CEN;//计数器使能
            break;
        }
        case 8:
       {
           RCC->APB2PCENR |= RCC_TIM8EN;//使能定时器时钟
           tmpcr1 = TIM8->CTLR1;
           tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_DIR | TIM_CMS)));
           tmpcr1 |= 0X0000;//指定计数器模式。
           tmpcr1 &= (uint16_t)(~((uint16_t)TIM_CTLR1_CKD));
           tmpcr1 |= (uint32_t)(0X0000);//设置时间分频
           TIM8->CTLR1 = tmpcr1;
           TIM8->ATRLR = (uint16_t)(time_ms-1) ;//设置周期
           TIM8->PSC = (uint16_t)(48000-1);//设置预分频
           TIM8->RPTCR= 0x0000;//指定重复计数器的值
           TIM8->SWEVGR = 0x0001;//立即加载预分频器（事件产生寄存器）
           TIM8->CTLR1 |= TIM_CEN;//计数器使能
           break;
       }
        case 2:
        {
            RCC->APB1PCENR |= RCC_TIM2EN;//使能定时器时钟
            tmpcr1 = TIM2->CTLR1;
            tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_DIR | TIM_CMS)));
            tmpcr1 |= 0X0000;//指定计数器模式。
            tmpcr1 &= (uint16_t)(~((uint16_t)TIM_CTLR1_CKD));
            tmpcr1 |= (uint32_t)(0X0000);//设置时间分频
            TIM2->CTLR1 = tmpcr1;//设置向上计数模式
            TIM2->ATRLR = (uint16_t)(48000-1) ;//设置周期   //time_ms*48000/PSC
            TIM2->PSC = SystemCoreClock-1;//设置预分频  //分频之后的频率=选定的时钟源频率/(PSC+1),   (ARR+1)*(PSC+1)/fclk
            TIM2->SWEVGR = 0x0001;//初始化计数器
            TIM2->CTLR1 |= TIM_CEN;//计数器使能
            break;
        }
        case 3: //
        {
            RCC->APB1PCENR |= RCC_TIM3EN;
            tmpcr1 = TIM3->CTLR1;
            tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_DIR | TIM_CMS)));
            tmpcr1 |= 0X0000;//指定计数器模式。
            tmpcr1 &= (uint16_t)(~((uint16_t)TIM_CTLR1_CKD));
            tmpcr1 |= (uint32_t)(0X0000);//设置时间分频
            TIM3->CTLR1 = tmpcr1;//设置向上计数模式
            TIM3->ATRLR = (uint16_t)(time_ms-1);
            TIM3->PSC = (uint16_t)(48000-1);
            TIM3->SWEVGR = 0x0001;
            TIM3->CTLR1 |= TIM_CEN;
            break;
        }
        case 6: //
        {
            RCC->APB1PCENR|= RCC_TIM6EN;
            TIM6->ATRLR = (uint16_t)(time_ms-1) ;
            TIM6->PSC = (uint16_t)(48000-1);
            TIM6->SWEVGR = 0x0001;
            TIM6->CTLR1 |= TIM_CEN;
            break;
        }
        case 7: //
       {
            RCC->APB1PCENR|= RCC_TIM7EN;
            TIM7->ATRLR = (uint16_t)(40-1) ;
            TIM7->PSC = (uint16_t)(36000-1);
            TIM7->SWEVGR = 0x0001;
            TIM7->CTLR1 |= TIM_CEN;
            break;

       }

    }

}

//============================================================================
//函数名称：timer_enable_int
//函数返回：无
//参数说明： timer_No:时钟模块号（使用宏定义TIMER1、TIMER2、…）
//功能概要：时钟模块使能，开启时钟模块中断及定时器中断
//============================================================================
void timer_enable_int(uint8_t timer_No)
{
    switch(timer_No)
    {
        case 1:
        {
            TIM1->DMAINTENR |= TIM_UIE;  //开中断控制器IRQ中断
            NVIC_SetPriority( TIM1_UP_IRQn,0);
            NVIC_EnableIRQ(TIM1_UP_IRQn);
            break;
        }
        case 8:
        {
            TIM8->DMAINTENR |= TIM_BIE;  //开中断控制器IRQ中断
            NVIC_SetPriority( TIM8_BRK_IRQn,0);
            NVIC_EnableIRQ(TIM8_BRK_IRQn);
            break;
        }
        case 2:
        {
            TIM2->DMAINTENR |= TIM_CC1IE;  //开中断控制器IRQ中断
            NVIC_SetPriority( TIM2_IRQn,0);
            NVIC_EnableIRQ(TIM2_IRQn);
            break;
        }
        case 3:
        {
            TIM3->DMAINTENR |= TIM_CC2IE;  //开中断控制器IRQ中断
            NVIC_SetPriority( TIM3_IRQn,0);
            NVIC_EnableIRQ(TIM3_IRQn);
            break;
        }
        case 6:
        {
            TIM6->DMAINTENR |= TIM_UIE;  //开中断控制器IRQ中断
            NVIC_SetPriority( TIM6_IRQn,0);
            NVIC_EnableIRQ(TIM6_IRQn);
            break;
        }
        case 7:
         {
             TIM7->DMAINTENR |= TIM_UIE;  //开中断控制器IRQ中断
             NVIC_SetPriority( TIM7_IRQn,0);
             NVIC_EnableIRQ(TIM7_IRQn);
             break;
         }
    }
}

//============================================================================
//函数名称：timer_disable_int
//函数返回：无
//参数说明： timer_No:时钟模块号（使用宏定义TIMER1、TIMER2、…）
//功能概要：定时器中断除能
//============================================================================
void timer_disable_int(uint8_t timer_No)
{
    switch(timer_No)
    {
        case 1:
        {
            TIM1->DMAINTENR &=~ TIM_UIE;  //禁止中断控制器IRQ中断
            NVIC_DisableIRQ(TIM1_UP_IRQn);
            break;
        }
        case 8:
        {
            TIM8->DMAINTENR &=~ TIM_BIE;
            NVIC_DisableIRQ(TIM8_BRK_IRQn);
            break;
        }
        case 2:
        {
            TIM2->DMAINTENR &=~ TIM_CC1IE;
            NVIC_DisableIRQ(TIM2_IRQn);
            break;
        }
        case 3:
        {
            TIM3->DMAINTENR &=~ TIM_CC2IE;
            NVIC_DisableIRQ(TIM3_IRQn);
            break;
        }
        case 6:
        {
            TIM6->DMAINTENR &=~  TIM_UIE;
            NVIC_DisableIRQ(TIM6_IRQn);
            break;
        }
        case 7:
         {
             TIM7->DMAINTENR &=~  TIM_UIE;
             NVIC_DisableIRQ(TIM7_IRQn);
             break;
         }
    }
}

//===================================================================
//函数名称：timer_get_int
//参数说明： timer_No:时钟模块号（使用宏定义TIMER1、TIMER2、…）
//功能概要：获取timer模块中断标志
//函数返回：中断标志 1=有对应模块中断产生;0=无对应模块中断产生
//===================================================================
uint8_t timer_get_int(uint8_t timer_No)
{
    ITStatus bitstatus = RESET;
    uint16_t itstatus = 0x0, itenable = 0x0;
    if(timer_No==1)
    {
        itstatus = TIM1->INTFR & TIM_UIE;
        itenable = TIM1->DMAINTENR & TIM_UIE;

        if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
        {
            bitstatus = SET;
        }
        else
        {
            bitstatus = RESET;
        }
    }
    else if(timer_No==8)
   {
       itstatus = TIM8->INTFR & TIM_BIE;
       itenable = TIM8->DMAINTENR & TIM_BIE;

       if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
       {
           bitstatus = SET;
       }
       else
       {
           bitstatus = RESET;
       }
   }
    else if(timer_No==2)
    {
        itstatus = TIM2->INTFR & TIM_CC1IE;
        itenable = TIM2->DMAINTENR & TIM_CC1IE;

        if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
        {
            bitstatus = SET;
        }
        else
        {
            bitstatus = RESET;
        }
    }
    else if(timer_No==3)
    {
        itstatus = TIM3->INTFR & TIM_CC2IE;
        itenable = TIM3->DMAINTENR & TIM_CC2IE;

        if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
        {
            bitstatus = SET;
        }
        else
        {
            bitstatus = RESET;
        }
    }
    else if(timer_No==7)
    {
        itstatus = TIM7->INTFR & TIM_UIE;
        itenable = TIM7->DMAINTENR & TIM_UIE;

        if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
        {
            bitstatus = SET;
        }
        else
        {
            bitstatus = RESET;
        }

    }
    return bitstatus;
}

//============================================================================
//函数名称：timer_clear_int
//函数返回：无
//参数说明： timer_No:时钟模块号（使用宏定义TIMER1、TIMER2、…）
//功能概要：定时器清除中断标志
//============================================================================
void timer_clear_int(uint8_t timer_No)
{
    switch(timer_No)
    {
        case 1:TIM1->INTFR = (uint16_t)~TIM_UIE;break;
        case 8:TIM8->INTFR = (uint16_t)~TIM_BIE;break;
        case 2:TIM2->INTFR = (uint16_t)~TIM_CC2IE;break;
        case 3:TIM3->INTFR = (uint16_t)~TIM_CC3IE;break;
        case 6:TIM6->INTFR = (uint16_t)~TIM_UIE;break;
        case 7:TIM7->INTFR = (uint16_t)~TIM_UIE;break;


    }
}
