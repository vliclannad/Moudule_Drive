#include "ESP8266.h"
#include "string.h"
#include "stdio.h"
#include "user_usart.h"


void WIFI_USART_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

//wifi连接指令
#define WIFI_MODE_SET "AT+CWMODE="      //设置WIFI模式，1:Station模式  2:SoftAP模式  3:SoftAP+Station模式
#define WIFI_LINK_AP  "AT+CWJAP="         //连接AP
#define WIFI_QUIT_AP  "AT+WJAPQ\r"      //断开AP
#define WIFI_DHCP     "AT+CWDHCP=1,1\r\n"     //开启DHCP
#define WIFI_AUTO_LINK"AT+CWAUTOCONN=1\r\n" // 开启WIFI自动连接

//wifi传输模式设置
#define WIFI_CIP_MODE "AT+CIPMODE=" //模式设置，0普通模式，1透传模式

//wifi通信指令
#define WIFI_STATUS "AT+CIPSTATUS\r\n"    //查询连接状态
#define WIFI_START "AT+CIPSTART="         //建立连接
#define WIFI_CLOSE "AT+CIPCLOSE\r\n"      //关闭连接
#define WIFI_SEND "AT+CIPSEND\r\n"        //发送指令
#define WIFI_PING "AT+PING="              //ping指定ip
#define WIFI_DOMAIN_RESOLVE "AT+CIPDOMAIN="  //域名解析

//通信参数设置和查询指令
#define WIFI_STA_MAC_Q "AT+CIPSTAMAC?\r\n"//查询station模式下的mac地址
#define WIFI_STA_MAC_S "AT+CIPSTAMAC="    //设置station模式下的mac地址
#define WIFI_STA_IP    "AT+CIPSTA="       //设置station模式下的IP地址
#define WIFI_SET_AP    "AT+CWSAP="        //设置AP模式的参数

//WIFI 通信指令接收用全局变量
static uint8_t  WIFI_CMD[128];   //命令返回数据接收缓冲区
static uint8_t  WIFI_CMD_FLAG;


static uint8_t  WIFI_RECVSTOP;    //接收模式标志

static uint8_t  WIFI_ERR_STOP[16]; //失败帧尾
static uint8_t  WIFI_OK_STOP[21]; //成功帧尾

static uint8_t MODE_TYPE;
static uint8_t WIFI_CONNECT_STATE;  //服务器连接状态
static uint8_t WIFI_AP_STATE;       //WIFI连接状态
static uint8_t WIFI_SEND_STATE;     //WIFI发送模式
static uint8_t WIFI_BOOTFLAG;


