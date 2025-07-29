#include "CS100A.h"
#include "ch32v30x_it.h"
//超声波传感器
void CS100A_EXTI_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void CS100A_Init(void)
{
    //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};    //开启时钟
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|CS100A_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = CS100A_TRIG_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(CS100A_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = CS100A_ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(CS100A_GPIO_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(CS100A_GPIO_PORT, CS100A_TRIG_PIN);//TRIG引脚默认低电平，输入一个10us以上的高电平后，便可发出超声波脉冲信号
    GPIO_ResetBits(CS100A_GPIO_PORT, CS100A_ECHO_PIN);

    //ECHO引脚中断初始化
    GPIO_EXTILineConfig(CS100A_PORTSOURCE , CS100A_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = CS100A_EXTILINE; // 外部中断线2
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 配置为外部中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; // 使能外部中断
    EXTI_Init(&EXTI_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = CS100A_EXTI_IRQ; //
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能中断
    NVIC_Init(&NVIC_InitStructure);


   //计时定时器初始化
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(CS100A_TIM_CLK, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = (10-1);                 // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;               // 定时器频率为1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                // 分频因子
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计时
    TIM_TimeBaseInit(CS100A_TIM, &TIM_TimeBaseStructure);

    NVIC_InitStructure.NVIC_IRQChannel = CS100A_TIM_IRQ ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 定时器中断开启 */
    TIM_ClearFlag(CS100A_TIM,TIM_IT_Update);         // 清除计时中断标志位
    TIM_ITConfig(CS100A_TIM, TIM_IT_Update, ENABLE);   // 开启计时中断
    TIM_SetCounter(CS100A_TIM, 0);                     // 设置初始计数为0
    TIM_Cmd(CS100A_TIM,DISABLE);                       // 使能


}

//测量物体距离太近的话，不能保证接收模块能够接收到发射模块的信息，从而导致测距失败。

void CS100A_GET(void)
{
    GPIO_SetBits(CS100A_GPIO_PORT, CS100A_TRIG_PIN);
    Delay_Us(50);
    GPIO_ResetBits(CS100A_GPIO_PORT, CS100A_TRIG_PIN);
}

//超声波传感器
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
            gCS100A_Distance = (float)(gCS100ACount*10+TIM_GetCounter(CS100A_TIM))/1000*34.0/2.0;//34cm/ms,gCS100A_Distance全局变量
            gCS100ACount = 0;
            TIM_Cmd(CS100A_TIM, DISABLE);

        }
    }
}

//超声波传感器计时
void CS100A_TIM_IRQHandler(void)
{

    if ( TIM_GetITStatus( CS100A_TIM, TIM_IT_Update) != RESET )
    {
        TIM_ClearFlag(CS100A_TIM , TIM_FLAG_Update);
        gCS100ACount++;
    }

}
