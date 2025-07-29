#include "AHT20.h"
//温湿度传感器
//*************************以下函数为内部函数******************************
/*******************************************************************************
 * 参数：命令，数据0，数据1
 * 函数功能：通用 AHT21写入 [命令+两字节]
*******************************************************************************/
void AIIC_Init(void);         //IIC初始化函数
void AIIC_Idle_State(void);   //IIC空闲状态
void AIIC_Start(void);        //IIC开始信号函数
void AIIC_Stop(void);         //IIC停止信号函数
void AIIC_SendByte(u8 data);  //IIC发送一个字节
u8   AIIC_ReadByte(void);     //IIC读取一个字节
u8   AIIC_WaitAck(void);      //等待响应信号（ACK或者NACK）
void AIIC_ACK(void);          //IIC发出ACK信号
void AIIC_NACK(void);         //IIC发出NACK信号


void AHT21_WriteHalfWord(uint8_t Command,uint8_t Data0,uint8_t Data1)
{
    AIIC_Start();//开始

    AIIC_SendByte(AHT20_ADDRESS_W);//从机地址＋写
    AIIC_WaitAck();//应答

    AIIC_SendByte(Command);//命令
    AIIC_WaitAck();//应答

    AIIC_SendByte(Data0);//写入Data0
    AIIC_WaitAck();//应答

    AIIC_SendByte(Data1);//写入Data1
    AIIC_WaitAck();//应答

    AIIC_Stop();//停止
}

/*******************************************************************************
 * 参数：命令
 * 函数功能：通用 AHT21写入 [命令]
*******************************************************************************/
void AHT21_WriteCommand(uint8_t Command)
{
    AIIC_Start();     //开始

    AIIC_SendByte(AHT20_ADDRESS_W);//从机地址＋写
    AIIC_WaitAck();//应答

    AIIC_SendByte(Command);//命令
    AIIC_WaitAck();//应答

    AIIC_Stop();//停止
}
/*******************************************************************************
 * 参数：无
 * 函数功能：读取 AHT21 状态
*******************************************************************************/
uint8_t AHT21_ReadState(void) // 读取 AHT21 状态
{
    uint8_t State;

    AIIC_Start();//开始

    AIIC_SendByte(AHT20_ADDRESS_W | 0x01);//从机地址＋读
    AIIC_WaitAck();//接收应答

    State=AIIC_ReadByte();//接收状态
    AIIC_NACK();//发送非应答

    AIIC_Stop();//停止
    return State;
}
/*******************************************************************************
 * 参数：无
 * 函数功能：AHT21 写入[初始化 寄存器指令]
*******************************************************************************/
void AHT21_WriteInit(void) //AHT21 写入[初始化 寄存器指令]
{
    AHT21_WriteHalfWord(0xBE,0x08,0x00);
}
/*******************************************************************************
 * 参数：无
 * 函数功能：写入[温湿度触发测量指令]给AHT21
*******************************************************************************/
void AHT21_WriteMeasure(void) //写入触发测量指令
{
    AHT21_WriteHalfWord(0xAC,0x33,0x00);
}
/*******************************************************************************
 * 参数：无
 * 函数功能：软复位
*******************************************************************************/
void AHT21_WriteReset(void)//软复位AHT21
{
    AHT21_WriteCommand(0xBA);

}
uint32_t AHT21_Pow(uint32_t Num,uint32_t Power)//幂函数（num，power）
{
    uint32_t Result = 1;
    while(Power--)
    {
        Result *= Num;
    }
    return Result;
}
/*******************************************************************************
 * 参数：无
 * 函数功能：发送测量命令，读取温度、湿度
 * 返回值：TH数组
*******************************************************************************/
float* AHT21_ReadTemperatureHumidity(void)//传回TH地址
{
    uint8_t Array[6] = {0};     //定义一个5位数据的数组 存放字节
    uint32_t Temp = 0;          //定义32位数组 暂时存放 20位数据
    uint32_t Result[2] = {0};   //定义一个2位数据的数组 存放数据结果
    static float TH[2] = {0};   //静态变量，返回最终数值

    AHT21_WriteMeasure();//发送测量命令
    Delay_Ms(500);

    AIIC_Start();//开始

    AIIC_SendByte(AHT20_ADDRESS_W | 0x01);   //从机地址＋读
    AIIC_WaitAck();

    Array[0]=AIIC_ReadByte();                    //状态字
    AIIC_ACK();//发送应答

    Array[1]=AIIC_ReadByte();         //湿度
    AIIC_ACK();//发送应答
    Array[2]=AIIC_ReadByte();         //湿度
    AIIC_ACK();//发送应答
    Array[3]=AIIC_ReadByte();         //湿度和温度
    AIIC_ACK();//发送应答
    Array[4]=AIIC_ReadByte();         //温度
    AIIC_ACK();//发送应答
    Array[5]=AIIC_ReadByte();         //温度
    AIIC_NACK();//不发送应答，结束

    AIIC_Stop();
    
    if(((Array[0]&0x80) == 0x00)) //如果测量完成 bit[7]=0,进行数据处理
    {
        Temp = (Temp | Array[1]) << 8;  //执行后0000 0000  0000 0000  aaaa aaaa  0000 0000
        Temp = (Temp | Array[2]) << 8;  //执行后0000 0000  aaaa aaaa  bbbb bbbb  0000 0000
        Temp = (Temp | Array[3]);   //执行后0000 0000  aaaa aaaa  bbbb bbbb  cccc cccc
        Temp =Temp >> 4;//数据右移  //执行后0000 0000  0000 aaaa  aaaa bbbb  bbbb cccc
        Result[0] = Temp;//湿度

        Temp = 0;
        Temp = (Temp | Array[3]) << 8;  //执行后0000 0000  0000 0000  cccc cccc  0000 0000
        Temp = (Temp | Array[4]) << 8;  //执行后0000 0000  cccc cccc  eeee eeee  0000 0000
        Temp = (Temp | Array[5]);       //执行后0000 0000  cccc cccc  eeee eeee  ffff ffff
        Temp = Temp & 0x000fffff;//置0   //执行后0000 0000  0000 cccc  eeee eeee  ffff ffff
        Result[1] = Temp;//温度

//      Value = 2000000.0/AHT21_Pow(2,20);
        TH[0] = (float)Result[0]/AHT21_Pow(2,20);
        TH[1] = (float)Result[1]/AHT21_Pow(2,20)*200-50;
        return TH;
    }
    else
    {
        printf("读失败/r/n");
        TH[0] = 0;
        TH[1] = 0;
        return TH;
    }
}
/*******************************************************************************
 * 参数：无
 * 函数功能：AHT21初始化
*******************************************************************************/
void AHT21_Init(void)
{
    uint8_t State = 0;
    AIIC_Init();
    //1 上电读取状态

    State = AHT21_ReadState();//读取状态

    //2 判断状态位 bit[3] 0x18未初始化寄存器/0x1c已初始化寄存器
    if((State&0x08) != 0x08)//不等于0000 0100，进行初始化寄存器
    {
        AHT21_WriteInit();//如果第三位是0，则初始化寄存器
        Delay_Ms(10);
    }
}

