/**
 ******************************************************************************
 * @file    keyboard.c
 * @author  RISC-V Development Team
 * @version V1.0.0
 * @date    2025-07-28
 * @brief   3x3����������������
 ******************************************************************************
 * @attention
 * 
 * ���ļ�ʵ����3x3��������������������ܣ�������
 * - GPIO���ú��ⲿ�жϳ�ʼ��
 * - ����ɨ��ģʽ�л�
 * - ����״̬��������/�ͷ�/������
 * - �����������
 * 
 * �������󲼾֣�
 *   COL1  COL2  COL3
 * ROW1 [11] [12] [13]
 * ROW2 [21] [22] [23] 
 * ROW3 [31] [32] [33]
 * 
 * ����ԭ��
 * 1. Ĭ��������Ϊ�������룬������Ϊ�͵�ƽ���
 * 2. ��������ʱ����Ӧ�����ű����ͣ������ⲿ�ж�
 * 3. �ж����л�����ɨ��ģʽ��ȷ�����尴��λ��
 * 4. ��¼����״̬���ָ���ɨ��ģʽ
 ******************************************************************************
 */

#include "keyboard.h"

// ȫ�ְ���״̬��������
KeyboardStatus_t gkeyboard_status = {0};

void KEYBOARD_EXTI_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/**
 * @brief  ���������ʼ������
 * @note   ����3x3���������GPIO���ⲿ�ж�
 *         - ����������Ϊ�������룬���ڼ�ⰴ������
 *         - ����������Ϊ�͵�ƽ����������ṩɨ���ź�
 *         - ���������ŵ��ⲿ�жϣ��½��ش���
 * @param  None
 * @retval None
 */
void Keyboard_Init(void)
{
    //GPIO��ʼ��
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};    //����ʱ��
    //����������Ϊ��������
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|KEY_ROW_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = KEY_ROW1_PIN|KEY_ROW2_PIN|KEY_ROW3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_ROW_PORT, &GPIO_InitStructure);
    //����������Ϊ�͵�ƽ�������
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|KEY_COL_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = KEY_COL1_PIN|KEY_COL2_PIN|KEY_COL3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEY_COL_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(KEY_COL_PORT, KEY_COL1_PIN|KEY_COL2_PIN|KEY_COL3_PIN);
    //���������жϳ�ʼ��,�������½����Ӧ�ж�
    GPIO_EXTILineConfig(KEY_ROW_PORTSOURCE , KEY_ROW1_PINSOURCE);
    GPIO_EXTILineConfig(KEY_ROW_PORTSOURCE , KEY_ROW2_PINSOURCE);
    GPIO_EXTILineConfig(KEY_ROW_PORTSOURCE , KEY_ROW3_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY_ROW1_EXTILINE|KEY_ROW2_EXTILINE|KEY_ROW3_EXTILINE; // �ⲿ�ж���2
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // ����Ϊ�ⲿ�ж�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; // ʹ���ⲿ�ж�
    EXTI_Init(&EXTI_InitStructure);
    //��������
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_InitStructure.NVIC_IRQChannel = KEYBOARD_EXTI_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  ��ɨ��ģʽ���ú���
 * @note   ����⵽ĳ���а�������ʱ���л�����ɨ��ģʽ��ȷ���������ĸ�����
 *         - ����������������Ϊ�͵�ƽ�������
 *         - ����������������Ϊ��������
 *         - ͨ����ȡ������״̬��ȷ������λ��
 * @param  None
 * @retval None
 */
void Keyboard_ScanCol_Mode(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    //����������Ϊ�͵�ƽ�������ģʽ
    GPIO_InitStructure.GPIO_Pin = KEY_ROW1_PIN|KEY_ROW2_PIN|KEY_ROW3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEY_ROW_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(KEY_ROW_PORT, KEY_ROW1_PIN|KEY_ROW2_PIN|KEY_ROW3_PIN);

    //����������Ϊ��������ģʽ
    GPIO_InitStructure.GPIO_Pin = KEY_COL1_PIN|KEY_COL2_PIN|KEY_COL3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_COL_PORT, &GPIO_InitStructure);
}

