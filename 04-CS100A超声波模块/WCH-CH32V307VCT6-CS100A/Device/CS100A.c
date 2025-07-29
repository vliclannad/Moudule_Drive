#include "CS100A.h"
#include "ch32v30x_it.h"
//������������
void CS100A_EXTI_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void CS100A_Init(void)
{
    //GPIO��ʼ��
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};    //����ʱ��
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|CS100A_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = CS100A_TRIG_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(CS100A_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = CS100A_ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(CS100A_GPIO_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(CS100A_GPIO_PORT, CS100A_TRIG_PIN);//TRIG����Ĭ�ϵ͵�ƽ������һ��10us���ϵĸߵ�ƽ�󣬱�ɷ��������������ź�
    GPIO_ResetBits(CS100A_GPIO_PORT, CS100A_ECHO_PIN);

    //ECHO�����жϳ�ʼ��
    GPIO_EXTILineConfig(CS100A_PORTSOURCE , CS100A_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = CS100A_EXTILINE; // �ⲿ�ж���2
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // ����Ϊ�ⲿ�ж�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; // ʹ���ⲿ�ж�
    EXTI_Init(&EXTI_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = CS100A_EXTI_IRQ; //
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // ʹ���ж�
    NVIC_Init(&NVIC_InitStructure);


   //��ʱ��ʱ����ʼ��
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(CS100A_TIM_CLK, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = (10-1);                 // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;               // ��ʱ��Ƶ��Ϊ1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                // ��Ƶ����
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ�ʱ
    TIM_TimeBaseInit(CS100A_TIM, &TIM_TimeBaseStructure);

    NVIC_InitStructure.NVIC_IRQChannel = CS100A_TIM_IRQ ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* ��ʱ���жϿ��� */
    TIM_ClearFlag(CS100A_TIM,TIM_IT_Update);         // �����ʱ�жϱ�־λ
    TIM_ITConfig(CS100A_TIM, TIM_IT_Update, ENABLE);   // ������ʱ�ж�
    TIM_SetCounter(CS100A_TIM, 0);                     // ���ó�ʼ����Ϊ0
    TIM_Cmd(CS100A_TIM,DISABLE);                       // ʹ��


}

//�����������̫���Ļ������ܱ�֤����ģ���ܹ����յ�����ģ�����Ϣ���Ӷ����²��ʧ�ܡ�

void CS100A_GET(void)
{
    GPIO_SetBits(CS100A_GPIO_PORT, CS100A_TRIG_PIN);
    Delay_Us(50);
    GPIO_ResetBits(CS100A_GPIO_PORT, CS100A_TRIG_PIN);
}

//������������
void CS100A_EXTI_IRQHandler(void)
{
    if(EXTI_GetITStatus(CS100A_EXTILINE)!=RESET)
    {
        EXTI_ClearITPendingBit(CS100A_EXTILINE);
        if(GPIO_ReadInputDataBit(CS100A_GPIO_PORT, CS100A_ECHO_PIN)==1)
        {
            TIM_SetCounter(CS100A_TIM, 0);
            TIM_Cmd(CS100A_TIM, ENABLE);
        }
        else
        {
            gCS100A_Distance = (float)(gCS100ACount*10+TIM_GetCounter(CS100A_TIM))/1000*34.0/2.0;//34cm/ms,gCS100A_Distanceȫ�ֱ���
            gCS100ACount = 0;
            TIM_Cmd(CS100A_TIM, DISABLE);

        }
    }
}

//��������������ʱ
void CS100A_TIM_IRQHandler(void)
{

    if ( TIM_GetITStatus( CS100A_TIM, TIM_IT_Update) != RESET )
    {
        TIM_ClearFlag(CS100A_TIM , TIM_FLAG_Update);
        gCS100ACount++;
    }

}
