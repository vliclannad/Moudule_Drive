#include "ESP8266.h"
#include "string.h"
#include "stdio.h"
#include "user_usart.h"


void WIFI_USART_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

//wifi����ָ��
#define WIFI_MODE_SET "AT+CWMODE="      //����WIFIģʽ��1:Stationģʽ  2:SoftAPģʽ  3:SoftAP+Stationģʽ
#define WIFI_LINK_AP  "AT+CWJAP="         //����AP
#define WIFI_QUIT_AP  "AT+WJAPQ\r"      //�Ͽ�AP
#define WIFI_DHCP     "AT+CWDHCP=1,1\r\n"     //����DHCP
#define WIFI_AUTO_LINK"AT+CWAUTOCONN=1\r\n" // ����WIFI�Զ�����

//wifi����ģʽ����
#define WIFI_CIP_MODE "AT+CIPMODE=" //ģʽ���ã�0��ͨģʽ��1͸��ģʽ

//wifiͨ��ָ��
#define WIFI_STATUS "AT+CIPSTATUS\r\n"    //��ѯ����״̬
#define WIFI_START "AT+CIPSTART="         //��������
#define WIFI_CLOSE "AT+CIPCLOSE\r\n"      //�ر�����
#define WIFI_SEND "AT+CIPSEND\r\n"        //����ָ��
#define WIFI_PING "AT+PING="              //pingָ��ip
#define WIFI_DOMAIN_RESOLVE "AT+CIPDOMAIN="  //��������

//ͨ�Ų������úͲ�ѯָ��
#define WIFI_STA_MAC_Q "AT+CIPSTAMAC?\r\n"//��ѯstationģʽ�µ�mac��ַ
#define WIFI_STA_MAC_S "AT+CIPSTAMAC="    //����stationģʽ�µ�mac��ַ
#define WIFI_STA_IP    "AT+CIPSTA="       //����stationģʽ�µ�IP��ַ
#define WIFI_SET_AP    "AT+CWSAP="        //����APģʽ�Ĳ���

//WIFI ͨ��ָ�������ȫ�ֱ���
static uint8_t  WIFI_CMD[128];   //��������ݽ��ջ�����
static uint8_t  WIFI_CMD_FLAG;


static uint8_t  WIFI_RECVSTOP;    //����ģʽ��־

static uint8_t  WIFI_ERR_STOP[16]; //ʧ��֡β
static uint8_t  WIFI_OK_STOP[21]; //�ɹ�֡β

static uint8_t MODE_TYPE;
static uint8_t WIFI_CONNECT_STATE;  //����������״̬
static uint8_t WIFI_AP_STATE;       //WIFI����״̬
static uint8_t WIFI_SEND_STATE;     //WIFI����ģʽ
static uint8_t WIFI_BOOTFLAG;


