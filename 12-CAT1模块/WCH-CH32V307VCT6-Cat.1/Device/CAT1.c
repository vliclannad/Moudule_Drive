#include "CAT1.h"
#include "string.h"
#include "stdio.h"
#include "user_usart.h"


void CAT_USART_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

//CAT����ģʽ����
#define CAT_CIP_MODE "AT+MIPMODE=0," //ģʽ���ã�0��ͨģʽ��1͸��ģʽ

//CATͨ��ָ��
#define CAT_START "AT+MIPOPEN=0,"         //��������
#define CAT_CLOSE "AT+MIPCLOSE\r\n"      //�ر�����

//CAT ͨ��ָ�������ȫ�ֱ���
static uint8_t  CAT_CMD[128];   //��������ݽ��ջ�����
static uint8_t  CAT_CMD_FLAG;


static uint8_t  CAT_RECVSTOP;    //����ģʽ��־

static uint8_t MODE_TYPE;
static uint8_t CAT_CONNECT_STATE;  //����������״̬
static uint8_t CAT_AP_STATE;       //CAT����״̬
static uint8_t CAT_SEND_STATE;     //CAT����ģʽ
static uint8_t CAT_BOOTFLAG;

uint8_t CAT_SendCMD(uint8_t* cmdstring);
uint8_t CAT_CheckIP(uint8_t* ip);
uint8_t CAT_CheckPort(uint8_t* port);
void CAT_CMD_ReturnCheck();
void CAT_ENABLE();
//======================================================================
//�������ƣ�CAT_Init
//���ܸ�Ҫ����ʼ��CATģ��
//����˵����mode_type:����ģ����Ϊ�ͻ��˻���AP�����;
//         mode_name:����ģ��APģʽ�µ�SSID;
//         pssword:Ҫ���õ������ַ�����������64λ����ASCII�ַ�;
//         chn:CAT�ŵ��ţ�1~13��ֻ��1��6��11�����ŵ���������;
//         ip:����Wi-Fiģ��STAģʽ�µ�IP��ַ,����Ϊ0.0.0.0��ʾ����DHCP
//�������أ���
//======================================================================
void CAT_Init(uint8_t mode_type)
{
    uint8_t test = 0xf1;
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    //��ʼ��ȫ�ֱ���
    CAT_RECVSTOP = 0;                     //����ģʽ��־
    CAT_CONNECT_STATE=0;                  //TCP����״̬
    CAT_AP_STATE=0;                       //Wi-Fi����״̬
    CAT_SEND_STATE=0;                     //͸��ģʽ�·���ģʽ״̬��־
    CAT_BOOTFLAG = 0;                     //������ɱ�־
    CAT_CMD_FLAG = 0;                     //����״̬��־

    RCC_APB1PeriphClockCmd(CAT_USART_CLK , ENABLE);
    RCC_APB2PeriphClockCmd(CAT_USART_GPIO_CLK , ENABLE);
    RCC_APB2PeriphClockCmd(CAT_Reboot_GPIO_CLK, ENABLE); //ʹ��CATʹ��GPIO����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //ʹ�ܸ��ù���ʱ��

    GPIO_PinRemapConfig(GPIO_FullRemap_USART5, ENABLE);
    //GPIO��ʼ��
    GPIO_InitStructure.GPIO_Pin = CAT_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(CAT_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = CAT_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(CAT_USART_RX_GPIO_PORT, &GPIO_InitStructure);
    //ʹ��CATģ��
    GPIO_InitStructure.GPIO_Pin = CAT_Reboot_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CAT_Reboot_GPIO_PORT, &GPIO_InitStructure);

    GPIO_SetBits(CAT_Reboot_GPIO_PORT, CAT_Reboot_GPIO_PIN); //CATʹ����������

    //���ڳ�ʼ��
    USART_InitStructure.USART_BaudRate = CAT_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(CAT_USARTx, &USART_InitStructure);
    USART_ITConfig(CAT_USARTx, USART_IT_RXNE, ENABLE);
    //���ý����ж�
    NVIC_InitStructure.NVIC_IRQChannel = CAT_USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(CAT_USARTx,USART_IT_RXNE,ENABLE);
    USART_Cmd(CAT_USARTx,ENABLE);

    //Delay_Ms(2000);
    //CAT_SendCMD((uint8_t*)"ATE0\r\n");//�رջ���
    //printf("���Թرճɹ�\r\n");

    //����ͨ��ģʽ
    switch(mode_type)
    {
        case CAT_STATION:
        CAT_ENABLE();
        printf("CATģ���ϵ�ɹ�\r\n");
        MODE_TYPE = 0;                          //��ǵ�ǰģʽΪSTAģʽ    
        break;
        default:
        break;
    }
}

