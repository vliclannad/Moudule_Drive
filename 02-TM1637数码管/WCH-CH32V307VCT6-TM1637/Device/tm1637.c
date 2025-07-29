/*
GND
CLK
DIO
5V
*/

#include "tm1637.h"    //����TM1637ͷ�ļ�
//�ļ���ȫ�ֱ���

uint8_t tab[22] =
{
        0x3F,    //0
        0x06,    //1
        0x5B,    //2
        0x4F,    //3
        0x66,    //4
        0x6D,    //5
        0x7D,    //6
        0x07,    //7
        0x7F,    //8
        0x6F,    //9
        0x77,    //10/A
        0x7C,    //11/B
        0x58,    //12/C
        0x5E,    //13/D
        0x79,    //14/E
        0x71,    //15/F
        0x76,    //16/H
        0x38,    //17/L
        0x54,    //18/U
        0x73,    //19/P
        0x3E,    //20/U
        0x00,    //21/��
};
uint16_t CLK_PIN;
uint16_t DIO_PIN;
//�ڲ���������
void TM1637_start(void);
uint8_t TM1637_ack(void);
void TM1637_stop(void);
void TM1637_Write(uint8_t Data);

//====================�ӿں�������=========================================
//======================================================================
//�������ƣ�TM1637_Init
//����������tm1637�õ�����������
//�������أ���
//���ܸ�Ҫ����TM1637��ʼ��
//======================================================================
void TM1637_Init()
{

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(TM1637_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = TM1637_CKL_PIN|TM1637_DIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TM1637_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(TM1637_PORT, TM1637_CKL_PIN, SET);
    GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,SET);
}
//======================================================================
//�������ƣ�tm1637_Display
//����������a,b,c,d�ֱ��ʾ�ڵ�1,2,3,4λ���������ʾ������
//       a1,b1,c1,d1�ֱ��ʾ�Ƿ���ʾ����֮��ĵ㡣1��ʾ��ʾ��0��ʾ����ʾ
//�������أ���
//���ܸ�Ҫ����TM1637д�봫���һ���ֽڵ�����
//======================================================================
void TM1637_Display(uint8_t a,uint8_t a1,uint8_t b,uint8_t b1,uint8_t c,
		uint8_t c1,uint8_t d,uint8_t d1)
{
	TM1637_start();
	TM1637_Write(0x40);    //д���ݵ���ʾ�Ĵ���+�Զ���ַ��1+��ͨģʽ
	TM1637_ack();
	TM1637_stop();
	TM1637_start();
	TM1637_Write(0xc0);    //������ʾ�׵�ַ����һ��LED
	TM1637_ack();

	TM1637_Write(tab[a]|a1<<7);
	TM1637_ack();
	TM1637_Write(tab[b]|b1<<7);    //hΪ1ʱ��ʾʱ���м������
	TM1637_ack();
	TM1637_Write(tab[c]|c1<<7);
	TM1637_ack();
	TM1637_Write(tab[d]|d1<<7);
	TM1637_ack();

	TM1637_stop();
	TM1637_start();
	TM1637_Write(0x89);    //��ʾ���ƣ�����ʾ��2/16����
	TM1637_ack();
	TM1637_stop();
}
//==========================�ڲ���������===================================
//======================================================================
//�������ƣ�TM1637_start
//������������
//�������أ���
//���ܸ�Ҫ��TM1637��ʼ�������ݡ�CLKΪ�ߣ�DIO�ɸ߱�ͣ��������뿪ʼ
//======================================================================
void TM1637_start(void)
{
     GPIO_WriteBit(TM1637_PORT, TM1637_CKL_PIN, SET);
     GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,SET);
     Delay_Ms(2);
	 GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,RESET);//CLKΪ�ߣ�DIO�ɸ߱�ͣ��������뿪ʼ
}

//======================================================================
//�������ƣ�TM1637_ack
//������������
//�������أ�
//���ܸ�Ҫ��TM1637Ӧ������
//======================================================================
uint8_t TM1637_ack(void)
{
    uint8_t ackBit;
    //GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,SET);
    GPIO_WriteBit(TM1637_PORT,TM1637_CKL_PIN,RESET);
    Delay_Ms(2);
    GPIO_WriteBit(TM1637_PORT,TM1637_CKL_PIN,SET);
    ackBit = GPIO_ReadInputDataBit(TM1637_PORT, TM1637_DIO_PIN);
    Delay_Ms(2);
    GPIO_WriteBit(TM1637_PORT,TM1637_CKL_PIN,RESET);
    Delay_Ms(2);
    return ackBit;
}

//======================================================================
//�������ƣ�TM1637_stop
//������������
//�������أ���
//���ܸ�Ҫ��TM1637����һλ������ɡ�CLKΪ��ʱ��DIO�ɵͱ��
//======================================================================
void TM1637_stop(void)
{
    GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,RESET);
    Delay_Ms(2);
    GPIO_WriteBit(TM1637_PORT,TM1637_CKL_PIN,SET);
    Delay_Ms(2);
	GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,SET);
	Delay_Ms(2);
}

//======================================================================
//�������ƣ�TM1637_Write
//����������Data:Ҫ��ʾ��һ���ֽڵ�����
//�������أ���
//���ܸ�Ҫ����TM1637д�봫���һ���ֽڵ�����
//======================================================================
void TM1637_Write(uint8_t Data)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
            //CLKΪ��ʱ��DIO�ϲſ��ԴӴ�����
	    GPIO_WriteBit(TM1637_PORT,TM1637_CKL_PIN,RESET);
		if(Data & 0x01)
		    GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,SET);
		else
		    GPIO_WriteBit(TM1637_PORT,TM1637_DIO_PIN,RESET);
		Delay_Ms(2);
		Data=Data>>1;
		GPIO_WriteBit(TM1637_PORT,TM1637_CKL_PIN,SET);
		Delay_Ms(2);
	}
}
