#ifndef __AHT20_H
#define __AHT20_H

#include "debug.h"

#define AHT20_SDA_PIN       GPIO_Pin_6
#define AHT20_SCL_PIN       GPIO_Pin_7
#define AHT20_GPIO_PORT     GPIOB
#define AHT20_GPIO_CLK      RCC_APB2Periph_GPIOB


#define AHT20_ADDRESS_W     0x70 //读地址
#define AHT20_ADDRESS_R     0x71 //写地址


#define  AIIC_SDA_H  GPIO_SetBits(AHT20_GPIO_PORT,AHT20_SDA_PIN)    //配置SDA接口高电平

#define  AIIC_SDA_L  GPIO_ResetBits(AHT20_GPIO_PORT,AHT20_SDA_PIN)  //配置SDA接口低电平



#define  AIIC_SCL_H  GPIO_SetBits(AHT20_GPIO_PORT,AHT20_SCL_PIN)    //配置SCL接口高电平

#define  AIIC_SCL_L  GPIO_ResetBits(AHT20_GPIO_PORT,AHT20_SCL_PIN)  //配置SCL接口低电平



#define  AIIC_SDA_READ()  GPIO_ReadInputDataBit(AHT20_GPIO_PORT, AHT20_SDA_PIN)  //读SDA口线状态

void AHT21_Init(void);

/*******************************************************************************
 * 参数：无
 * 函数功能：软复位
*******************************************************************************/
void AHT21_WriteReset(void);//软复位AHT21

/*******************************************************************************
 * 参数：无
 * 函数功能：发送测量命令，读取温度、湿度
 * 返回值：TH数组
*******************************************************************************/
float* AHT21_ReadTemperatureHumidity(void);//传回TH地址

/*******************************************************************************
 * 参数：无
 * 函数功能：写入[温湿度触发测量指令]给AHT21
*******************************************************************************/
void AHT21_WriteMeasure(void); //写入触发测量指令

/*******************************************************************************
 * 参数：无
 * 函数功能：AHT21 写入[初始化 寄存器指令]
*******************************************************************************/
void AHT21_WriteInit(void); //AHT21 写入[初始化 寄存器指令]

/*******************************************************************************
 * 参数：无
 * 函数功能：读取 AHT21 状态
*******************************************************************************/
uint8_t AHT21_ReadState(void); // 读取 AHT21 状态

/*******************************************************************************
 * 参数：命令
 * 函数功能：通用 AHT21写入 [命令]
*******************************************************************************/
void AHT21_WriteCommand(uint8_t Command);




#endif