uint8_t CAT_SetTxMode(uint8_t modetype)
{
    uint8_t cmd[14];
    if(modetype<0||modetype>1)
    {
        return CAT_ERROR;
    }
    sprintf((char*)cmd,"%s%d\r\n",CAT_CIP_MODE,modetype);
    if(modetype ==1)
    {
        CAT_SendCMD(cmd);
        memset(cmd,0,sizeof(cmd));
        sprintf((char*)cmd,"%s\r\n","AT+MIPMODE=0,1");
        return(CAT_SendCMD(cmd));
    }
    else
    {
        return(CAT_SendCMD(cmd));
    }

}

//======================================================================
//�������ƣ�CAT_send_cmd
//���ܸ�Ҫ������CATģ��������жϿ�����������ܹ�����CATģ��Ӧ������
//����˵����cmdstring:�����ַ���
//�������أ����ͺ�CATģ��Ӧ��״̬��=0�ɹ���=1����=2��Ӧ��
//======================================================================
uint8_t CAT_SendCMD(uint8_t* cmdstring)
{
    //����ֲ�����
    uint32_t i;
    uint8_t retryCount = 0;
    //1.��ʼ��ȫ�ֱ���
    CAT_CMD_FLAG = 0;
    CAT_RECVSTOP = CAT_RECV_CMD;//�л�����ģʽΪ���������
    //2.�򴮿ڷ���ָ��
    Delay_Ms(3000);
    while(retryCount>=0&&retryCount<4)
    {
    USER_UART_SendString(CAT_USARTx,cmdstring);   
    i=0;
    Delay_Ms(3000);
    //3.�����ɹ�״̬��ʧ��״̬
    while(1)
    {

        //3.1 �������ɹ�״̬
        if(CAT_CMD_FLAG == 1)
        {
            CAT_RECVSTOP = CAT_RECV_DATA;    //�����жϽ���ģʽΪ���ݽ���ģʽ
            return CAT_OK;                    //��������ɹ���־CAT_OK��0��
        }
        //3.2 ������ʧ��״̬
        else if(CAT_CMD_FLAG == 2)
        {
           
            retryCount++;
            break;
            //return CAT_ERROR;                 //��������ʧ�ܱ�־CAT_ERROR��1��
        }
        i++;                                   //�ƴα�־��һ
        //3.3 ������ʱ
        if(i>0x1ffffff)
        {
            CAT_RECVSTOP = CAT_RECV_DATA;    //�����жϽ���ģʽΪ���ݽ���ģʽ
            return CAT_NORESPONSE;            //�������ʱ��־CAT_NORESPONSE��2��
        }
    }
    }
    CAT_RECVSTOP = CAT_RECV_DATA;    //�����жϽ���ģʽΪ���ݽ���ģʽ
    return CAT_ERROR;                         //��������ʧ�ܱ�־CAT_ERROR��1��
}
//�����������TCP����
uint8_t CAT_TCPConnect(uint8_t* server_ip,uint8_t* server_port)
{
    //����ֲ�����
    uint8_t flag,cmd[100];
    //1.���CATģ�鵱ǰ״̬�Ƿ��ڷ���ģʽ
    if(CAT_SEND_STATE==1)
        return 2;
    //2.��������ip��ַ�Ƿ�Ϸ�
    if(!CAT_CheckIP(server_ip))
        return CAT_ERROR;
    //3.��������portֵ�Ƿ�Ϸ�
    if(!CAT_CheckPort(server_port))
        return CAT_ERROR;
    //4.ƴ��ָ��
    sprintf((char*)cmd,"%s\"%s\",\"%s\",%s,%d,%d\r\n",CAT_START,"TCP",(char*)server_ip,(char*)server_port,60,0);
    //5.����ָ��
    printf("%s\r\n",cmd);
    flag=CAT_SendCMD(cmd);
    if(flag == CAT_OK)
    {
        printf("TCP���ӽ����ɹ�");
        CAT_CONNECT_STATE = 1;
    }
    return flag;
}
//�Ͽ�TCP��UDP����
uint8_t CAT_Disconnnect()
{
    return CAT_SendCMD((uint8_t*)CAT_CLOSE);
}

