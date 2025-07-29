#include "infrared.h"
//»ÀÃÂ∫ÏÕ‚

void Infrared_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(INFRARED_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = INFRARED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(INFRARED_GPIO_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(INFRARED_GPIO_PORT, INFRARED_GPIO_PIN);
}

uint8_t Infrared_GetData(void)
{
    uint8_t data;
    if(GPIO_ReadInputDataBit(INFRARED_GPIO_PORT, INFRARED_GPIO_PIN)==1)
    {
        data = 1;
    }
    else {
        data = 0;
    }
    return data;
}
