#include "AHT20.h"
//��ʪ�ȴ�����
//*************************���º���Ϊ�ڲ�����******************************
/*******************************************************************************
 * �������������0������1
 * �������ܣ�ͨ�� AHT21д�� [����+���ֽ�]
*******************************************************************************/
void AIIC_Init(void);         //IIC��ʼ������
void AIIC_Idle_State(void);   //IIC����״̬
void AIIC_Start(void);        //IIC��ʼ�źź���
void AIIC_Stop(void);         //IICֹͣ�źź���
void AIIC_SendByte(u8 data);  //IIC����һ���ֽ�
u8   AIIC_ReadByte(void);     //IIC��ȡһ���ֽ�
u8   AIIC_WaitAck(void);      //�ȴ���Ӧ�źţ�ACK����NACK��
void AIIC_ACK(void);          //IIC����ACK�ź�
void AIIC_NACK(void);         //IIC����NACK�ź�


void AHT21_WriteHalfWord(uint8_t Command,uint8_t Data0,uint8_t Data1)
{
    AIIC_Start();//��ʼ

    AIIC_SendByte(AHT20_ADDRESS_W);//�ӻ���ַ��д
    AIIC_WaitAck();//Ӧ��

    AIIC_SendByte(Command);//����
    AIIC_WaitAck();//Ӧ��

    AIIC_SendByte(Data0);//д��Data0
    AIIC_WaitAck();//Ӧ��

    AIIC_SendByte(Data1);//д��Data1
    AIIC_WaitAck();//Ӧ��

    AIIC_Stop();//ֹͣ
}

