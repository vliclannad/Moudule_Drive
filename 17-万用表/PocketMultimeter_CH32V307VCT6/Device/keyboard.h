#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "debug.h"

#define KEY_ROW_PORT        GPIOD
#define KEY_ROW1_PIN        GPIO_Pin_10
#define KEY_ROW2_PIN        GPIO_Pin_11
#define KEY_ROW3_PIN        GPIO_Pin_12
#define KEY_ROW_GPIO_CLK    RCC_APB2Periph_GPIOD

#define KEY_ROW_PORTSOURCE       GPIO_PortSourceGPIOD
#define KEY_ROW1_PINSOURCE       GPIO_PinSource10
#define KEY_ROW1_EXTILINE         EXTI_Line10

#define KEY_ROW2_PINSOURCE       GPIO_PinSource11
#define KEY_ROW2_EXTILINE         EXTI_Line11

#define KEY_ROW3_PINSOURCE       GPIO_PinSource12
#define KEY_ROW3_EXTILINE         EXTI_Line12

#define KEY_COL_PORT        GPIOE
#define KEY_COL1_PIN        GPIO_Pin_3
#define KEY_COL2_PIN        GPIO_Pin_4
#define KEY_COL3_PIN        GPIO_Pin_5
#define KEY_COL_GPIO_CLK    RCC_APB2Periph_GPIOE

#define KEYBOARD_EXTI_IRQn            EXTI15_10_IRQn
#define KEYBOARD_EXTI_IRQHandler      EXTI15_10_IRQHandler

// 按键编号定义 (行列索引)
typedef enum {
    KEY_11 = 0,  // 第1行第1列
    KEY_12 = 1,  // 第1行第2列
    KEY_13 = 2,  // 第1行第3列
    KEY_21 = 3,  // 第2行第1列
    KEY_22 = 4,  // 第2行第2列
    KEY_23 = 5,  // 第2行第3列
    KEY_31 = 6,  // 第3行第1列
    KEY_32 = 7,  // 第3行第2列
    KEY_33 = 8,  // 第3行第3列
    KEY_MAX = 9
} KeyIndex_t;

// 按键状态结构体
typedef struct {
    unsigned short key_pressed;       // 按键按下状态位域 (bit0-bit8对应9个按键)
    unsigned short key_released;      // 按键释放状态位域
    unsigned short key_long_pressed;  // 长按状态位域
    unsigned long key_press_time[9];  // 每个按键的按下时间戳
} KeyboardStatus_t;

// 按键状态检测宏定义
#define IS_KEY_PRESSED(key)      (gkeyboard_status.key_pressed & (1<<(key)))
#define IS_KEY_RELEASED(key)     (gkeyboard_status.key_released & (1<<(key)))
#define IS_KEY_LONG_PRESSED(key) (gkeyboard_status.key_long_pressed & (1<<(key)))

// 全局按键状态变量
extern KeyboardStatus_t gkeyboard_status;

void KEYBOARD_EXTI_IRQHandler(void);

void Keyboard_Init(void);
void Keyboard_ScanCol_Mode(void);
void Keyboard_ScanRow_Mode(void);

// 按键状态管理函数
void Keyboard_SetKeyPressed(KeyIndex_t key);
void Keyboard_SetKeyReleased(KeyIndex_t key);
void Keyboard_ClearKeyStatus(KeyIndex_t key);
void Keyboard_ClearAllStatus(void);
unsigned char Keyboard_IsKeyPressed(KeyIndex_t key);
unsigned char Keyboard_IsKeyReleased(KeyIndex_t key);
unsigned char Keyboard_IsKeyLongPressed(KeyIndex_t key);
unsigned short Keyboard_GetPressedKeys(void);
#endif