void AIIC_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( AHT20_GPIO_CLK, ENABLE );
    GPIO_InitStructure.GPIO_Pin = AHT20_SDA_PIN|AHT20_SCL_PIN;//使用PC14和PC15作为模拟IIC引脚，PC14对应SDA，PC15对应SCL
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //开漏输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(AHT20_GPIO_PORT, &GPIO_InitStructure);
    AIIC_Idle_State();

}
//IIC空闲状态

//当IIC总线的SDA和SCL两条信号线同时处于高电平时，规定为IIC总线的空闲状态
void AIIC_Idle_State()
{
    AIIC_SDA_H;
    AIIC_SCL_H;
    Delay_Ms(1);
}
//IIC开始信号

//当IIC SCL线处于高电平时，SDA线由高电平向低电平跳变，为IIC开始信号，配置开始信号前必须保证IIC总线处于空闲状态

void AIIC_Start()
{
    AIIC_SDA_H;
    AIIC_SCL_H;
    Delay_Ms(1);
    AIIC_SDA_L;
    Delay_Ms(1);
    AIIC_SCL_L;
    Delay_Ms(1);
}
//IIC停止信号

//当IIC SCL线处于高电平时，SDA线由低电平向高电平跳变，为IIC停止信号

void AIIC_Stop()
{
    AIIC_SDA_L;
    AIIC_SCL_H;
    Delay_Ms(1);
    AIIC_SDA_H;
}
void AIIC_SendByte(u8 data)
{
    u8 i;
    //先发送字节的高位bit7
    for (i = 0; i < 8; i++)
    {
        if (data & 0x80)  //判断8位数据每一位的值（0或1）
        {
            AIIC_SDA_H;
        }
        else
        {
            AIIC_SDA_L;

        }
        Delay_Ms(1);      //控制SCL线产生高低电平跳变，产生通讯时钟，同时利用延时函数在SCL为高电平期间读取SDA线电平逻辑
        AIIC_SCL_H;
        Delay_Ms(1);
        AIIC_SCL_L;
        if (i == 7)
        {
            AIIC_SDA_H;    //控制SDA线输出高电平，释放总线，等待接收方应答信号
        }
        data <<= 1;       //左移一个bit
        Delay_Ms(1);
    }
}
//IIC读取一个字节
u8 AIIC_ReadByte(void)
{
    u8 i;
    u8 value;
    //读到第1个bit为数据的bit7
    value = 0;
    for(i = 0; i < 8; i++)
    {
        value <<= 1;
        AIIC_SCL_H;
        Delay_Ms(1);
        if (AIIC_SDA_READ()) //利用延时函数在SCL为高电平期间读取SDA线电平逻辑
        {
            value++;
        }
        AIIC_SCL_L;
        Delay_Ms(1);
    }
    return value;

}
//IIC等待应答信号

u8 AIIC_WaitAck(void)
{
    uint8_t rvalue;
    AIIC_SDA_H;     //发送端释放SDA总线，由接收端控制SDA线
    Delay_Ms(1);
    AIIC_SCL_H;     //在SCL为高电平期间等待响应，若SDA线为高电平，表示NACK信号，反之则为ACK信号
    Delay_Ms(1);
    if(AIIC_SDA_READ())  //读取SDA线状态判断响应类型，高电平，返回去，为NACK信号，反之则为ACK信号
    {
        rvalue = 1;
    }
    else
    {
        rvalue = 0;
    }
    AIIC_SCL_L;
    Delay_Ms(1);
    return rvalue;
}
//产生应答信号ACK

void AIIC_ACK(void)
{
    AIIC_SDA_L;
    Delay_Ms(1);
    AIIC_SCL_H;   //在SCL线为高电平期间读取SDA线为低电平，则为ACK响应
    Delay_Ms(1);
    AIIC_SCL_L;
    Delay_Ms(1);
    AIIC_SDA_H;
}
//产生非应答信号NACK

void AIIC_NACK(void)
{
    AIIC_SDA_H;
    Delay_Ms(1);
    AIIC_SCL_H;   //在SCL线为高电平期间读取SDA线为高电平，则为NACK响应
    Delay_Ms(1);
    AIIC_SCL_L;
    Delay_Ms(1);

}

