#include "adxl345.h".
//加速度传感器
void ADLX345_Init()
{
    uint8_t data1 = 0x0B;
    //uint8_t data2 = 0x08;
    uint8_t data3 = 0x28;
    uint8_t data4 = 0x00;
    uint8_t data5 = 0x00;
    uint8_t data6 = 0x00;
    uint8_t data7 = 0x00;
    IIC_Init();//100kHz,使用IIC2
    IIC_WriteLen(ADXL345_ADDR,DATA_FORMAT,1,&data1);
    //IIC_WriteLen(ADXL345_ADDR,BW_RATE,1,&data2);//数据输出速度为100HZ
    IIC_WriteLen(ADXL345_ADDR,POWER_CTL,1,&data3);//链接使能，测量模式
    IIC_WriteLen(ADXL345_ADDR,INT_ENABLE,1,&data4);
    IIC_WriteLen(ADXL345_ADDR,OFSX,1,&data5);
    IIC_WriteLen(ADXL345_ADDR,OFSY,1,&data6);
    IIC_WriteLen(ADXL345_ADDR,OFSZ,1,&data7);
}

void ADLX345_GetData(uint8_t *data,uint8_t num)
{
  IIC_ReadLen(ADXL345_ADDR, DATA_X0, num, data);
}


