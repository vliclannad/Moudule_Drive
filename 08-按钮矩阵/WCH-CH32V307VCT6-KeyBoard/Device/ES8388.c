#include "ES8388.h"


u8 ES8388_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
void ES8388_Contrl_Init(void);
/*********************************************************************
 * @fn      ES8388_Contrl_Init
 *
 * @brief   控制I2S和SD卡的初始化
 *
 * @return  none
 */
void ES8388_Contrl_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}


u8 ES8388_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitTSturcture;
    RCC_APB2PeriphClockCmd( ES8388_IIC_GPIO_CLK , ENABLE );
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE );

    GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE); // I2C1复用到PB8和PB9
    RCC_APB1PeriphClockCmd( ES8388_IIC_CLK, ENABLE );

    GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);

    GPIO_InitStructure.GPIO_Pin = ES8388_IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( ES8388_IIC_GPIO_PORT, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = ES8388_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( ES8388_IIC_GPIO_PORT, &GPIO_InitStructure );

    I2C_InitTSturcture.I2C_ClockSpeed = ES8388_IIC_BOUND;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = 0x01;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init( ES8388_IIC, &I2C_InitTSturcture );
    I2C_Cmd( ES8388_IIC, ENABLE );

    ES8388_Contrl_Init();

    //以下为通用设置
    //复位ES8388
    ES8388_Write_Reg(0, 0x80);
    ES8388_Write_Reg(0, 0x00);
    Delay_Ms(100);				//等待复位

    ES8388_Write_Reg(0x01, 0x58); //复位模拟软件部分
    ES8388_Write_Reg(0x01, 0x50); //模拟软件部分工作
    ES8388_Write_Reg(0x02, 0xF3); //断电ADC和DAC
    ES8388_Write_Reg(0x02, 0xF0); //重新设置ADC和DAC
    ES8388_Write_Reg(0x03, 0x09);    /* 麦克风偏置电源关闭 */
    ES8388_Write_Reg(0x00, 0x06);    /* 使能参考 500K驱动使能 */
    ES8388_Write_Reg(0x04, 0x00);    /* DAC电源管理，不打开任何通道 */

    ES8388_Write_Reg(0x08, 0x00);    /* MCLK不分频 */

    ES8388_Write_Reg(0x2B, 0x80);    /* DAC控制    DACLRC与ADCLRC相同 */
    ES8388_Write_Reg(0x09, 0x88);    /* ADC L/R PGA增益配置为+24dB */
    /*ADC  数据选择为left data = left ADC,
     * right data = left ADC  音频数据为16bit */
    ES8388_Write_Reg(0x0C, 0x4C);
    ES8388_Write_Reg(0x0D, 0x02);    /* ADC配置 MCLK/采样率=256 */
    ES8388_Write_Reg(0x10, 0x00);    /* ADC数字音量控制将信号衰减 L  设置为最小！！！ */
    ES8388_Write_Reg(0x11, 0x00);    /* ADC数字音量控制将信号衰减 R  设置为最小！！！ */
    ES8388_Write_Reg(0x12, 0x00);     //关闭ACL
    ES8388_Write_Reg(0x17, 0x18);    /* DAC 音频数据为16bit */
    ES8388_Write_Reg(0x18, 0x02);    /* DAC 配置 MCLK/采样率=256 */
    ES8388_Write_Reg(0x1A, 0x00);    /* DAC数字音量控制将信号衰减 L  设置为最小！！！ */
    ES8388_Write_Reg(0x1B, 0x00);    /* DAC数字音量控制将信号衰减 R  设置为最小！！！ */
    ES8388_Write_Reg(0x27, 0xB8);    /* L混频器 */
    ES8388_Write_Reg(0x2A, 0xB8);    /* R混频器 */
    Delay_Ms(100);
    return 0;
}
u8 ES8388_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT){
    u16 counter = 0xffff;
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT ) ){
        counter--;
        if(counter == 0){
            return 1;
        }
    }
    return 0;
}
u8 ES8388_Write_Reg(u8 reg, u8 val)
{
    // 等待I2C总线空闲
    while( I2C_GetFlagStatus( ES8388_IIC, I2C_FLAG_BUSY ) != RESET ); 
    
    // 发送起始条件
    I2C_GenerateSTART( ES8388_IIC, ENABLE );
    if(ES8388_WaitEvent( ES8388_IIC, I2C_EVENT_MASTER_MODE_SELECT )) return 1;
    
    // 发送设备地址
    I2C_Send7bitAddress(ES8388_IIC,((ES8388_ADDR << 1) | 0),I2C_Direction_Transmitter);
    if(ES8388_WaitEvent( ES8388_IIC, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED )) return 1;

    // 发送寄存器地址
    while( I2C_GetFlagStatus( ES8388_IIC, I2C_FLAG_TXE ) ==  RESET );
    I2C_SendData(ES8388_IIC,reg);
    if(ES8388_WaitEvent( ES8388_IIC, I2C_EVENT_MASTER_BYTE_TRANSMITTED )) return 1;

    // 发送数据
    while( I2C_GetFlagStatus( ES8388_IIC, I2C_FLAG_TXE ) ==  RESET );
    I2C_SendData(ES8388_IIC,val & 0XFF); 
    if(ES8388_WaitEvent( ES8388_IIC, I2C_EVENT_MASTER_BYTE_TRANSMITTED )) return 1;

    // 发送停止条件
    I2C_GenerateSTOP( ES8388_IIC, ENABLE );
    return 0;
}
u8 ES8388_Read_Reg(u8 reg)
{
    u8 temp = 0;

    // 等待I2C总线空闲
    while( I2C_GetFlagStatus( ES8388_IIC, I2C_FLAG_BUSY ) != RESET );
    
    // 第一次传输：写寄存器地址
    I2C_GenerateSTART( ES8388_IIC, ENABLE );
    if(ES8388_WaitEvent( ES8388_IIC, I2C_EVENT_MASTER_MODE_SELECT )) return 0xFF;
    
    I2C_Send7bitAddress(ES8388_IIC,(ES8388_ADDR << 1) | 0X00,I2C_Direction_Transmitter);
    if(ES8388_WaitEvent( ES8388_IIC, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED )) return 0xFF;
    
    I2C_SendData(ES8388_IIC,reg);
    if(ES8388_WaitEvent( ES8388_IIC, I2C_EVENT_MASTER_BYTE_TRANSMITTED )) return 0xFF;

    // 第二次传输：读取数据
    I2C_GenerateSTART( ES8388_IIC, ENABLE );
    if(ES8388_WaitEvent( ES8388_IIC, I2C_EVENT_MASTER_MODE_SELECT )) return 0xFF;

    I2C_Send7bitAddress(ES8388_IIC,((ES8388_ADDR << 1) | 0x01),I2C_Direction_Receiver);
    if(ES8388_WaitEvent( ES8388_IIC, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED )) return 0xFF;

    // 禁用ACK并生成停止条件
    I2C_AcknowledgeConfig(ES8388_IIC, DISABLE);
    I2C_GenerateSTOP( ES8388_IIC, ENABLE );
    
    while(I2C_GetFlagStatus( ES8388_IIC, I2C_FLAG_RXNE ) ==  RESET);
    temp = I2C_ReceiveData( ES8388_IIC );
    
    // 重新使能ACK
    I2C_AcknowledgeConfig(ES8388_IIC, ENABLE);

    return temp;
}

