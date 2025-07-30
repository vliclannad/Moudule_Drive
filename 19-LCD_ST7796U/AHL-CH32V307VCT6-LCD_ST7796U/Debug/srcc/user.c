///=====================================================================
//文件名称：user.c文件
//制作单位：SD-Arm(sumcu.suda.edu.cn)
//更新记录：20211230
//移植规则：【固定】
//=====================================================================
#include "user.h"  //【CC-220102】

//======================================================================
//备    注：对中断服务例程进行重写覆盖
//======================================================================
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler()
{
    if(UART2_Handler != 0) UART2_Handler();
}

void EXTI15_10_IRQHandler (void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI15_10_IRQHandler ()
{
    if(Cam_Handler != 0) Cam_Handler();
}

//void EXTI0_IRQHandler (void) __attribute__((interrupt("WCH-Interrupt-fast")));
//void EXTI0_IRQHandler ()
//{
//    if(Cam_Handler != 0) Cam_Handler();
//}


void TIM7_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM7_IRQHandler()
{
    if(TIMER_Handler != 0) TIMER_Handler();
}



