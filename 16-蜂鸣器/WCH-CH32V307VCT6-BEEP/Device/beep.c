#include "beep.h"
void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
    GPIO_InitStructure.GPIO        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
        NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4,
        
        // ========== 第一段副歌 (Verse 17-32小节) ==========
        // 音域提高，情感更加激昂de = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);
}

void BEEP_PWM_Init(uint16_t arr, uint16_t psc, uint16_t pulse)
{
    //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(BEEP_TIM_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);

    //PWM初始化
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period = arr - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc - 1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//定时结束后立马触发事件


    TIM_TimeBaseInit( BEEP_TIM, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = pulse;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC3Init(BEEP_TIM, &TIM_OCInitStructure);

    TIM_OC3PreloadConfig(BEEP_TIM, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(BEEP_TIM, ENABLE);

    TIM_Cmd(BEEP_TIM, DISABLE);  // 初始化后先禁用，需要时再启用
    TIM_CtrlPWMOutputs(BEEP_TIM, ENABLE);
}

void BEEP_PWM_Init_1MHz(void)
{
    //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
    RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(BEEP_TIM_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);
    
    //PWM初始化 - 固定1MHz时钟
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // 计算预分频器以获得1MHz PWM时钟
    uint32_t tim_input_clock = SystemCoreClock; 
    uint32_t prescaler = (tim_input_clock / BEEP_PWM_CLOCK_FREQ) - 1;
    
    // 初始设置一个中等频率（500Hz）作为默认值
    uint32_t default_period = BEEP_PWM_CLOCK_FREQ / 500; // 1MHz / 500Hz = 2000
    
    TIM_TimeBaseInitStructure.TIM_Period = default_period - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = prescaler;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    
    TIM_TimeBaseInit(BEEP_TIM, &TIM_TimeBaseInitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = default_period / 2; // 50%占空比
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    
    TIM_OC3Init(BEEP_TIM, &TIM_OCInitStructure);
    
    TIM_OC3PreloadConfig(BEEP_TIM, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(BEEP_TIM, ENABLE);
    
    TIM_Cmd(BEEP_TIM, DISABLE);
    TIM_CtrlPWMOutputs(BEEP_TIM, ENABLE);
}

void BEEP_SetFrequency(uint16_t frequency)
{
    if (frequency == 0) {
        // 静音 - 停止PWM输出
        TIM_Cmd(BEEP_TIM, DISABLE);
        return;
    }
    
    // 使用固定的1MHz PWM时钟，只需要调整ARR值
    uint32_t period = BEEP_PWM_CLOCK_FREQ / frequency; // 1MHz / 频率 = 周期计数值
    
    // 限制周期值在合理范围内
    if (period > 65535) {
        period = 65535;
    }
    if (period < 10) {
        period = 10;
    }
    
    // 更新定时器的ARR和CCR3寄存器
    TIM_SetAutoreload(BEEP_TIM, period - 1);
    TIM_SetCompare3(BEEP_TIM, period / 2); // 50%占空比
    
    // 启动PWM输出
    TIM_Cmd(BEEP_TIM, ENABLE);
}

void BEEP_PlayTone(uint16_t frequency, uint16_t duration_ms)
{
    BEEP_SetFrequency(frequency);
    Delay_Ms(duration_ms);
    // 停止播放
    BEEP_SetFrequency(0);
}

void BEEP_PlayNote(uint16_t note, uint16_t duration_ms)
{
    BEEP_PlayTone(note, duration_ms);
}

void BEEP_Stop(void)
{
    TIM_Cmd(BEEP_TIM, DISABLE);
    TIM_CtrlPWMOutputs(BEEP_TIM, DISABLE);
}

void BEEP_PlayBadApple(void)
{
    // 音乐参数设置
    uint32_t bpm = 137;                    // 每分钟137拍的节奏
    // 计算16分音符的时长：60秒 * 1000毫秒 * 4(四分音符转16分音符) * 0.8(调整系数) / BPM
    uint32_t ndms = 12000;                 // 16分音符的基础时长(毫秒)
    
    // 音符范围：从低音#F调到高音G调
    uint32_t melody[] = {
    NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
    NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
    NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
    NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
    NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
    NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
    NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
    NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
    
    // 过渡段落
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, NOTE_DS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_GS3,
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_GS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_DS3, NOTE_FS3,
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, NOTE_DS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_GS3,
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_GS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_DS3, NOTE_FS3,
        
        // ========== 第一段主歌 (Verse 1: 1-16小节) ==========
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
    NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
    NOTE_F4, NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_F4, NOTE_DS4, NOTE_D4, NOTE_F4,
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
    NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
    NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4,
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
    NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
    NOTE_F4, NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_F4, NOTE_DS4, NOTE_D4, NOTE_F4,
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
    NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
    NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4,
    // Verse 17 - 32
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
    NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
    NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
    NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_F5,
    NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_CS5, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
    // Verse 33 - 48
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
    NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
    NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
    NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_F5,
    NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_CS5, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, 0,
    // Interlude
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, NOTE_DS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_GS3,
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_GS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_DS3, NOTE_FS3,
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, NOTE_DS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_GS3,
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
    NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_GS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_DS3, NOTE_FS3,
    // Verse(2) 1 - 16
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
    NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
    NOTE_F4, NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_F4, NOTE_DS4, NOTE_D4, NOTE_F4,
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
    NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
    NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4,
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
    NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
    NOTE_F4, NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_F4, NOTE_DS4, NOTE_D4, NOTE_F4,
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
    NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
    NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
    NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4,
    // Verse(2) 17 - 32
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
    NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
    NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
    NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_F5,
    NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_CS5, NOTE_AS4, NOTE_GS4, NOTE_AS4,
    //                                               to 1=G
    NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_B4, NOTE_D5,
    // Verse(2) 33 - 48 1=G
    NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_B4,
    NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_B4,
    NOTE_A4, NOTE_G4, NOTE_FS4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4,
    NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_D5,
    NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_B4,
    NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_B4,
    NOTE_A4, NOTE_G4, NOTE_FS4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4,
    NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_D5,
    NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_B4,
    NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_B4,
    NOTE_A4, NOTE_G4, NOTE_FS4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4,
    NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_D5,
    NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_B4,
    NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_E5, NOTE_FS5,
    NOTE_G5, NOTE_FS5, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_A4, NOTE_B4,
    NOTE_A4, NOTE_G4, NOTE_FS4, NOTE_D4, NOTE_E4, 0,
    // Outro 1=G
    NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, 0,
    0
    };
    
    // ========== 音符时长数组 ==========
    // 时长单位：16分音符 (1/16拍)
    // 例如：4 = 四分音符(1/4拍), 2 = 八分音符(1/8拍), 1 = 十六分音符(1/16拍)
    // 时长计算：实际时间 = 数值 * (60秒/BPM) * (1/4) 秒
    uint32_t noteDurations[] = {
        // ========== 前奏时长 (Intro) ==========
    4, 4, 4, 1, 1, 1, 1,
    4, 4, 4, 2, 2,
    4, 4, 4, 1, 1, 1, 1,
    4, 4, 4, 2, 2,
    4, 4, 4, 1, 1, 1, 1,
    4, 4, 4, 2, 2,
    4, 4, 4, 1, 1, 1, 1,
    4, 4, 4, 2, 2,
    4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
    4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
    4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
    4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
    4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
    4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
    4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
    4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
    // Verse 1 - 16
    2, 2, 2, 2, 4, 2, 2,
    4, 4, 2, 2, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    4, 4, 2, 2, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    4, 4, 4, 4,
    2, 2, 2, 2, 4, 2, 2,
    4, 4, 2, 2, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    4, 4, 2, 2, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    4, 4, 4, 4,
    // Verse 17 - 32
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    // Verse 33 - 48
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 4,
    // Interlude
    4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
    4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
    4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
    4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
    4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
    4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
    4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
    4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
    // Verse(2) 1 - 16
    2, 2, 2, 2, 4, 2, 2,
    4, 4, 2, 2, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    4, 4, 2, 2, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    4, 4, 4, 4,
    2, 2, 2, 2, 4, 2, 2,
    4, 4, 2, 2, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    4, 4, 2, 2, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    4, 4, 4, 4,
    // Verse(2) 17 - 32
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    // Verse(2) 33 - 48
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 2, 2,
    2, 2, 2, 2, 4, 4,
    // Outro
    3, 3, 2, 3, 3, 2,
    3, 3, 2, 3, 3, 2,
    3, 3, 2, 3, 3, 2,
    3, 3, 2, 3, 3, 2,
    16  // 最后一个音符：全音符时长
    };
    
    // ========== 音乐播放主循环 ==========
    // 遍历播放整首曲子的每一个音符
    for (uint32_t i = 0; i < 895; i++) {
        // 计算当前音符的实际播放时长 (毫秒)
        // 公式：时长 = (基础时长 * 音符时值) / BPM
        uint32_t noteDuration = ndms * noteDurations[i] / bpm;
        
        // 播放当前音符，如果是0则为静音
        BEEP_PlayNote(melody[i], noteDuration); 
        // 计算音符间的停顿时间 (音符时长的1/4)
        uint32_t pauseBetweenNotes = noteDuration / 4;
        Delay_Ms(pauseBetweenNotes);
    }
    // 播放完成后确保蜂鸣器停止
    BEEP_Stop();
}