/*******************************************************************************
 * ����������
 * �������ܣ�ͨ�� AHT21д�� [����]
*******************************************************************************/
void AHT21_WriteCommand(uint8_t Command)
{
    AIIC_Start();     //��ʼ

    AIIC_SendByte(AHT20_ADDRESS_W);//�ӻ���ַ��д
    AIIC_WaitAck();//Ӧ��

    AIIC_SendByte(Command);//����
    AIIC_WaitAck();//Ӧ��

    AIIC_Stop();//ֹͣ
}
/*******************************************************************************
 * ��������
 * �������ܣ���ȡ AHT21 ״̬
*******************************************************************************/
uint8_t AHT21_ReadState(void) // ��ȡ AHT21 ״̬
{
    uint8_t State;

    AIIC_Start();//��ʼ

    AIIC_SendByte(AHT20_ADDRESS_W | 0x01);//�ӻ���ַ����
    AIIC_WaitAck();//����Ӧ��

    State=AIIC_ReadByte();//����״̬
    AIIC_NACK();//���ͷ�Ӧ��

    AIIC_Stop();//ֹͣ
    return State;
}
/*******************************************************************************
 * ��������
 * �������ܣ�AHT21 д��[��ʼ�� �Ĵ���ָ��]
*******************************************************************************/
void AHT21_WriteInit(void) //AHT21 д��[��ʼ�� �Ĵ���ָ��]
{
    AHT21_WriteHalfWord(0xBE,0x08,0x00);
}
/*******************************************************************************
 * ��������
 * �������ܣ�д��[��ʪ�ȴ�������ָ��]��AHT21
*******************************************************************************/
void AHT21_WriteMeasure(void) //д�봥������ָ��
{
    AHT21_WriteHalfWord(0xAC,0x33,0x00);
}
/*******************************************************************************
 * ��������
 * �������ܣ���λ
*******************************************************************************/
void AHT21_WriteReset(void)//��λAHT21
{
    AHT21_WriteCommand(0xBA);

}
uint32_t AHT21_Pow(uint32_t Num,uint32_t Power)//�ݺ�����num��power��
{
    uint32_t Result = 1;
    while(Power--)
    {
        Result *= Num;
    }
    return Result;
}
/*******************************************************************************
 * ��������
 * �������ܣ����Ͳ��������ȡ�¶ȡ�ʪ��
 * ����ֵ��TH����
*******************************************************************************/
float* AHT21_ReadTemperatureHumidity(void)//����TH��ַ
{
    uint8_t Array[6] = {0};     //����һ��5λ���ݵ����� ����ֽ�
    uint32_t Temp = 0;          //����32λ���� ��ʱ��� 20λ����
    uint32_t Result[2] = {0};   //����һ��2λ���ݵ����� ������ݽ��
    static float TH[2] = {0};   //��̬����������������ֵ

    AHT21_WriteMeasure();//���Ͳ�������
    Delay_Ms(500);

    AIIC_Start();//��ʼ

    AIIC_SendByte(AHT20_ADDRESS_W | 0x01);   //�ӻ���ַ����
    AIIC_WaitAck();

    Array[0]=AIIC_ReadByte();                    //״̬��
    AIIC_ACK();//����Ӧ��

    Array[1]=AIIC_ReadByte();         //ʪ��
    AIIC_ACK();//����Ӧ��
    Array[2]=AIIC_ReadByte();         //ʪ��
    AIIC_ACK();//����Ӧ��
    Array[3]=AIIC_ReadByte();         //ʪ�Ⱥ��¶�
    AIIC_ACK();//����Ӧ��
    Array[4]=AIIC_ReadByte();         //�¶�
    AIIC_ACK();//����Ӧ��
    Array[5]=AIIC_ReadByte();         //�¶�
    AIIC_NACK();//������Ӧ�𣬽���

    AIIC_Stop();
    
    if(((Array[0]&0x80) == 0x00)) //���������� bit[7]=0,�������ݴ���
    {
        Temp = (Temp | Array[1]) << 8;  //ִ�к�0000 0000  0000 0000  aaaa aaaa  0000 0000
        Temp = (Temp | Array[2]) << 8;  //ִ�к�0000 0000  aaaa aaaa  bbbb bbbb  0000 0000
        Temp = (Temp | Array[3]);   //ִ�к�0000 0000  aaaa aaaa  bbbb bbbb  cccc cccc
        Temp =Temp >> 4;//��������  //ִ�к�0000 0000  0000 aaaa  aaaa bbbb  bbbb cccc
        Result[0] = Temp;//ʪ��

        Temp = 0;
        Temp = (Temp | Array[3]) << 8;  //ִ�к�0000 0000  0000 0000  cccc cccc  0000 0000
        Temp = (Temp | Array[4]) << 8;  //ִ�к�0000 0000  cccc cccc  eeee eeee  0000 0000
        Temp = (Temp | Array[5]);       //ִ�к�0000 0000  cccc cccc  eeee eeee  ffff ffff
        Temp = Temp & 0x000fffff;//��0   //ִ�к�0000 0000  0000 cccc  eeee eeee  ffff ffff
        Result[1] = Temp;//�¶�

//      Value = 2000000.0/AHT21_Pow(2,20);
        TH[0] = (float)Result[0]/AHT21_Pow(2,20);
        TH[1] = (float)Result[1]/AHT21_Pow(2,20)*200-50;
        return TH;
    }
    else
    {
        printf("��ʧ��/r/n");
        TH[0] = 0;
        TH[1] = 0;
        return TH;
    }
}
/*******************************************************************************
 * ��������
 * �������ܣ�AHT21��ʼ��
*******************************************************************************/
void AHT21_Init(void)
{
    uint8_t State = 0;
    AIIC_Init();
    //1 �ϵ��ȡ״̬

    State = AHT21_ReadState();//��ȡ״̬

    //2 �ж�״̬λ bit[3] 0x18δ��ʼ���Ĵ���/0x1c�ѳ�ʼ���Ĵ���
    if((State&0x08) != 0x08)//������0000 0100�����г�ʼ���Ĵ���
    {
        AHT21_WriteInit();//�������λ��0�����ʼ���Ĵ���
        Delay_Ms(10);
    }
}

void AIIC_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( AHT20_GPIO_CLK, ENABLE );
    GPIO_InitStructure.GPIO_Pin = AHT20_SDA_PIN|AHT20_SCL_PIN;//ʹ��PC14��PC15��Ϊģ��IIC���ţ�PC14��ӦSDA��PC15��ӦSCL
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //��©���ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(AHT20_GPIO_PORT, &GPIO_InitStructure);
    AIIC_Idle_State();

}
//IIC����״̬