/**
 * @brief  ��ɨ��ģʽ���ú���
 * @note   ɨ����ɺ󣬻ָ�����ɨ��ģʽ���ȴ���һ�ΰ�������
 *         - ����������������Ϊ�������룬���ڼ�ⰴ������
 *         - ����������������Ϊ�͵�ƽ����������ṩɨ���ź�
 *         - ���ǰ��������Ĭ�Ϲ���ģʽ
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

    //����������Ϊ�͵�ƽ�������
    GPIO_InitStructure.GPIO_Pin = KEY_COL1_PIN|KEY_COL2_PIN|KEY_COL3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEY_COL_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(KEY_COL_PORT, KEY_COL1_PIN|KEY_COL2_PIN|KEY_COL3_PIN);

}

/**
 * @brief  ���������ⲿ�жϴ�����
 * @note   ������������κ�һ���а�������ʱ�������ж�
 *         - �������һ�д����ж�
 *         - �л�����ɨ��ģʽ��ȷ�����尴��λ��
 *         - ����״̬��������¼��������״̬
 *         - ����50ms�����������
 * @param  None
 * @retval None
 */
void KEYBOARD_EXTI_IRQHandler(void)
{
    //��1�жϴ���
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
    //��2�жϴ���
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
    //��3�жϴ���
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

// ==================== ����״̬������ʵ�� ====================

/**
 * @brief  ����ָ������Ϊ����״̬
 * @note   ʹ��λ�������ð���״̬λ��
 *         - ���ö�ӦλΪ1��ʾ��������
 *         - ͬʱ����ͷ�״̬λ������״̬��ͻ
 * @param  key: �������� (KEY_11 ~ KEY_33)
 * @retval None
 */
void Keyboard_SetKeyPressed(KeyIndex_t key) {
    if (key < KEY_MAX) {
        gkeyboard_status.key_pressed |= (1 << key);
        gkeyboard_status.key_released &= ~(1 << key);
    }
}

/**
 * @brief  ����ָ������Ϊ�ͷ�״̬
 * @note   ʹ��λ�������ð���״̬λ��
 *         - �����ͷ�״̬λΪ1
 *         - �������״̬λ�ͳ���״̬λ
 * @param  key: �������� (KEY_11 ~ KEY_33)
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
 * @brief  ���ָ������������״̬
 * @note   ��ָ������������״̬λ����
 *         - ������¡��ͷš�����״̬
 *         - �����������ʱ���
 * @param  key: �������� (KEY_11 ~ KEY_33)
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
 * @brief  ������а���״̬
 * @note   ����������״̬����ṹ������
 *         - ������а����İ��¡��ͷš�����״̬
 *         - ������а�����ʱ���
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
 * @brief  ���ָ�������Ƿ��ڰ���״̬
 * @note   ͨ��λ������鰴��״̬λ��
 * @param  key: �������� (KEY_11 ~ KEY_33)
 * @retval 1: ��������, 0: ����δ���»�������Ч
 */
unsigned char Keyboard_IsKeyPressed(KeyIndex_t key) {
    if (key < KEY_MAX) {
        return (gkeyboard_status.key_pressed & (1 << key)) ? 1 : 0;
    }
    return 0;
}

/**
 * @brief  ���ָ�������Ƿ����ͷ�״̬
 * @note   ͨ��λ������鰴���ͷ�״̬λ��
 * @param  key: �������� (KEY_11 ~ KEY_33)
 * @retval 1: �����ͷ�, 0: ����δ�ͷŻ�������Ч
 */
unsigned char Keyboard_IsKeyReleased(KeyIndex_t key) {
    if (key < KEY_MAX) {
        return (gkeyboard_status.key_released & (1 << key)) ? 1 : 0;
    }
    return 0;
}

/**
 * @brief  ���ָ�������Ƿ��ڳ���״̬
 * @note   ͨ��λ������鰴������״̬λ��
 * @param  key: �������� (KEY_11 ~ KEY_33)
 * @retval 1: ��������, 0: ����δ������������Ч
 */
unsigned char Keyboard_IsKeyLongPressed(KeyIndex_t key) {
    if (key < KEY_MAX) {
        return (gkeyboard_status.key_long_pressed & (1 << key)) ? 1 : 0;
    }
    return 0;
}

/**
 * @brief  ��ȡ��ǰ���а��µİ���״̬
 * @note   ����16λ״̬�֣�ÿһλ����һ�������İ���״̬
 *         - bit0: KEY_11, bit1: KEY_12, ..., bit8: KEY_33
 *         - 1��ʾ�������£�0��ʾ����δ����
 * @param  None
 * @retval ����״̬λ�� (0x0000 ~ 0x01FF)
 */
unsigned short Keyboard_GetPressedKeys(void) {
    return gkeyboard_status.key_pressed;
}
