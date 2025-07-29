#ifndef __ADLX345_H     //防止重复定义（_ADLX345_H  开头)
#define __ADLX345_H

#include "ch32v30x.h"

#include "IIC.h"

//CS高电平启用IIC模式
//宏定义
#define   ADXL345_ADDR    0x53    //该数据取决于地址线选择,SD0接地地址为0x53，接3.3V地址为0x1D(高7位）


#define DEVICE_ID       0X00    //器件ID,0XE5
#define THRESH_TAP      0X1D    //敲击阀值
#define OFSX            0X1E
#define OFSY            0X1F
#define OFSZ            0X20
#define DUR             0X21
#define Latent          0X22
#define Window          0X23
#define THRESH_ACK      0X24
#define THRESH_INACT    0X25
#define TIME_INACT      0X26
#define ACT_INACT_CTL   0X27
#define THRESH_FF       0X28
#define TIME_FF         0X29
#define TAP_AXES        0X2A
#define ACT_TAP_STATUS  0X2B
#define BW_RATE         0X2C    //数据速率以及功率模式配置
#define POWER_CTL       0x2D    //省电特性配置

#define INT_ENABLE      0X2E
#define INT_MAP         0X2F
#define INT_SOURCE      0X30
#define DATA_FORMAT     0x31    //数据格式控制
#define DATA_X0         0X32
#define DATA_X1         0X33
#define DATA_Y0         0X34
#define DATA_Y1         0X35
#define DATA_Z0         0X36
#define DATA_Z1         0X37
#define FIFO_CTL        0X38
#define FIFO_STATUS     0X39

//SD0接3.3V,0x3A写入，0x3B读取  SD0接地，0xA6写入，0xA7读取
#define ADXL_READ    0XA7
#define ADXL_WRITE   0XA6

void ADLX345_Init();
void ADLX345_GetData(uint8_t *data,uint8_t num);

#endif    //防止重复定义（_ADLX345_H  结尾)
