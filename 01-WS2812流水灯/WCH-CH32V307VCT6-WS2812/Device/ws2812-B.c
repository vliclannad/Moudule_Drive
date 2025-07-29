
#include "ws2812-B.h"    //����ws2812ͷ�ļ�


//====================�ڲ���������=======================================
void WS_Reset();    //������ʹ�ı���Ч��
//====================�ڲ���������=======================================


//====================�ӿں�������=======================================
//======================================================================
//�������ƣ�ws_Init
//����������port_pin������ws2812��GPIO
//�������أ���
//���ܸ�Ҫ����ʼ��ws2812��gpio��
//======================================================================
void WS_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(WS2812B_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = WS2812B_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(WS2812B_GPIO_PORT, &GPIO_InitStructure);
    Delay_Ms(10);
}

//======================================================================
//�������ƣ�ws_sendOnePix
//����������;ptr�����������ɫ���ֽ�����;num:��������
//�������أ���
//���ܸ�Ҫ������ws2812�������ɫ
//======================================================================
void WS_SendOnePix(uint8_t *ptr,uint8_t num)
{
    uint8_t i,j,temp;
    for(j=0;j<num*3;j++)//һ��������Ҫ3���ֽڵ���ɫ����
    {
        temp=ptr[j];
        for(i=0;i<8;i++)
        {
            if(temp&0x80)    //�Ӹ�λ��ʼ����
            {
                GPIO_SetBits(WS2812B_GPIO_PORT, WS2812B_GPIO_PIN);  //���͡�1����,�ߵ�ƽ750ns-1.6us,�͵�ƽ220ns-420ns
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                 __asm volatile ("nop");
                 __asm volatile ("nop");
                 __asm volatile ("nop");
                 __asm volatile ("nop");
                 __asm volatile ("nop");
                 __asm volatile ("nop");

                GPIO_ResetBits(WS2812B_GPIO_PORT, WS2812B_GPIO_PIN);

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
            }
            else             //���͡�0���룬�ߵ�ƽ220ns-420ns���͵�ƽ750ns-1.6us
            {
                GPIO_SetBits(WS2812B_GPIO_PORT, WS2812B_GPIO_PIN);
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");

                GPIO_ResetBits(WS2812B_GPIO_PORT, WS2812B_GPIO_PIN);
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");

                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");
                __asm volatile ("nop");


            }
            temp=(temp<<1);    //����һλ��������һ�ֽ�
        }
    }
    GPIO_ResetBits(WS2812B_GPIO_PORT, WS2812B_GPIO_PIN);
    Delay_Us(500);
}
//======================================================================
//�������ƣ�ws2812_reset
//����������port_pin������ws2812��GPIO
//�������أ���
//���ܸ�Ҫ������ws2812��ʹ�ı���Ч��
//======================================================================
void WS_Reset()
{
    GPIO_ResetBits(WS2812B_GPIO_PORT, WS2812B_GPIO_PIN);
    Delay_Us(500);
}