//===========================�ڲ�����====================================
//======================================================================
//�������ƣ�CAT_CheckIP
//���ܸ�Ҫ�����ip��ַ�Ƿ�Ϸ�
//����˵����ip:��Ҫ����ip��ַ�ַ���,��ʽ"xxx.xxx.xxx.xxx",xxxȡֵ��Χ0-255
//�������أ�=1�Ϸ���=0���Ϸ�
//======================================================================
uint8_t CAT_CheckIP(uint8_t* ip)
{
    //����ֲ�����
    uint8_t i,j,maxlength;           //i��������ӿ�,j������¼�ӿ鳤��
    uint16_t length,num[4];
    //��ʼ���ֲ�����
    for(i=0;i<4;i++)
        num[i]=0;
    maxlength=15;
    //1.��鳤�Ȳ�������ip�ַ����зֳ��Ŀ�
    for(length=0,i=0,j=0;*ip!='\0'&&length<=maxlength;length++)
    {
        if(*ip=='.')
        {
            if(j==0||j>3)
                return 0;
            i++;                     //�ӿ��ʶ��һ
            j=0;                     //�ӿ鳤�����㿪ʼ������һ���ӿ�
        }
        else if(*ip>='0'&&*ip<='9')
        {
            num[i]=num[i]*10+(*ip-'0');
            j++;                     //��ǰ�ӿ鳤��+1
        }
        else return 0;               //��⵽�����ַ���0
        ip++;                        //ת���¸��ַ�
    }
    if(length>maxlength)             //����,����0
        return 0;
    //2.����ӿ��Ƿ�С�ڵ���255
    for(i=0;i<4;i++)
    {
        if(!(num[i]<256))            //��⵽����255���ӿ�,����0
            return 0;
    }
    return 1;                        //��������������,����1
}
uint8_t CAT_CheckPort(uint8_t* port)
{
    //����ֲ�����
    uint8_t length,maxlength;
    uint32_t num;
    maxlength=5;    //�˿��ַ��������5��
    for(length=0,num=0;*port!='\0'&&length<maxlength;length++)
    {
        //������������µ�numֵ�����򷵻�0
        if(*port>='0'&&*port<='9')
            num=num*10+(*port-'0');
        else return 0;
        port++;
    }
    //��������0
    if(length>maxlength)
        return 0;
    //�˿�ֵ������Χ
    if(num>65535)
        return 0;
    return 1;
}

void CAT_USART_IRQHandler()
{
    if(USART_GetITStatus(CAT_USARTx, USART_IT_RXNE) != RESET)
    {
        
        static uint16_t index = 0;
        uint8_t  data = USART_ReceiveData(CAT_USARTx);
        switch (CAT_RECVSTOP)
        {
        //���ݽ���ģʽ
        case 0:
            printf("%s",data);
            break;
        //����ؽ���ģʽ
        case 1:
            CAT_CMD[index++] = data;
            if(data == '\n')
            {
                index = 0;
                CAT_CMD_ReturnCheck();
            }
            break;
        default:
            break;
        }
    }
}

void CAT_CMD_ReturnCheck()
{
    if(strstr(CAT_CMD,"+MIPOPEN: 0,0"))
    {
        printf("TCP���ӳɹ�\r\n");
        CAT_CMD_FLAG = 1;
    }
    else if(strstr(CAT_CMD,"OK"))
    {
        printf("Command executed successfully\r\n");
        CAT_CMD_FLAG = 1;
    }
    else if(strstr(CAT_CMD,"CONNECT"))
    {
        printf("�ɹ�����͸��ģʽ\r\n");
        CAT_CMD_FLAG = 1;
    }
    else if(strstr(CAT_CMD,"ERROR"))
    {
        printf("Command executed failed\r\n");
        CAT_CMD_FLAG = 2;
    }
    else
    {
        printf("%s\r\n",CAT_CMD);
    }
    memset(CAT_CMD,0,sizeof(CAT_CMD));

}
void CAT_ENABLE()
{
    GPIO_SetBits(CAT_Reboot_GPIO_PORT, CAT_Reboot_GPIO_PIN);
}