//��IIC���ߵ�SDA��SCL�����ź���ͬʱ���ڸߵ�ƽʱ���涨ΪIIC���ߵĿ���״̬
void AIIC_Idle_State()
{
    AIIC_SDA_H;
    AIIC_SCL_H;
    Delay_Ms(1);
}
//IIC��ʼ�ź�

//��IIC SCL�ߴ��ڸߵ�ƽʱ��SDA���ɸߵ�ƽ��͵�ƽ���䣬ΪIIC��ʼ�źţ����ÿ�ʼ�ź�ǰ���뱣֤IIC���ߴ��ڿ���״̬

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
//IICֹͣ�ź�

//��IIC SCL�ߴ��ڸߵ�ƽʱ��SDA���ɵ͵�ƽ��ߵ�ƽ���䣬ΪIICֹͣ�ź�

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
    //�ȷ����ֽڵĸ�λbit7
    for (i = 0; i < 8; i++)
    {
        if (data & 0x80)  //�ж�8λ����ÿһλ��ֵ��0��1��
        {
            AIIC_SDA_H;
        }
        else
        {
            AIIC_SDA_L;

        }
        Delay_Ms(1);      //����SCL�߲����ߵ͵�ƽ���䣬����ͨѶʱ�ӣ�ͬʱ������ʱ������SCLΪ�ߵ�ƽ�ڼ��ȡSDA�ߵ�ƽ�߼�
        AIIC_SCL_H;
        Delay_Ms(1);
        AIIC_SCL_L;
        if (i == 7)
        {
            AIIC_SDA_H;    //����SDA������ߵ�ƽ���ͷ����ߣ��ȴ����շ�Ӧ���ź�
        }
        data <<= 1;       //����һ��bit
        Delay_Ms(1);
    }
}
//IIC��ȡһ���ֽ�
u8 AIIC_ReadByte(void)
{
    u8 i;
    u8 value;
    //������1��bitΪ���ݵ�bit7
    value = 0;
    for(i = 0; i < 8; i++)
    {
        value <<= 1;
        AIIC_SCL_H;
        Delay_Ms(1);
        if (AIIC_SDA_READ()) //������ʱ������SCLΪ�ߵ�ƽ�ڼ��ȡSDA�ߵ�ƽ�߼�
        {
            value++;
        }
        AIIC_SCL_L;
        Delay_Ms(1);
    }
    return value;

}
//IIC�ȴ�Ӧ���ź�

u8 AIIC_WaitAck(void)
{
    uint8_t rvalue;
    AIIC_SDA_H;     //���Ͷ��ͷ�SDA���ߣ��ɽ��ն˿���SDA��
    Delay_Ms(1);
    AIIC_SCL_H;     //��SCLΪ�ߵ�ƽ�ڼ�ȴ���Ӧ����SDA��Ϊ�ߵ�ƽ����ʾNACK�źţ���֮��ΪACK�ź�
    Delay_Ms(1);
    if(AIIC_SDA_READ())  //��ȡSDA��״̬�ж���Ӧ���ͣ��ߵ�ƽ������ȥ��ΪNACK�źţ���֮��ΪACK�ź�
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
//����Ӧ���ź�ACK

void AIIC_ACK(void)
{
    AIIC_SDA_L;
    Delay_Ms(1);
    AIIC_SCL_H;   //��SCL��Ϊ�ߵ�ƽ�ڼ��ȡSDA��Ϊ�͵�ƽ����ΪACK��Ӧ
    Delay_Ms(1);
    AIIC_SCL_L;
    Delay_Ms(1);
    AIIC_SDA_H;
}
//������Ӧ���ź�NACK

void AIIC_NACK(void)
{
    AIIC_SDA_H;
    Delay_Ms(1);
    AIIC_SCL_H;   //��SCL��Ϊ�ߵ�ƽ�ڼ��ȡSDA��Ϊ�ߵ�ƽ����ΪNACK��Ӧ
    Delay_Ms(1);
    AIIC_SCL_L;
    Delay_Ms(1);

}

