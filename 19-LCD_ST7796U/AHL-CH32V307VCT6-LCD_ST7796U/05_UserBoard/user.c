///=====================================================================
//�ļ����ƣ�user.c�ļ�
//������λ��SD-Arm(sumcu.suda.edu.cn)
//���¼�¼��20211230
//��ֲ���򣺡��̶���
//=====================================================================
#include "user.h"  //��CC-220102��

//======================================================================
//��    ע�����жϷ������̽�����д����
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