/*********************************************************************
 * @fn      ES8388_I2S_Cfg
 *
 * @brief   配置I2S工作模式
 *
 * @param   fmt - I2S mode
 *            0 - 标准I2S
 *            1 - MSB
 *            2 - LSB
 *            3 - PCM/DSP
 *          address
 *
 * @return  none
 */
void ES8388_I2S_Cfg(u8 fmt, u8 len)
{
    fmt &= 0X03;
    len &= 0X07; //长度范围
    ES8388_Write_Reg(23, (fmt << 1) | (len << 3));	//R23,ES8388音频格式配置
}
/*********************************************************************
 * @fn      ES8388_ADDA_Cfg
 *
 * @brief   ES8388 DAC/ADC配置
 *
 * @param   dacen -
 *            1 - dac enable
 *            0 - dac disable
 *          adcen -
 *            1 - adc enable
 *            0 - adc disable
 *
 * @return  none
 */
void ES8388_ADDA_Cfg(uint8_t dacen, uint8_t adcen)
{
    uint8_t tempreg = 0;
   
   tempreg |= ((!dacen) << 0);
   tempreg |= ((!adcen) << 1);
   tempreg |= ((!dacen) << 2);
   tempreg |= ((!adcen) << 3);
   ES8388_Write_Reg(0x02, tempreg);
}
/**
*@brief        ES8388 DAC输出通道配置
*@param        o1en : 通道1使能(1)/禁止(0)
*@param        o2en : 通道2使能(1)/禁止(0)
*@retval       无
*/
void ES8388_Output_Cfg(uint8_t o1en, uint8_t o2en)
{
    uint8_t tempreg = 0;
   tempreg |= o1en * (3 << 4);
   tempreg |= o2en * (3 << 2);
   ES8388_Write_Reg(0x04, tempreg);
}
/*********************************************************************
 * @fn      ES8388_Input_Cfg
 *
 * @brief   ES8388 ADC输入通道选择
 *
 * @param   in -
 *            0-通道1输入
 *            1-通道2输入
 *
 * @return  none
 */
void ES8388_Input_Cfg(u8 in)
{
	ES8388_Write_Reg(0x0A,(5*in)<<4);	//ADC1 输入通道选择L/R	INPUT1
}

void ES8388_hpvol_set(uint8_t volume)
{
    if (volume > 33)
    {
       volume = 33;
    }

   ES8388_Write_Reg(0x2E, volume);
   ES8388_Write_Reg(0x2F, volume);
}
/**
*@brief        设置喇叭音量
*@param        volume : 音量大小(0 ~ 33)
*@retval       无
*/
void ES8388_spkvol_set(uint8_t volume)
{
    if (volume > 33)
    {
       volume = 33;
    }

   ES8388_Write_Reg(0x30, volume);
   ES8388_Write_Reg(0x31, volume);
}

//关闭ES8388各电源
void ES8388_close(void)
{
    ES8388_Init();
    ES8388_Write_Reg(0, 0x80);
    ES8388_Write_Reg(0, 0x00);
    Delay_Ms(100);				//等待复位
    ES8388_Write_Reg(0x01, 0x5D);
    ES8388_Write_Reg(0x02, 0x1F); 
    ES8388_Write_Reg(0x04, 0xC0); 
}