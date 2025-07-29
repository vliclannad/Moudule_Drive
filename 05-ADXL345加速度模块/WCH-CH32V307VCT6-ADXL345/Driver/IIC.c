#include "IIC.h"



/*******************************************************************************
* Function Name  : IIC_Init
* Description    : Initializes the IIC peripheral.
* Input          : None
* Return         : None
*******************************************************************************/
void IIC_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitTSturcture;
    RCC_APB2PeriphClockCmd( USER_IIC_GPIO_CLK , ENABLE );
    RCC_APB1PeriphClockCmd( USER_IIC_CLK, ENABLE );

    GPIO_InitStructure.GPIO_Pin = USER_IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( USER_IIC_GPIO_PORT, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = USER_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( USER_IIC_GPIO_PORT, &GPIO_InitStructure );

    I2C_InitTSturcture.I2C_ClockSpeed = USER_IIC_BOUND;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = USER_IIC_ADDR;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init( USER_IIC, &I2C_InitTSturcture );
    I2C_Cmd( USER_IIC, ENABLE );
}

u8 IIC_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT){
    u16 counter = 0xffff;
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT ) ){
        counter--;
        if(counter == 0){
            return 1;
        }
    }
    return 0;
}
u8 IIC_WriteLen(u8 addr, u8 reg, u8 len, u8 *buf)
{
    u8 i = 0;
    I2C_GenerateSTART(USER_IIC, ENABLE);

    if(IIC_WaitEvent(USER_IIC, I2C_EVENT_MASTER_MODE_SELECT))
        return 1;
    I2C_Send7bitAddress(USER_IIC, (addr<<1) | 0, I2C_Direction_Transmitter); // 发送器件地址+写命令

    if(IIC_WaitEvent(USER_IIC, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        return 1; // 等待应答

    while (I2C_GetFlagStatus(USER_IIC, I2C_FLAG_TXE) == RESET)
        ;
    I2C_SendData(USER_IIC, reg); // 写寄存器地址

    while (i < len)
    {
        if (I2C_GetFlagStatus(USER_IIC, I2C_FLAG_TXE) != RESET)
        {
            I2C_SendData(USER_IIC, buf[i]); // 发送数据
            i++;
        }
    }
    while (I2C_GetFlagStatus(USER_IIC, I2C_FLAG_TXE) == RESET);

    I2C_GenerateSTOP(USER_IIC, ENABLE);

    return 0;
}

u8 IIC_ReadLen(u8 addr, u8 reg, u8 len, u8 *buf)
{
    u8 i = 0;


    I2C_GenerateSTART(USER_IIC, ENABLE);

    if(IIC_WaitEvent(USER_IIC, I2C_EVENT_MASTER_MODE_SELECT))return 1       ;
    I2C_Send7bitAddress(USER_IIC, (addr << 1) | 0X00, I2C_Direction_Transmitter); //发送器件地址+写命令

    if(IIC_WaitEvent(USER_IIC, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))return 1       ; //等待应答

    I2C_SendData(USER_IIC, reg); //写寄存器地址

    I2C_GenerateSTART(USER_IIC, ENABLE);
    if(IIC_WaitEvent(USER_IIC, I2C_EVENT_MASTER_MODE_SELECT))return 1       ;

    I2C_Send7bitAddress(USER_IIC, ((addr << 1) | 0x01), I2C_Direction_Receiver); //发送器件地址+读命令
    if(IIC_WaitEvent(USER_IIC, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))return 1       ; //等待应答

    while (i < len)
    {
        if (I2C_GetFlagStatus(USER_IIC, I2C_FLAG_RXNE) != RESET)
        {
            if (i == (len - 1))
            {
                I2C_AcknowledgeConfig(USER_IIC, DISABLE);//关闭自动ACK,不IIC总线不会被释放
                buf[i] = I2C_ReceiveData(USER_IIC); //读数据,发送nACK
            }
            else
            {
                buf[i] = I2C_ReceiveData(USER_IIC); //读数据,发送ACK
            }
            i++;
        }
    }

    
    I2C_AcknowledgeConfig(USER_IIC, ENABLE);//重新启动自动ACK,不然程序会停在接收
    I2C_GenerateSTOP(USER_IIC, ENABLE); //产生一个停止条件

    return 0;
}

u8 IIC_ReadByte(u8 addr,u8 reg)
{
    u8 res;
    I2C_GenerateSTART( USER_IIC, ENABLE );

    while( !I2C_CheckEvent( USER_IIC, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress(USER_IIC,(addr << 1) | 0X00,I2C_Direction_Transmitter); //发送器件地址+写命令

    if(IIC_WaitEvent( USER_IIC, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) )return 1;  //等待应答
    I2C_SendData(USER_IIC,reg);         //写寄存器地址

    I2C_GenerateSTART( USER_IIC, ENABLE );
    while( !I2C_CheckEvent( USER_IIC, I2C_EVENT_MASTER_MODE_SELECT ) );

    I2C_Send7bitAddress(USER_IIC,((addr << 1) | 0x01),I2C_Direction_Receiver);//发送器件地址+读命令
    while( !I2C_CheckEvent( USER_IIC, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) ); //等待应答

    I2C_AcknowledgeConfig( USER_IIC, DISABLE );

    while( I2C_GetFlagStatus( USER_IIC, I2C_FLAG_RXNE ) ==  RESET );
    res = I2C_ReceiveData( USER_IIC ); //读数据,发送nACK

    I2C_AcknowledgeConfig(USER_IIC, ENABLE);
    I2C_GenerateSTOP( USER_IIC, ENABLE );//产生一个停止条件
    return res;
}

u8 IIC_WriteByte(u8 addr,uint8_t reg, uint8_t data)
{
    uint8_t res = 0;

    I2C_GenerateSTART(USER_IIC, ENABLE);

    while (1) {
        if (IIC_WaitEvent(USER_IIC, I2C_EVENT_MASTER_MODE_SELECT)) {
            res = 1;
            break;
        }
        I2C_Send7bitAddress(USER_IIC, ((addr << 1) | 0), I2C_Direction_Transmitter);

        if (IIC_WaitEvent(USER_IIC, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
            res = 1;
            break;
        }
        I2C_SendData(USER_IIC, reg);

        if (IIC_WaitEvent(USER_IIC, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
            res = 1;
            break;
        }

        while (I2C_GetFlagStatus(USER_IIC, I2C_FLAG_TXE) == RESET);
        I2C_SendData(USER_IIC, data);

        while (I2C_GetFlagStatus(USER_IIC, I2C_FLAG_TXE) == RESET);
        break;
    }
    I2C_GenerateSTOP(USER_IIC, ENABLE);

    return res;
}