uint8_t WIFI_SetMode(uint8_t modetype);
uint8_t WIFI_SendCMD(uint8_t* cmdstring);
uint8_t WIFI_SetIP(uint8_t* ip);
uint8_t WIFI_CheckIP(uint8_t* ip);
uint8_t WIFI_CheckPort(uint8_t* port);
uint8_t WIFI_CMD_ReturnCheck();
//======================================================================
//函数名称：WIFI_Init
//功能概要：初始化wifi模块
//参数说明：mode_type:决定模块作为客户端还是AP接入点;
//         mode_name:设置模块AP模式下的SSID;
//         pssword:要设置的密码字符串，必须是64位以内ASCII字符;
//         chn:wifi信道号，1~13，只有1、6、11三个信道互不干扰;
//         ip:设置Wi-Fi模块STA模式下的IP地址,设置为0.0.0.0表示开启DHCP
//函数返回：无
//======================================================================
void WIFI_Init(uint8_t mode_type)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    //初始化全局变量
    WIFI_RECVSTOP = 0;                     //接收模式标志
    WIFI_CONNECT_STATE=0;                  //TCP连接状态
    WIFI_AP_STATE=0;                       //Wi-Fi连接状态
    WIFI_SEND_STATE=0;                     //透传模式下发送模式状态标志
    WIFI_BOOTFLAG = 0;                     //启动完成标志
    WIFI_CMD_FLAG = 0;                     //命令状态标志

    RCC_APB1PeriphClockCmd(WIFI_USART_CLK , ENABLE);
    RCC_APB2PeriphClockCmd(WIFI_USART_GPIO_CLK , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //开启AFIO时钟

    GPIO_PinRemapConfig(GPIO_FullRemap_USART6, ENABLE); //配置GPIO复用功能
    //GPIO初始化
    GPIO_InitStructure.GPIO_Pin = WIFI_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(WIFI_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = WIFI_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(WIFI_USART_RX_GPIO_PORT, &GPIO_InitStructure);
    //串口初始化
    USART_InitStructure.USART_BaudRate = WIFI_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(WIFI_USARTx, &USART_InitStructure);
    USART_ITConfig(WIFI_USARTx, USART_IT_RXNE, ENABLE);
    //配置接收中断
    NVIC_InitStructure.NVIC_IRQChannel = WIFI_USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(WIFI_USARTx,USART_IT_RXNE,ENABLE);
    USART_Cmd(WIFI_USARTx,ENABLE);

     WIFI_SendCMD((uint8_t*)"ATE0\r\n");//关闭回显
     printf("回显关闭成功\r\n");

    //设置通信模式
    switch(mode_type)
    {
        case WIFI_STATION:
        printf("开始设置WIFI模式为STA模式");
        if(WIFI_SetMode(WIFI_STATION) == WIFI_OK)              //设置STA模式
        printf("设置WIFI模式为STA模式成功\r\n");
        MODE_TYPE = 0;                          //标记当前模式为STA模式    
        if(WIFI_SendCMD((uint8_t*)WIFI_AUTO_LINK))   //开启wifi模块自动连接
        printf("WIFI模块自动连接成功\r\n");
        break;
        //AP模式
        //混合模式
        default:
        break;
    }
}

//======================================================================
//函数名称：wifi_mode_set
//功能概要：设置WIFI应用模式
//参数说明：modetype:应用模式,=1 Station模式
//                      =2 AP模式
//                      =3 混合模式
//函数返回：模式设置是否成功=0成功，=1错误，=2无返回数据
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
//函数名称：wifi_send_cmd
//功能概要：发送wifi模块命令，在中断开启的情况下能够返回WiFi模块应答内容
//参数说明：cmdstring:命令字符串
//函数返回：发送后wifi模块应答状态，=0成功，=1错误，=2无应答
//======================================================================
uint8_t WIFI_SendCMD(uint8_t* cmdstring)
{
    //定义局部变量
    uint32_t i;
    uint8_t retryCount = 0;
    //1.初始化全局变量
    WIFI_CMD_FLAG = 0;
    WIFI_RECVSTOP = WIFI_RECV_CMD;//切换接收模式为接收命令返回
    //2.向串口发送指令
    Delay_Ms(3000);
    while(retryCount>=0&&retryCount<4)
    {
    USER_UART_SendString(WIFI_USARTx,cmdstring);   
    i=0;
    Delay_Ms(3000);
    //3.监听成功状态和失败状态
    while(1)
    {

        //3.1 监听到成功状态
        if(WIFI_CMD_FLAG == 1)
        {
            WIFI_RECVSTOP = WIFI_RECV_DATA;    //设置中断接收模式为数据接收模式
            return WIFI_OK;                    //返回命令成功标志WIFI_OK（0）
        }
        //3.2 监听到失败状态
        else if(WIFI_CMD_FLAG == 2)
        {
           
            retryCount++;
            break;
            //return WIFI_ERROR;                 //返回命令失败标志WIFI_ERROR（1）
        }
        i++;                                   //计次标志加一
        //3.3 监听超时
        if(i>0x1ffffff)
        {
            WIFI_RECVSTOP = WIFI_RECV_DATA;    //设置中断接收模式为数据接收模式
            return WIFI_NORESPONSE;            //返回命令超时标志WIFI_NORESPONSE（2）
        }
    }
    }
    WIFI_RECVSTOP = WIFI_RECV_DATA;    //设置中断接收模式为数据接收模式
    return WIFI_ERROR;                         //返回命令失败标志WIFI_ERROR（1）
}

//======================================================================
//函数名称：WIFI_SetIP
//功能概要：设置sta模式下WiFi模块的IP地址
//参数说明：ip:要设置的IP地址
//函数返回：=0设置成功，=1设置失败，=2WiFi模块无应答
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
        //定义局部变量
    uint8_t cmd[40],flag;

    sprintf((char*)cmd,"%s\"%s\"%s\"%s\"\r\n","AT+CWJAP=",(char*)ssid,",",(char*)password);
    flag=WIFI_SendCMD(cmd);
    if(flag == WIFI_OK)
    {
        WIFI_AP_STATE = 1;
    }
    return (flag);
}
//与服务器建立TCP连接
uint8_t WIFI_TCPConnect(uint8_t* server_ip,uint8_t* server_port)
{
    //定义局部变量
    uint8_t flag,cmd[100];
    //1.检测wifi模块当前状态是否处于发送模式
    if(WIFI_SEND_STATE==1)
        return 2;
    //2.检测输入的ip地址是否合法
    if(!WIFI_CheckIP(server_ip))
        return WIFI_ERROR;
    //3.检测输入的port值是否合法
    if(!WIFI_CheckPort(server_port))
        return WIFI_ERROR;
    //4.拼接指令
    sprintf((char*)cmd,"%s\"%s\",\"%s\",%s\r\n",WIFI_START,"TCP",(char*)server_ip,(char*)server_port);
    //5.发送指令
    flag=WIFI_SendCMD(cmd);
    if(flag == WIFI_OK)
    {
        printf("TCP连接建立成功");
        WIFI_CONNECT_STATE = 1;
    }
    return flag;
}
//断开TCP或UDP连接
uint8_t WIFI_Disconnnect()
{
    return WIFI_SendCMD((uint8_t*)WIFI_CLOSE);
}

