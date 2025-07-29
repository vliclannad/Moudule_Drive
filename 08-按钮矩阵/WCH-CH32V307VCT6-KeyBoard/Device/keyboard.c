/**
 ******************************************************************************
 * @file    keyboard.c
 * @author  RISC-V Development Team
 * @version V1.0.0
 * @date    2025-07-28
 * @brief   3x3按键矩阵驱动程序
 ******************************************************************************
 * @attention
 * 
 * 本文件实现了3x3按键矩阵的完整驱动功能，包括：
 * - GPIO配置和外部中断初始化
 * - 行列扫描模式切换
 * - 按键状态管理（按下/释放/长按）
 * - 软件消抖处理
 * 
 * 按键矩阵布局：
 *   COL1  COL2  COL3
 * ROW1 [11] [12] [13]
 * ROW2 [21] [22] [23] 
 * ROW3 [31] [32] [33]
 * 
 * 工作原理：
 * 1. 默认行引脚为上拉输入，列引脚为低电平输出
 * 2. 按键按下时，对应行引脚被拉低，触发外部中断
 * 3. 中断中切换到列扫描模式，确定具体按键位置
 * 4. 记录按键状态，恢复行扫描模式
 ******************************************************************************
 */

#include "keyboard.h"

// 全局按键状态变量定义
KeyboardStatus_t gkeyboard_status = {0};

void KEYBOARD_EXTI_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/**
 * @brief  按键矩阵初始化函数
 * @note   配置3x3按键矩阵的GPIO和外部中断
 *         - 行引脚配置为上拉输入，用于检测按键按下
 *         - 列引脚配置为低电平推挽输出，提供扫描信号
 *         - 配置行引脚的外部中断，下降沿触发
 * @param  None
 * @retval None
 */
void Keyboard_Init(void)
{
    //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};    //开启时钟
    //所有行配置为上拉输入
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|KEY_ROW_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = KEY_ROW1_PIN|KEY_ROW2_PIN|KEY_ROW3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_ROW_PORT, &GPIO_InitStructure);
    //所有列配置为低电平推挽输出
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|KEY_COL_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = KEY_COL1_PIN|KEY_COL2_PIN|KEY_COL3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEY_COL_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(KEY_COL_PORT, KEY_COL1_PIN|KEY_COL2_PIN|KEY_COL3_PIN);
    //行因引脚中断初始化,按键按下进入对应中断
    GPIO_EXTILineConfig(KEY_ROW_PORTSOURCE , KEY_ROW1_PINSOURCE);
    GPIO_EXTILineConfig(KEY_ROW_PORTSOURCE , KEY_ROW2_PINSOURCE);
    GPIO_EXTILineConfig(KEY_ROW_PORTSOURCE , KEY_ROW3_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY_ROW1_EXTILINE|KEY_ROW2_EXTILINE|KEY_ROW3_EXTILINE; // 外部中断线2
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 配置为外部中断
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; // 使能外部中断
    EXTI_Init(&EXTI_InitStructure);
    //按键矩阵
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_InitStructure.NVIC_IRQChannel = KEYBOARD_EXTI_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  列扫描模式配置函数
 * @note   当检测到某行有按键按下时，切换到列扫描模式来确定具体是哪个按键
 *         - 将所有行引脚配置为低电平推挽输出
 *         - 将所有列引脚配置为上拉输入
 *         - 通过读取列引脚状态来确定按键位置
 * @param  None
 * @retval None
 */
void Keyboard_ScanCol_Mode(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    //所有行配置为低电平推挽输出模式
    GPIO_InitStructure.GPIO_Pin = KEY_ROW1_PIN|KEY_ROW2_PIN|KEY_ROW3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEY_ROW_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(KEY_ROW_PORT, KEY_ROW1_PIN|KEY_ROW2_PIN|KEY_ROW3_PIN);

    //所有列配置为上拉输入模式
    GPIO_InitStructure.GPIO_Pin = KEY_COL1_PIN|KEY_COL2_PIN|KEY_COL3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_COL_PORT, &GPIO_InitStructure);
}

/**
 * @brief  行扫描模式配置函数
 * @note   扫描完成后，恢复到行扫描模式，等待下一次按键按下
 *         - 将所有行引脚配置为上拉输入，用于检测按键按下
 *         - 将所有列引脚配置为低电平推挽输出，提供扫描信号
 *         - 这是按键矩阵的默认工作模式
 * @param  None
 * @retval None
 */
