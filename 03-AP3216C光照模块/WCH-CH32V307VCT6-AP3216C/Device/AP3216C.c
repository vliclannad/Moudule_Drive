#include "AP3216C.h"
#include "debug.h"
#include "IIC.h"
//���մ�����

void AP3216C_Init(void)
{
    IIC_Init();                      //��ʼ��IIC
    IIC_WriteByte(AP3216C_ADDR, 0x00, 0x04);  //��λģ��
    Delay_Ms(20);
    IIC_WriteByte(AP3216C_ADDR, 0x00, 0x01);   //����ALS

}

void AP3216C_ReadData(uint16_t* alsData)
{
    uint8_t temp[2];
    temp[0] = IIC_ReadByte(AP3216C_ADDR,0X0C);//����ǿ�ȵ�λ
    temp[1] = IIC_ReadByte(AP3216C_ADDR,0X0D);//����ǿ�ȸ�λ

    *alsData = ((uint16_t)temp[1] << 8) | temp[0];                //��ȡALS������������
}

