#include "AP3216C.h"
#include "debug.h"
#include "IIC.h"
//光照传感器

void AP3216C_Init(void)
{
    IIC_Init();                      //初始化IIC
    IIC_WriteByte(AP3216C_ADDR, 0x00, 0x04);  //复位模块
    Delay_Ms(20);
    IIC_WriteByte(AP3216C_ADDR, 0x00, 0x01);   //开启ALS

}

void AP3216C_ReadData(uint16_t* alsData)
{
    uint8_t temp[2];
    temp[0] = IIC_ReadByte(AP3216C_ADDR,0X0C);//光照强度低位
    temp[1] = IIC_ReadByte(AP3216C_ADDR,0X0D);//光照强度高位

    *alsData = ((uint16_t)temp[1] << 8) | temp[0];                //读取ALS传感器的数据
}