void Keyboard_ScanRow_Mode(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Pin = KEY_ROW1_PIN|KEY_ROW2_PIN|KEY_ROW3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_ROW_PORT, &GPIO_InitStructure);

    //所有列配置为低电平推挽输出
    GPIO_InitStructure.GPIO_Pin = KEY_COL1_PIN|KEY_COL2_PIN|KEY_COL3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEY_COL_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(KEY_COL_PORT, KEY_COL1_PIN|KEY_COL2_PIN|KEY_COL3_PIN);

}

/**
 * @brief  按键矩阵外部中断处理函数
 * @note   当按键矩阵的任何一行有按键按下时触发此中断
 *         - 检测是哪一行触发中断
 *         - 切换到列扫描模式，确定具体按键位置
 *         - 调用状态管理函数记录按键按下状态
 *         - 包含50ms软件消抖处理
 * @param  None
 * @retval None
 */
void KEYBOARD_EXTI_IRQHandler(void)
{
    //行1中断触发
    if(EXTI_GetITStatus(KEY_ROW1_EXTILINE)!=RESET)
    {
        EXTI_ClearITPendingBit(KEY_ROW1_EXTILINE);
        Delay_Ms(50);
        if(GPIO_ReadInputDataBit(KEY_ROW_PORT, KEY_ROW1_PIN)==0)
        {
            Keyboard_ScanCol_Mode();
            if(GPIO_ReadInputDataBit(KEY_COL_PORT, KEY_COL1_PIN)==0)
            {
                //printf("11\r\n");
                Keyboard_SetKeyPressed(KEY_11);
            }
            if(GPIO_ReadInputDataBit(KEY_COL_PORT, KEY_COL2_PIN)==0)
            {
                //printf("12\r\n");
                Keyboard_SetKeyPressed(KEY_12);
            }
            if(GPIO_ReadInputDataBit(KEY_COL_PORT, KEY_COL3_PIN)==0)
            {
                //printf("13\r\n");
                Keyboard_SetKeyPressed(KEY_13);
            }
            Keyboard_ScanRow_Mode();
            EXTI_ClearITPendingBit(KEY_ROW1_EXTILINE);
        }

    }
    //行2中断触发
    else if(EXTI_GetITStatus(KEY_ROW2_EXTILINE)!=RESET)
    {
        EXTI_ClearITPendingBit(KEY_ROW2_EXTILINE);
        Delay_Ms(50);
        if(GPIO_ReadInputDataBit(KEY_ROW_PORT, KEY_ROW2_PIN)==0)
        {
            Keyboard_ScanCol_Mode();
            if(GPIO_ReadInputDataBit(KEY_COL_PORT, KEY_COL1_PIN)==0)
            {
                //printf("21\r\n");
                Keyboard_SetKeyPressed(KEY_21);
            }
            if(GPIO_ReadInputDataBit(KEY_COL_PORT, KEY_COL2_PIN)==0)
            {
                //printf("22\r\n");
                Keyboard_SetKeyPressed(KEY_22);
            }
            if(GPIO_ReadInputDataBit(KEY_COL_PORT, KEY_COL3_PIN)==0)
            {
                //printf("23\r\n");
                Keyboard_SetKeyPressed(KEY_23);
            }
            Keyboard_ScanRow_Mode();
            EXTI_ClearITPendingBit(KEY_ROW2_EXTILINE);
        }

    }
    //行3中断触发
    else if(EXTI_GetITStatus(KEY_ROW3_EXTILINE)!=RESET)
    {
        EXTI_ClearITPendingBit(KEY_ROW3_EXTILINE);
        Delay_Ms(50);
        if(GPIO_ReadInputDataBit(KEY_ROW_PORT, KEY_ROW3_PIN)==0)
        {
            Keyboard_ScanCol_Mode();
            if(GPIO_ReadInputDataBit(KEY_COL_PORT, KEY_COL1_PIN)==0)
            {
                //printf("31\r\n");
                Keyboard_SetKeyPressed(KEY_31);
            }
            if(GPIO_ReadInputDataBit(KEY_COL_PORT, KEY_COL2_PIN)==0)
            {
                //printf("32\r\n");
                Keyboard_SetKeyPressed(KEY_32);
            }
            if(GPIO_ReadInputDataBit(KEY_COL_PORT, KEY_COL3_PIN)==0)
            {
                //printf("33\r\n");
                Keyboard_SetKeyPressed(KEY_33);
            }
            Keyboard_ScanRow_Mode();
            EXTI_ClearITPendingBit(KEY_ROW3_EXTILINE);
        }

    }
}

