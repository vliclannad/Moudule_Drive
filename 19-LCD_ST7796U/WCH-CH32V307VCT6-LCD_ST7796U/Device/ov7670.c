#include "ov7670.h"

//д��λ
void FIFO_ResetW(void)
{
    CAM_WRST_CLR;
    Delay_Us(1);
    CAM_WRST_SET;
    Delay_Us(1);

}
//��ָ�븴λ
void FIFO_ResetR(void)
{
    CAM_RRST_CLR;
    //��Ҫ����CLK���ڣ��ڶ�����������������RRST����֮��
    CAM_RRST_CLR;
    CAM_CLK_CLR;
    CAM_CLK_SET;
    CAM_CLK_CLR;

    CAM_RRST_SET;
    CAM_CLK_SET;
}

//��FIFO�ж�ȡ����
void CAM_ReadData(uint8_t *data,uint16_t len)
{
    uint16_t i;
    FIFO_ResetR();//��ָ�븴λ
    CAM_OE_CLR();//�����ȡ

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
        CAM_CLK_SET;//clk����
        Delay_Us(50);
        CAM_CLK_CLR;//clk����
    }
}

void sccb_rec1(uint8_t *data)
{
    uint8_t i;
    CAM_SDA_SET;//sda����
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