//===========================内部函数====================================
//======================================================================
//函数名称：WIFI_CheckIP
//功能概要：检查ip地址是否合法
//参数说明：ip:需要检查的ip地址字符串,格式"xxx.xxx.xxx.xxx",xxx取值范围0-255
//函数返回：=1合法，=0不合法
//======================================================================
uint8_t WIFI_CheckIP(uint8_t* ip)
{
    //定义局部变量
    uint8_t i,j,maxlength;           //i用作标记子块,j用作记录子块长度
    uint16_t length,num[4];
    //初始化局部变量
    for(i=0;i<4;i++)
        num[i]=0;
    maxlength=15;
    //1.检查长度并将整个ip字符串切分成四块
    for(length=0,i=0,j=0;*ip!='\0'&&length<=maxlength;length++)
    {
        if(*ip=='.')
        {
            if(j==0||j>3)
                return 0;
            i++;                     //子块标识加一
            j=0;                     //子块长度清零开始计算下一个子块
        }
        else if(*ip>='0'&&*ip<='9')
        {
            num[i]=num[i]*10+(*ip-'0');
            j++;                     //当前子块长度+1
        }
        else return 0;               //检测到非数字返回0
        ip++;                        //转到下个字符
    }
    if(length>maxlength)             //超长,返回0
        return 0;
    //2.检测子块是否小于等于255
    for(i=0;i<4;i++)
    {
        if(!(num[i]<256))            //检测到超过255的子块,返回0
            return 0;
    }
    return 1;                        //所有条件均满足,返回1
}
uint8_t WIFI_CheckPort(uint8_t* port)
{
    //定义局部变量
    uint8_t length,maxlength;
    uint32_t num;
    maxlength=5;    //端口字符串内容最长5字
    for(length=0,num=0;*port!='\0'&&length<maxlength;length++)
    {
        //是数字则计算新的num值，否则返回0
        if(*port>='0'&&*port<='9')
            num=num*10+(*port-'0');
        else return 0;
        port++;
    }
    //超长返回0
    if(length>maxlength)
        return 0;
    //端口值超出范围
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
        //数据接收模式
        case 0:
            printf("%d",data);
            break;
        //命令返回接收模式
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