// ==================== 按键状态管理函数实现 ====================

/**
 * @brief  设置指定按键为按下状态
 * @note   使用位操作设置按键状态位域
 *         - 设置对应位为1表示按键按下
 *         - 同时清除释放状态位，避免状态冲突
 * @param  key: 按键索引 (KEY_11 ~ KEY_33)
 * @retval None
 */
void Keyboard_SetKeyPressed(KeyIndex_t key) {
    if (key < KEY_MAX) {
        gkeyboard_status.key_pressed |= (1 << key);
        gkeyboard_status.key_released &= ~(1 << key);
    }
}

/**
 * @brief  设置指定按键为释放状态
 * @note   使用位操作设置按键状态位域
 *         - 设置释放状态位为1
 *         - 清除按下状态位和长按状态位
 * @param  key: 按键索引 (KEY_11 ~ KEY_33)
 * @retval None
 */
void Keyboard_SetKeyReleased(KeyIndex_t key) {
    if (key < KEY_MAX) {
        gkeyboard_status.key_released |= (1 << key);
        gkeyboard_status.key_pressed &= ~(1 << key);
        gkeyboard_status.key_long_pressed &= ~(1 << key);
    }
}

/**
 * @brief  清除指定按键的所有状态
 * @note   将指定按键的所有状态位清零
 *         - 清除按下、释放、长按状态
 *         - 清除按键按下时间戳
 * @param  key: 按键索引 (KEY_11 ~ KEY_33)
 * @retval None
 */
void Keyboard_ClearKeyStatus(KeyIndex_t key) {
    if (key < KEY_MAX) {
        gkeyboard_status.key_pressed &= ~(1 << key);
        gkeyboard_status.key_released &= ~(1 << key);
        gkeyboard_status.key_long_pressed &= ~(1 << key);
        gkeyboard_status.key_press_time[key] = 0;
    }
}

/**
 * @brief  清除所有按键状态
 * @note   将整个按键状态管理结构体清零
 *         - 清除所有按键的按下、释放、长按状态
 *         - 清除所有按键的时间戳
 * @param  None
 * @retval None
 */
void Keyboard_ClearAllStatus(void) {
    gkeyboard_status.key_pressed = 0;
    gkeyboard_status.key_released = 0;
    gkeyboard_status.key_long_pressed = 0;
    for (int i = 0; i < KEY_MAX; i++) {
        gkeyboard_status.key_press_time[i] = 0;
    }
}

/**
 * @brief  检查指定按键是否处于按下状态
 * @note   通过位操作检查按键状态位域
 * @param  key: 按键索引 (KEY_11 ~ KEY_33)
 * @retval 1: 按键按下, 0: 按键未按下或索引无效
 */
unsigned char Keyboard_IsKeyPressed(KeyIndex_t key) {
    if (key < KEY_MAX) {
        return (gkeyboard_status.key_pressed & (1 << key)) ? 1 : 0;
    }
    return 0;
}

/**
 * @brief  检查指定按键是否处于释放状态
 * @note   通过位操作检查按键释放状态位域
 * @param  key: 按键索引 (KEY_11 ~ KEY_33)
 * @retval 1: 按键释放, 0: 按键未释放或索引无效
 */
unsigned char Keyboard_IsKeyReleased(KeyIndex_t key) {
    if (key < KEY_MAX) {
        return (gkeyboard_status.key_released & (1 << key)) ? 1 : 0;
    }
    return 0;
}

/**
 * @brief  检查指定按键是否处于长按状态
 * @note   通过位操作检查按键长按状态位域
 * @param  key: 按键索引 (KEY_11 ~ KEY_33)
 * @retval 1: 按键长按, 0: 按键未长按或索引无效
 */
unsigned char Keyboard_IsKeyLongPressed(KeyIndex_t key) {
    if (key < KEY_MAX) {
        return (gkeyboard_status.key_long_pressed & (1 << key)) ? 1 : 0;
    }
    return 0;
}

/**
 * @brief  获取当前所有按下的按键状态
 * @note   返回16位状态字，每一位代表一个按键的按下状态
 *         - bit0: KEY_11, bit1: KEY_12, ..., bit8: KEY_33
 *         - 1表示按键按下，0表示按键未按下
 * @param  None
 * @retval 按键状态位域 (0x0000 ~ 0x01FF)
 */
unsigned short Keyboard_GetPressedKeys(void) {
    return gkeyboard_status.key_pressed;
}
