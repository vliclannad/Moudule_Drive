#include "motor.h"

//������������AM32����̼���PWM����
//AM32�����PWM���Ʒ�Χ��1100us-1900us
//�ϵ����Ҫά�ֵ���1000us��PWM�źţ�֮������PWM�ź���1000us-1100us֮��,ά��1s���ϲ��ܽ������
//�������������PWM�ź���1100us-1900us֮����Ƶ��ת��





// ȫ�ֱ����洢��ǰ����ֵ
static uint8_t g_current_throttle = 0;  // ��ǰ����ֵ 0-100%

/**
 * @brief  ���PWM���Ƴ�ʼ��
 * @note   ��ʼ��TIM3��PWM������ܣ����ڿ���AM32���
 *         - ����GPIOΪ�����������ģʽ
 *         - ����TIM3ΪPWMģʽ�����Ƶ��50Hz
 *         - ֧��1100us-1900us�������ȿ���
 * @param  arr: �Զ���װ��ֵ������PWMƵ�� (����1440����Ӧ50Hz)
 * @param  psc: Ԥ��Ƶֵ�����arr����Ƶ�� (����1000)
 * @param  pulse: ��ʼ������ (����900��������С����)
 * @retval None
 */
void Motor_Init(uint16_t arr, uint16_t psc, uint16_t pulse) {


    //GPIO��ʼ��
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(MOTOR_GPIO_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(MOTOR_TIM_CLK, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);//���ָ���


    GPIO_InitStructure.GPIO_Pin = MOTOR_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(MOTOR_GPIO_PORT, &GPIO_InitStructure);

    //PWM��ʼ��
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period = arr - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc - 1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//��ʱ�������������¼�


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
 * @brief  ����������������
 * @note   ����AM32����Ľ���ʱ���������
 *         - ����PWM�źŵ�1050us�����Ź���λ�ã�
 *         - ά��2������ʱ��������
 *         - ����ȫ������ֵΪ0
 * @param  None
 * @retval None
 */
void Motor_Start(void) {
    TIM_SetCompare1(MOTOR_TIM, 1050);//���Ź���
    g_current_throttle = 0;  // ����ȫ�ֱ���
    Delay_Ms(2000);//���Ź���1s���ϣ�����ESC
}

/**
 * @brief  ֹͣ���
 * @note   ����ֹͣ������������
 *         - ����PWM�źŵ�900us��������С���ţ�
 *         - �ȴ�2��ȷ�������Ӧ
 *         - ����ȫ������ֵΪ0
 * @param  None
 * @retval None
 */
void Motor_Stop(void) {
    // ֱ������PWM�ź�С������
    TIM_SetCompare1(MOTOR_TIM, 900); // ����PWMռ�ձȵ�900us
    g_current_throttle = 0;  // ����ȫ�ֱ���
    Delay_Ms(2000); // �ȴ�2�룬ȷ��ESC��Ӧ
}

/**
 * @brief  ����ֹͣ���
 * @note   ƽ���ؼ���PWM������ֱ�����ֹͣ
 *         - ��ȡ��ǰPWM������
 *         - ÿ�μ���50us�����100ms
 *         - ȷ�����������Ȳ�����900us
 * @param  None
 * @retval None
 */
void Motor_StopGradual(void) {
    //�𽥼���ռ�ձȵ�900
    uint16_t current_pulse = TIM_GetCapture1(MOTOR_TIM);
    while (current_pulse > 900) {
        current_pulse -= 50; // ÿ�μ���50us
        if (current_pulse < 900) {
            current_pulse = 900; // ȷ��������900us
        }
        TIM_SetCompare1(MOTOR_TIM, current_pulse);
        Delay_Ms(100); // ÿ�μ��ٺ���ʱ100ms
    }
}

/**
 * @brief  ���õ������ֵ
 * @note   ֱ�����õ�������Űٷֱ�
 *         - ���ٷֱ�ת��Ϊ��Ӧ��PWM������
 *         - 0%��Ӧ1100us��100%��Ӧ1900us
 *         - ����ȫ�����ű���
 * @param  throttle_value: ����ֵ (0-100%)
 * @retval None
 */
void Motor_SetThrottle(uint8_t throttle_value) {
    if (throttle_value > 100) {
        throttle_value = 100; // �������ֵΪ100
    }
    
    // ����ȫ������ֵ
    g_current_throttle = throttle_value;
    
    // �����ȷ�Χ��1100us-1900us
    // 1100us = 1100������ֵ (0%����)
    // 1900us = 1900������ֵ (100%����)
    uint16_t pulse = 1100 + (800 * throttle_value) / 100;
    if(throttle_value == 0) {
        pulse = 900; // �������Ϊ0������Ϊ900us
    }

    TIM_SetCompare1(MOTOR_TIM, pulse); // ����PWMռ�ձ�
}

/**
 * @brief  ������������ֵ
 * @note   ƽ���شӵ�ǰ����ֵ�仯��Ŀ������ֵ
 *         - ����ͻȻ��PWM�仯�Ե����ɳ��
 *         - �ṩ��ƽ���Ŀ�������
 * @param  target_throttle: Ŀ������ֵ (0-100)
 * @param  step_delay_ms: ÿ��֮�����ʱ (����10-50ms)
 * @retval None
 */
void Motor_SetThrottleGradual(uint8_t target_throttle, uint16_t step_delay_ms) {
    if (target_throttle > 100) {
        target_throttle = 100;
    }
    
    // ֱ��ʹ��ȫ�ֱ�����ȡ��ǰ����ֵ
    uint8_t current_throttle = g_current_throttle;
    
    // ȷ���仯����
    int8_t step = (target_throttle > current_throttle) ? 1 : -1;
    
    // �𲽱仯��Ŀ��ֵ
    while (current_throttle != target_throttle) {
        current_throttle += step;
        
        // �߽���
        if (current_throttle > 100) current_throttle = 100;
        if (current_throttle < 0) current_throttle = 0;
        
        // �����µ�����ֵ
        Motor_SetThrottle(current_throttle);
        
        // ��ʱ
        Delay_Ms(step_delay_ms);
    }
}
/**
 * @brief  ���Ų�������
 * @note   ÿ�ε��ö������step_value��ֵ��������
 *         - ֱ��ʹ��ȫ�ֱ�����ȡ��ǰ����ֵ
 *         - ��������������ֵ
 *         - �Զ�������0-100%��Χ��
 *         - ���������Ϣ��ʾ�仯����
 * @param  step_value: ����ֵ (����1-10)
 * @retval None
 */
void Motor_ThrottleStepUp(uint8_t step_value) {
    // ֱ��ʹ��ȫ�ֱ�����ȡ��ǰ����ֵ
    uint8_t current_throttle = g_current_throttle;
    
    // �����µ�����ֵ
    uint16_t new_throttle = current_throttle + step_value;
    
    // ������0-100%��Χ��
    if (new_throttle > 100) {
        new_throttle = 100;
    }
    
    // �����µ�����ֵ
    Motor_SetThrottleGradual((uint8_t)new_throttle, MOTOR_THROTTLE_STEP_DELAY);
    
    // ��ӡ������Ϣ
    printf("Throttle: %d%% -> %d%% (Step +%d)\r\n", 
           current_throttle, new_throttle, step_value);
}

/**
 * @brief  ���Ų�������
 * @note   ÿ�ε��ö������step_value��ֵ��������
 *         - ֱ��ʹ��ȫ�ֱ�����ȡ��ǰ����ֵ
 *         - ��������������ֵ
 *         - �Զ�������0-100%��Χ��
 * @param  step_value: ����ֵ (����1-10)
 * @retval None
 */
void Motor_ThrottleStepDown(uint8_t step_value) {
    // ֱ��ʹ��ȫ�ֱ�����ȡ��ǰ����ֵ
    uint8_t current_throttle = g_current_throttle;
    
    // �����µ�����ֵ
    int16_t new_throttle = current_throttle - step_value;
    
    // ������0-100%��Χ��
    if (new_throttle < 0) {
        new_throttle = 0;
    }
    
    // �����µ�����ֵ
    Motor_SetThrottleGradual((uint8_t)new_throttle, MOTOR_THROTTLE_STEP_DELAY);
    
    // ��ӡ������Ϣ
    printf("Throttle: %d%% -> %d%% (Step -%d)\r\n", 
           current_throttle, new_throttle, step_value);
}

/**
 * @brief  ��ȡ��ǰ����ֵ
 * @note   ���ص�ǰ�洢������ֵ
 * @param  None
 * @retval ��ǰ����ֵ (0-100%)
 */
uint8_t Motor_GetCurrentThrottle(void) {
    return g_current_throttle;
}