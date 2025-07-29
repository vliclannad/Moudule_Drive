#include "motor.h"

//本驱动适用于AM32电调固件的PWM控制
//AM32电调的PWM控制范围是1100us-1900us
//上电后需要维持低于1000us的PWM信号，之后设置PWM信号在1000us-1100us之间,维持1s以上才能解锁电调
//电调解锁后，设置PWM信号在1100us-1900us之间控制电机转速





// 全局变量存储当前油门值
static uint8_t g_current_throttle = 0;  // 当前油门值 0-100%

/**
 * @brief  电机PWM控制初始化
 * @note   初始化TIM3的PWM输出功能，用于控制AM32电调
 *         - 配置GPIO为复用推挽输出模式
 *         - 配置TIM3为PWM模式，输出频率50Hz
 *         - 支持1100us-1900us的脉冲宽度控制
 * @param  arr: 自动重装载值，决定PWM频率 (建议1440，对应50Hz)
 * @param  psc: 预分频值，配合arr设置频率 (建议1000)
 * @param  pulse: 初始脉冲宽度 (建议900，低于最小油门)
 * @retval None
 */
void Motor_Init(uint16_t arr, uint16_t psc, uint16_t pulse) {


    //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(MOTOR_GPIO_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(MOTOR_TIM_CLK, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);//部分复用


    GPIO_InitStructure.GPIO_Pin = MOTOR_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(MOTOR_GPIO_PORT, &GPIO_InitStructure);

    //PWM初始化
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period = arr - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc - 1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//定时结束后立马触发事件


    TIM_TimeBaseInit( MOTOR_TIM, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = pulse;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(MOTOR_TIM, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(MOTOR_TIM, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(MOTOR_TIM, ENABLE);

    TIM_Cmd( MOTOR_TIM, ENABLE );
    TIM_CtrlPWMOutputs(MOTOR_TIM, ENABLE);

}

/**
 * @brief  启动电机并解锁电调
 * @note   按照AM32电调的解锁时序启动电机
 *         - 设置PWM信号到1050us（油门归零位置）
 *         - 维持2秒以上时间解锁电调
 *         - 重置全局油门值为0
 * @param  None
 * @retval None
 */
void Motor_Start(void) {
    TIM_SetCompare1(MOTOR_TIM, 1050);//油门归零
    g_current_throttle = 0;  // 更新全局变量
    Delay_Ms(2000);//油门归零1s以上，解锁ESC
}

/**
 * @brief  停止电机
 * @note   立即停止电机并锁定电调
 *         - 设置PWM信号到900us（低于最小油门）
 *         - 等待2秒确保电调响应
 *         - 重置全局油门值为0
 * @param  None
 * @retval None
 */
void Motor_Stop(void) {
    // 直接设置PWM信号小于油门
    TIM_SetCompare1(MOTOR_TIM, 900); // 设置PWM占空比到900us
    g_current_throttle = 0;  // 更新全局变量
    Delay_Ms(2000); // 等待2秒，确保ESC响应
}

/**
 * @brief  渐变停止电机
 * @note   平滑地减少PWM脉冲宽度直到电机停止
 *         - 读取当前PWM脉冲宽度
 *         - 每次减少50us，间隔100ms
 *         - 确保最终脉冲宽度不低于900us
 * @param  None
 * @retval None
 */
void Motor_StopGradual(void) {
    //逐渐减少占空比到900
    uint16_t current_pulse = TIM_GetCapture1(MOTOR_TIM);
    while (current_pulse > 900) {
        current_pulse -= 50; // 每次减少50us
        if (current_pulse < 900) {
            current_pulse = 900; // 确保不低于900us
        }
        TIM_SetCompare1(MOTOR_TIM, current_pulse);
        Delay_Ms(100); // 每次减少后延时100ms
    }
}

/**
 * @brief  设置电机油门值
 * @note   直接设置电机的油门百分比
 *         - 将百分比转换为对应的PWM脉冲宽度
 *         - 0%对应1100us，100%对应1900us
 *         - 更新全局油门变量
 * @param  throttle_value: 油门值 (0-100%)
 * @retval None
 */
void Motor_SetThrottle(uint8_t throttle_value) {
    if (throttle_value > 100) {
        throttle_value = 100; // 限制最大值为100
    }
    
    // 更新全局油门值
    g_current_throttle = throttle_value;
    
    // 脉冲宽度范围：1100us-1900us
    // 1100us = 1100个计数值 (0%油门)
    // 1900us = 1900个计数值 (100%油门)
    uint16_t pulse = 1100 + (800 * throttle_value) / 100;
    if(throttle_value == 0) {
        pulse = 900; // 如果油门为0，设置为900us
    }

    TIM_SetCompare1(MOTOR_TIM, pulse); // 设置PWM占空比
}

/**
 * @brief  渐变设置油门值
 * @note   平滑地从当前油门值变化到目标油门值
 *         - 避免突然的PWM变化对电机造成冲击
 *         - 提供更平滑的控制体验
 * @param  target_throttle: 目标油门值 (0-100)
 * @param  step_delay_ms: 每步之间的延时 (建议10-50ms)
 * @retval None
 */
void Motor_SetThrottleGradual(uint8_t target_throttle, uint16_t step_delay_ms) {
    if (target_throttle > 100) {
        target_throttle = 100;
    }
    
    // 直接使用全局变量获取当前油门值
    uint8_t current_throttle = g_current_throttle;
    
    // 确定变化方向
    int8_t step = (target_throttle > current_throttle) ? 1 : -1;
    
    // 逐步变化到目标值
    while (current_throttle != target_throttle) {
        current_throttle += step;
        
        // 边界检查
        if (current_throttle > 100) current_throttle = 100;
        if (current_throttle < 0) current_throttle = 0;
        
        // 设置新的油门值
        Motor_SetThrottle(current_throttle);
        
        // 延时
        Delay_Ms(step_delay_ms);
    }
}
/**
 * @brief  油门步进增加
 * @note   每次调用都会根据step_value的值增加油门
 *         - 直接使用全局变量获取当前油门值
 *         - 按步长增加油门值
 *         - 自动限制在0-100%范围内
 *         - 输出调试信息显示变化过程
 * @param  step_value: 步进值 (建议1-10)
 * @retval None
 */
void Motor_ThrottleStepUp(uint8_t step_value) {
    // 直接使用全局变量获取当前油门值
    uint8_t current_throttle = g_current_throttle;
    
    // 计算新的油门值
    uint16_t new_throttle = current_throttle + step_value;
    
    // 限制在0-100%范围内
    if (new_throttle > 100) {
        new_throttle = 100;
    }
    
    // 设置新的油门值
    Motor_SetThrottleGradual((uint8_t)new_throttle, MOTOR_THROTTLE_STEP_DELAY);
    
    // 打印调试信息
    printf("Throttle: %d%% -> %d%% (Step +%d)\r\n", 
           current_throttle, new_throttle, step_value);
}

/**
 * @brief  油门步进减少
 * @note   每次调用都会根据step_value的值减少油门
 *         - 直接使用全局变量获取当前油门值
 *         - 按步长减少油门值
 *         - 自动限制在0-100%范围内
 * @param  step_value: 步进值 (建议1-10)
 * @retval None
 */
void Motor_ThrottleStepDown(uint8_t step_value) {
    // 直接使用全局变量获取当前油门值
    uint8_t current_throttle = g_current_throttle;
    
    // 计算新的油门值
    int16_t new_throttle = current_throttle - step_value;
    
    // 限制在0-100%范围内
    if (new_throttle < 0) {
        new_throttle = 0;
    }
    
    // 设置新的油门值
    Motor_SetThrottleGradual((uint8_t)new_throttle, MOTOR_THROTTLE_STEP_DELAY);
    
    // 打印调试信息
    printf("Throttle: %d%% -> %d%% (Step -%d)\r\n", 
           current_throttle, new_throttle, step_value);
}

/**
 * @brief  获取当前油门值
 * @note   返回当前存储的油门值
 * @param  None
 * @retval 当前油门值 (0-100%)
 */
uint8_t Motor_GetCurrentThrottle(void) {
    return g_current_throttle;
}