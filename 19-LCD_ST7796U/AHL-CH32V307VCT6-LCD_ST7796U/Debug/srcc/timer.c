//=====================================================================
//�ļ����ƣ�timer.c
//���ܸ�Ҫ��timer�ײ���������Դ�ļ�
//������λ�����ݴ�ѧǶ��ʽϵͳ���������о���(sumcu.suda.edu.cn)
//��    ���� 2021-10-31
//����оƬ��
//=====================================================================
#include "timer.h"

//=======================================================================
//�������ƣ�timer_init
//�������أ���
//����˵����timer_No��ʱ��ģ��ţ�ʹ�ú궨��TIMER1��TIMER2������
//          time_ms����ʱ���жϵ�ʱ��������λΪ���룬����Χ��1~2^16ms
//���ܸ�Ҫ��ʱ��ģ���ʼ��������Timer1��Timer8Ϊ�߼���ʱ����Timer2��Timer3Ϊͨ�ö�ʱ����
//          TIM6��TIM7Ϊ������ʱ��
//=======================================================================

void timer_init(uint8_t timer_No,uint32_t time_ms)
{

    uint16_t tmpcr1 = 0;
    switch(timer_No)
    {
        case 1:
        {
            RCC->APB2PCENR |= RCC_TIM1EN;//ʹ�ܶ�ʱ��ʱ��
            tmpcr1 = TIM1->CTLR1;
            tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_DIR | TIM_CMS)));//������룬�������ļ���ģʽ
            tmpcr1 |= 0X0000;//ָ��������ģʽ��
            tmpcr1 &= (uint16_t)(~((uint16_t)TIM_CTLR1_CKD));
            tmpcr1 |= (uint32_t)(0X0000);//����ʱ���Ƶ
            TIM1->CTLR1 = tmpcr1;
            TIM1->ATRLR = (uint16_t)(time_ms-1) ;//��������
            TIM1->PSC = (uint16_t)(144000-1);//����Ԥ��Ƶ
            TIM1->RPTCR= 0x0000;//ָ���ظ���������ֵ
            TIM1->SWEVGR = 0x0001;//��������Ԥ��Ƶ�����¼������Ĵ�����
            TIM1->CTLR1 |= TIM_CEN;//������ʹ��
            break;
        }
        case 8:
       {
           RCC->APB2PCENR |= RCC_TIM8EN;//ʹ�ܶ�ʱ��ʱ��
           tmpcr1 = TIM8->CTLR1;
           tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_DIR | TIM_CMS)));
           tmpcr1 |= 0X0000;//ָ��������ģʽ��
           tmpcr1 &= (uint16_t)(~((uint16_t)TIM_CTLR1_CKD));
           tmpcr1 |= (uint32_t)(0X0000);//����ʱ���Ƶ
           TIM8->CTLR1 = tmpcr1;
           TIM8->ATRLR = (uint16_t)(time_ms-1) ;//��������
           TIM8->PSC = (uint16_t)(48000-1);//����Ԥ��Ƶ
           TIM8->RPTCR= 0x0000;//ָ���ظ���������ֵ
           TIM8->SWEVGR = 0x0001;//��������Ԥ��Ƶ�����¼������Ĵ�����
           TIM8->CTLR1 |= TIM_CEN;//������ʹ��
           break;
       }
        case 2:
        {
            RCC->APB1PCENR |= RCC_TIM2EN;//ʹ�ܶ�ʱ��ʱ��
            tmpcr1 = TIM2->CTLR1;
            tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_DIR | TIM_CMS)));
            tmpcr1 |= 0X0000;//ָ��������ģʽ��
            tmpcr1 &= (uint16_t)(~((uint16_t)TIM_CTLR1_CKD));
            tmpcr1 |= (uint32_t)(0X0000);//����ʱ���Ƶ
            TIM2->CTLR1 = tmpcr1;//�������ϼ���ģʽ
            TIM2->ATRLR = (uint16_t)(48000-1) ;//��������   //time_ms*48000/PSC
            TIM2->PSC = SystemCoreClock-1;//����Ԥ��Ƶ  //��Ƶ֮���Ƶ��=ѡ����ʱ��ԴƵ��/(PSC+1),   (ARR+1)*(PSC+1)/fclk
            TIM2->SWEVGR = 0x0001;//��ʼ��������
            TIM2->CTLR1 |= TIM_CEN;//������ʹ��
            break;
        }
        case 3: //
        {
            RCC->APB1PCENR |= RCC_TIM3EN;
            tmpcr1 = TIM3->CTLR1;
            tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_DIR | TIM_CMS)));
            tmpcr1 |= 0X0000;//ָ��������ģʽ��
            tmpcr1 &= (uint16_t)(~((uint16_t)TIM_CTLR1_CKD));
            tmpcr1 |= (uint32_t)(0X0000);//����ʱ���Ƶ
            TIM3->CTLR1 = tmpcr1;//�������ϼ���ģʽ
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
//�������ƣ�timer_enable_int
//�������أ���
//����˵���� timer_No:ʱ��ģ��ţ�ʹ�ú궨��TIMER1��TIMER2������
//���ܸ�Ҫ��ʱ��ģ��ʹ�ܣ�����ʱ��ģ���жϼ���ʱ���ж�
//============================================================================
void timer_enable_int(uint8_t timer_No)
{
    switch(timer_No)
    {
        case 1:
        {
            TIM1->DMAINTENR |= TIM_UIE;  //���жϿ�����IRQ�ж�
            NVIC_SetPriority( TIM1_UP_IRQn,0);
            NVIC_EnableIRQ(TIM1_UP_IRQn);
            break;
        }
        case 8:
        {
            TIM8->DMAINTENR |= TIM_BIE;  //���жϿ�����IRQ�ж�
            NVIC_SetPriority( TIM8_BRK_IRQn,0);
            NVIC_EnableIRQ(TIM8_BRK_IRQn);
            break;
        }
        case 2:
        {
            TIM2->DMAINTENR |= TIM_CC1IE;  //���жϿ�����IRQ�ж�
            NVIC_SetPriority( TIM2_IRQn,0);
            NVIC_EnableIRQ(TIM2_IRQn);
            break;
        }
        case 3:
        {
            TIM3->DMAINTENR |= TIM_CC2IE;  //���жϿ�����IRQ�ж�
            NVIC_SetPriority( TIM3_IRQn,0);
            NVIC_EnableIRQ(TIM3_IRQn);
            break;
        }
        case 6:
        {
            TIM6->DMAINTENR |= TIM_UIE;  //���жϿ�����IRQ�ж�
            NVIC_SetPriority( TIM6_IRQn,0);
            NVIC_EnableIRQ(TIM6_IRQn);
            break;
        }
        case 7:
         {
             TIM7->DMAINTENR |= TIM_UIE;  //���жϿ�����IRQ�ж�
             NVIC_SetPriority( TIM7_IRQn,0);
             NVIC_EnableIRQ(TIM7_IRQn);
             break;
         }
    }
}

//============================================================================
//�������ƣ�timer_disable_int
//�������أ���
//����˵���� timer_No:ʱ��ģ��ţ�ʹ�ú궨��TIMER1��TIMER2������
//���ܸ�Ҫ����ʱ���жϳ���
//============================================================================
void timer_disable_int(uint8_t timer_No)
{
    switch(timer_No)
    {
        case 1:
        {
            TIM1->DMAINTENR &=~ TIM_UIE;  //��ֹ�жϿ�����IRQ�ж�
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
//�������ƣ�timer_get_int
//����˵���� timer_No:ʱ��ģ��ţ�ʹ�ú궨��TIMER1��TIMER2������
//���ܸ�Ҫ����ȡtimerģ���жϱ�־
//�������أ��жϱ�־ 1=�ж�Ӧģ���жϲ���;0=�޶�Ӧģ���жϲ���
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
//�������ƣ�timer_clear_int
//�������أ���
//����˵���� timer_No:ʱ��ģ��ţ�ʹ�ú궨��TIMER1��TIMER2������
//���ܸ�Ҫ����ʱ������жϱ�־
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
