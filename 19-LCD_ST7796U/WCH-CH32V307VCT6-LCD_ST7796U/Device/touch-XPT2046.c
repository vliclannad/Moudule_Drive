#include "touch-XPT2046.h"





static void Touch_WriteCMD(uint8_t cmd);
static uint16_t Touch_ReadCMD(void);

void Touch_Init()
{
    //模拟SPI的gpio初始化
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(TOUCH_CS_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(TOUCH_IRQ_RCC,ENABLE);
    RCC_APB2PeriphClockCmd(TOUCH_CLK_RCC,ENABLE);
    RCC_APB2PeriphClockCmd(TOUCH_MOSI_RCC,ENABLE);
    RCC_APB2PeriphClockCmd(TOUCH_MISO_RCC,ENABLE);


    GPIO_InitStructure.GPIO_Pin   = TOUCH_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(TOUCH_CS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = TOUCH_CLK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(TOUCH_CLK_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = TOUCH_MOSI_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(TOUCH_MOSI_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = TOUCH_MISO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_Init(TOUCH_MISO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = TOUCH_IRQ_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(TOUCH_IRQ_PORT,&GPIO_InitStructure);

    TOUCH_CS_CLR;


}

static void Touch_WriteCMD(uint8_t cmd)
{
    uint8_t i;
    TOUCH_MOSI_CLR;
    TOUCH_CLK_CLR;
    
    for(i=0;i<8;i++)
    {
        ((cmd >>(7-i))&0x01)?TOUCH_MOSI_SET:TOUCH_MOSI_CLR;
        Delay_Us(5);
        TOUCH_CLK_SET;
        Delay_Us(5);
        TOUCH_CLK_CLR;
    }
}

static uint16_t Touch_ReadCMD(void)
{
    uint8_t i;
    uint16_t buf = 0;
    TOUCH_MOSI_CLR;
    TOUCH_CLK_SET;
    for(i=0;i<12;i++)
    {
        TOUCH_CLK_CLR;
        buf |= (GPIO_ReadInputDataBit(TOUCH_MISO_PORT,TOUCH_MISO_PIN))<<(11-i);
        TOUCH_CLK_SET;
    }
    return buf;
}



//获取数据
 uint16_t Touch_ReadData(uint8_t cmd)
{
    uint8_t getTimes = 20;//采样次数
    uint16_t readValue[getTimes];
    uint16_t valueTemp;//存储临时变量
    uint32_t totalValue;//存储平均值
    uint8_t i;
    uint8_t j;
    for(i=0;i<=getTimes;i++)
    {
        Touch_WriteCMD(cmd);
       // Delay_Us(10);

        readValue[i] = Touch_ReadCMD();
    }
    //按从大到小排列
    for(i=0;i<getTimes-1;i++)
    {
        for(j=i+1;j<getTimes;j++)
        {
            if(readValue[i]<readValue[j])
            {
                valueTemp =readValue[i];
                readValue[i]= readValue[j];
                readValue[j]=valueTemp;
            }
        }
        
    }
    j=getTimes - 1;
    totalValue = 0;
    //取平均值
    for(i=1;i<j;i++)
    {
        totalValue +=readValue[i];
    }
    totalValue = totalValue/(getTimes - 2);
    return totalValue;
}

void Touch_ReadData_XY(uint16_t *x,uint16_t *y)
{
    *x = Touch_ReadData(0xD0);
    Delay_Us(5);
    *y = Touch_ReadData(0X90);

}