#include "ov7670.h"

//写复位
void FIFO_ResetW(void)
{
    CAM_WRST_CLR;
    Delay_Us(1);
    CAM_WRST_SET;
    Delay_Us(1);

}
//读指针复位
void FIFO_ResetR(void)
{
    CAM_RRST_CLR;
    //需要两个CLK周期，第二个周期上升沿是在RRST拉高之后
    CAM_RRST_CLR;
    CAM_CLK_CLR;
    CAM_CLK_SET;
    CAM_CLK_CLR;

    CAM_RRST_SET;
    CAM_CLK_SET;
}

//从FIFO中读取数据
void CAM_ReadData(uint8_t *data,uint16_t len)
{
    uint16_t i;
    FIFO_ResetR();//读指针复位
    CAM_OE_CLR();//允许读取

    for()
}


void sccb_send1(uint8_t data)
{
    uint8_t i;
    for(i=0;i<=8;i++)
    {
        data&0x08?CAM_SDA_SET:CAM_SDA_CLR;
        data<<1;
        Delay_Us(50);
        CAM_CLK_SET;//clk拉高
        Delay_Us(50);
        CAM_CLK_CLR;//clk拉低
    }
}

void sccb_rec1(uint8_t *data)
{
    uint8_t i;
    CAM_SDA_SET;//sda拉高
    for(i=0;i<8;i++)
    {
        CAM_CLK_SET;
        Delay_Us(50);
        *data<<1;
        GPIO_ReadInputDataBit(CAM_SDA_PORT,CAM_SDA_PIN)?(*data|=1):(*data|=0);
        CAM_CLK_CLR;
        Delay_Us(50);
    }

    
}