uint8_t WIFI_SetMode(uint8_t modetype);
uint8_t WIFI_SendCMD(uint8_t* cmdstring);
uint8_t WIFI_SetIP(uint8_t* ip);
uint8_t WIFI_CheckIP(uint8_t* ip);
uint8_t WIFI_CheckPort(uint8_t* port);
uint8_t WIFI_CMD_ReturnCheck();
//======================================================================
//�������ƣ�WIFI_Init
//���ܸ�Ҫ����ʼ��wifiģ��
//����˵����mode_type:����ģ����Ϊ�ͻ��˻���AP�����;
//         mode_name:����ģ��APģʽ�µ�SSID;
//         pssword:Ҫ���õ������ַ�����������64λ����ASCII�ַ�;
//         chn:wifi�ŵ��ţ�1~13��ֻ��1��6��11�����ŵ���������;
//         ip:����Wi-Fiģ��STAģʽ�µ�IP��ַ,����Ϊ0.0.0.0��ʾ����DHCP
//�������أ���
//======================================================================
void WIFI_Init(uint8_t mode_type)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    //��ʼ��ȫ�ֱ���
    WIFI_RECVSTOP = 0;                     //����ģʽ��־
    WIFI_CONNECT_STATE=0;                  //TCP����״̬
    WIFI_AP_STATE=0;                       //Wi-Fi����״̬
    WIFI_SEND_STATE=0;                     //͸��ģʽ�·���ģʽ״̬��־
    WIFI_BOOTFLAG = 0;                     //������ɱ�־
    WIFI_CMD_FLAG = 0;                     //����״̬��־

    RCC_APB1PeriphClockCmd(WIFI_USART_CLK , ENABLE);
    RCC_APB2PeriphClockCmd(WIFI_USART_GPIO_CLK , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //����AFIOʱ��

    GPIO_PinRemapConfig(GPIO_FullRemap_USART6, ENABLE); //����GPIO���ù���
    //GPIO��ʼ��
    GPIO_InitStructure.GPIO_Pin = WIFI_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(WIFI_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = WIFI_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(WIFI_USART_RX_GPIO_PORT, &GPIO_InitStructure);
    //���ڳ�ʼ��
    USART_InitStructure.USART_BaudRate = WIFI_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(WIFI_USARTx, &USART_InitStructure);
    USART_ITConfig(WIFI_USARTx, USART_IT_RXNE, ENABLE);
    //���ý����ж�
    NVIC_InitStructure.NVIC_IRQChannel = WIFI_USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(WIFI_USARTx,USART_IT_RXNE,ENABLE);
    USART_Cmd(WIFI_USARTx,ENABLE);

     WIFI_SendCMD((uint8_t*)"ATE0\r\n");//�رջ���
     printf("���Թرճɹ�\r\n");

    //����ͨ��ģʽ
    switch(mode_type)
    {
        case WIFI_STATION:
        printf("��ʼ����WIFIģʽΪSTAģʽ");
        if(WIFI_SetMode(WIFI_STATION) == WIFI_OK)              //����STAģʽ
        printf("����WIFIģʽΪSTAģʽ�ɹ�\r\n");
        MODE_TYPE = 0;                          //��ǵ�ǰģʽΪSTAģʽ    
        if(WIFI_SendCMD((uint8_t*)WIFI_AUTO_LINK))   //����wifiģ���Զ�����
        printf("WIFIģ���Զ����ӳɹ�\r\n");
        break;
        //APģʽ
        //���ģʽ
        default:
        break;
    }
}

//======================================================================
//�������ƣ�wifi_mode_set
//���ܸ�Ҫ������WIFIӦ��ģʽ
//����˵����modetype:Ӧ��ģʽ,=1 Stationģʽ
//                      =2 APģʽ
//                      =3 ���ģʽ
//�������أ�ģʽ�����Ƿ�ɹ�=0�ɹ���=1����=2�޷�������
//======================================================================
uint8_t WIFI_SetMode(uint8_t modetype)
{
    uint8_t cmd[14];
    if(modetype<1||modetype>3)
        return WIFI_ERROR;
    sprintf((char*)cmd,"%s%d\r\n",WIFI_MODE_SET,modetype);
    return (WIFI_SendCMD(cmd));
}

uint8_t WIFI_SetTxMode(uint8_t modetype)
{
    uint8_t cmd[14];
    if(modetype<0||modetype>1)
    {
        return WIFI_ERROR;
    }
    sprintf((char*)cmd,"%s%d\r\n",WIFI_CIP_MODE,modetype);
    if(modetype ==1)
    {
        WIFI_SendCMD(cmd);
        memset(cmd,0,sizeof(cmd));
        sprintf((char*)cmd,"%s\r\n","AT+CIPSEND");
        return(WIFI_SendCMD(cmd));
    }
    else
    {
        return(WIFI_SendCMD(cmd));
    }



}

//======================================================================
//�������ƣ�wifi_send_cmd
//���ܸ�Ҫ������wifiģ��������жϿ�����������ܹ�����WiFiģ��Ӧ������
//����˵����cmdstring:�����ַ���
//�������أ����ͺ�wifiģ��Ӧ��״̬��=0�ɹ���=1����=2��Ӧ��
//======================================================================
uint8_t WIFI_SendCMD(uint8_t* cmdstring)
{
    //����ֲ�����
    uint32_t i;
    uint8_t retryCount = 0;
    //1.��ʼ��ȫ�ֱ���
    WIFI_CMD_FLAG = 0;
    WIFI_RECVSTOP = WIFI_RECV_CMD;//�л�����ģʽΪ���������
    //2.�򴮿ڷ���ָ��
    Delay_Ms(3000);
    while(retryCount>=0&&retryCount<4)
    {
    USER_UART_SendString(WIFI_USARTx,cmdstring);   
    i=0;
    Delay_Ms(3000);
    //3.�����ɹ�״̬��ʧ��״̬
    while(1)
    {

        //3.1 �������ɹ�״̬
        if(WIFI_CMD_FLAG == 1)
        {
            WIFI_RECVSTOP = WIFI_RECV_DATA;    //�����жϽ���ģʽΪ���ݽ���ģʽ
            return WIFI_OK;                    //��������ɹ���־WIFI_OK��0��
        }
        //3.2 ������ʧ��״̬
        else if(WIFI_CMD_FLAG == 2)
        {
           
            retryCount++;
            break;
            //return WIFI_ERROR;                 //��������ʧ�ܱ�־WIFI_ERROR��1��
        }
        i++;                                   //�ƴα�־��һ
        //3.3 ������ʱ
        if(i>0x1ffffff)
        {
            WIFI_RECVSTOP = WIFI_RECV_DATA;    //�����жϽ���ģʽΪ���ݽ���ģʽ
            return WIFI_NORESPONSE;            //�������ʱ��־WIFI_NORESPONSE��2��
        }
    }
    }
    WIFI_RECVSTOP = WIFI_RECV_DATA;    //�����жϽ���ģʽΪ���ݽ���ģʽ
    return WIFI_ERROR;                         //��������ʧ�ܱ�־WIFI_ERROR��1��
}

//======================================================================
//�������ƣ�WIFI_SetIP
//���ܸ�Ҫ������staģʽ��WiFiģ���IP��ַ
//����˵����ip:Ҫ���õ�IP��ַ
//�������أ�=0���óɹ���=1����ʧ�ܣ�=2WiFiģ����Ӧ��
//======================================================================
uint8_t WIFI_SetIP(uint8_t* ip)
{
    uint8_t cmd[30];
    if(!WIFI_CheckIP(ip))
    {
        return WIFI_ERROR;
    }
    sprintf((char*)cmd,"%s\"%s\"\r\n",WIFI_STA_IP,ip);
    return WIFI_SendCMD(cmd);
}
uint8_t WIFI_LinkSSID(uint8_t* ssid,uint8_t* password)
{
        //����ֲ�����
    uint8_t cmd[40],flag;

    sprintf((char*)cmd,"%s\"%s\"%s\"%s\"\r\n","AT+CWJAP=",(char*)ssid,",",(char*)password);
    flag=WIFI_SendCMD(cmd);
    if(flag == WIFI_OK)
    {
        WIFI_AP_STATE = 1;
    }
    return (flag);
}
//�����������TCP����
uint8_t WIFI_TCPConnect(uint8_t* server_ip,uint8_t* server_port)
{
    //����ֲ�����
    uint8_t flag,cmd[100];
    //1.���wifiģ�鵱ǰ״̬�Ƿ��ڷ���ģʽ
    if(WIFI_SEND_STATE==1)
        return 2;
    //2.��������ip��ַ�Ƿ�Ϸ�
    if(!WIFI_CheckIP(server_ip))
        return WIFI_ERROR;
    //3.��������portֵ�Ƿ�Ϸ�
    if(!WIFI_CheckPort(server_port))
        return WIFI_ERROR;
    //4.ƴ��ָ��
    sprintf((char*)cmd,"%s\"%s\",\"%s\",%s\r\n",WIFI_START,"TCP",(char*)server_ip,(char*)server_port);
    //5.����ָ��
    flag=WIFI_SendCMD(cmd);
    if(flag == WIFI_OK)
    {
        printf("TCP���ӽ����ɹ�");
        WIFI_CONNECT_STATE = 1;
    }
    return flag;
}
//�Ͽ�TCP��UDP����
uint8_t WIFI_Disconnnect()
{
    return WIFI_SendCMD((uint8_t*)WIFI_CLOSE);
}

//===========================�ڲ�����====================================
//======================================================================
//�������ƣ�WIFI_CheckIP
//���ܸ�Ҫ�����ip��ַ�Ƿ�Ϸ�
//����˵����ip:��Ҫ����ip��ַ�ַ���,��ʽ"xxx.xxx.xxx.xxx",xxxȡֵ��Χ0-255
//�������أ�=1�Ϸ���=0���Ϸ�
//======================================================================
uint8_t WIFI_CheckIP(uint8_t* ip)
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
uint8_t WIFI_CheckPort(uint8_t* port)
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

void WIFI_USART_IRQHandler()
{
    if(USART_GetITStatus(WIFI_USARTx, USART_IT_RXNE) != RESET)
    {
        
        static uint16_t index = 0;
        uint8_t  data = USART_ReceiveData(WIFI_USARTx);
        switch (WIFI_RECVSTOP)
        {
        //���ݽ���ģʽ
        case 0:
            printf("%d",data);
            break;
        //����ؽ���ģʽ
        case 1:
            WIFI_CMD[index++] = data;
            if(data == '\n')
            {
                index = 0;
                WIFI_CMD_ReturnCheck();
            }
            break;
        default:
            break;
        }

    }
}

uint8_t WIFI_CMD_ReturnCheck()
{
    if(strstr(WIFI_CMD,"OK"))
    {
        printf("Command executed successfully\r\n");
        WIFI_CMD_FLAG = 1;
    }
    else if(strstr(WIFI_CMD,"ERROR"))
    {
        printf("Command executed failed\r\n");
        WIFI_CMD_FLAG = 2;
    }
    else
    {
        printf("%s\r\n",WIFI_CMD);
    }
    memset(WIFI_CMD,0,sizeof(WIFI_CMD));

}






