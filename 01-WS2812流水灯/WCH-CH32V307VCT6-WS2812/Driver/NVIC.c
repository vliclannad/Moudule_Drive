#include "NVIC.h"





void Nvic_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);



}
