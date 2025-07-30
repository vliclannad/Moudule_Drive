#include "CAT1.h"
#include "string.h"
#include "stdio.h"
#include "user_usart.h"


void CAT_USART_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

//CAT传输模式设置
#define CAT_CIP_MODE "AT+MIPMODE=0," //模式设置，0普通模式，1透传模式

//CAT通信指令
#define CAT_START "AT+MIPOPEN=0,"         //建立连接
#define CAT_CLOSE "AT+MIPCLOSE\r\n"      //关闭连接

//CAT 通信指令接收用全局变量
static uint8_t  CAT_CMD[128];   //命令返回数据接收缓冲区
static uint8_t  CAT_CMD_FLAG;


static uint8_t  CAT_RECVSTOP;    //接收模式标志

static uint8_t MODE_TYPE;
static uint8_t CAT_CONNECT_STATE;  //服务器连接状态
static uint8_t CAT_AP_STATE;       //CAT连接状态
static uint8_t CAT_SEND_STATE;     //CAT发送模式
static uint8_t CAT_BOOTFLAG;

uint8_t CAT_SendCMD(uint8_t* cmdstring);
uint8_t CAT_CheckIP(uint8_t* ip);
uint8_t CAT_CheckPort(uint8_t* port);
void CAT_CMD_ReturnCheck();
void CAT_ENABLE();
//======================================================================
//函数名称：CAT_Init
//功能概要：初始化CAT模块
//参数说明：mode_type:决定模块作为客户端还是AP接入点;
//         mode_name:设置模块AP模式下的SSID;
//         pssword:要设置的密码字符串，必须是64位以内ASCII字符;
//         chn:CAT信道号，1~13，只有1、6、11三个信道互不干扰;
//         ip:设置Wi-Fi模块STA模式下的IP地址,设置为0.0.0.0表示开启DHCP
//函数返回：无
//======================================================================
void CAT_Init(uint8_t mode_type)
{
    uint8_t test = 0xf1;
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    //初始化全局变量
    CAT_RECVSTOP = 0;                     //接收模式标志
    CAT_CONNECT_STATE=0;                  //TCP连接状态
    CAT_AP_STATE=0;                       //Wi-Fi连接状态
    CAT_SEND_STATE=0;                     //透传模式下发送模式状态标志
    CAT_BOOTFLAG = 0;                     //启动完成标志
    CAT_CMD_FLAG = 0;                     //命令状态标志

    RCC_APB1PeriphClockCmd(CAT_USART_CLK , ENABLE);
    RCC_APB2PeriphClockCmd(CAT_USART_GPIO_CLK , ENABLE);
    RCC_APB2PeriphClockCmd(CAT_Reboot_GPIO_CLK, ENABLE); //使能CAT使能GPIO引脚时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //使能复用功能时钟

    GPIO_PinRemapConfig(GPIO_FullRemap_USART5, ENABLE);
    //GPIO初始化
    GPIO_InitStructure.GPIO_Pin = CAT_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(CAT_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = CAT_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(CAT_USART_RX_GPIO_PORT, &GPIO_InitStructure);
    //使能CAT模块
    GPIO_InitStructure.GPIO_Pin = CAT_Reboot_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CAT_Reboot_GPIO_PORT, &GPIO_InitStructure);

    GPIO_SetBits(CAT_Reboot_GPIO_PORT, CAT_Reboot_GPIO_PIN); //CAT使能引脚拉高

    //串口初始化
    USART_InitStructure.USART_BaudRate = CAT_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(CAT_USARTx, &USART_InitStructure);
    USART_ITConfig(CAT_USARTx, USART_IT_RXNE, ENABLE);
    //配置接收中断
    NVIC_InitStructure.NVIC_IRQChannel = CAT_USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(CAT_USARTx,USART_IT_RXNE,ENABLE);
    USART_Cmd(CAT_USARTx,ENABLE);

    //Delay_Ms(2000);
    //CAT_SendCMD((uint8_t*)"ATE0\r\n");//关闭回显
    //printf("回显关闭成功\r\n");

    //设置通信模式
    switch(mode_type)
    {
        case CAT_STATION:
        CAT_ENABLE();
        printf("CAT模块上电成功\r\n");
        MODE_TYPE = 0;                          //标记当前模式为STA模式    
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
//函数名称：CAT_send_cmd
//功能概要：发送CAT模块命令，在中断开启的情况下能够返回CAT模块应答内容
//参数说明：cmdstring:命令字符串
//函数返回：发送后CAT模块应答状态，=0成功，=1错误，=2无应答
//======================================================================
uint8_t CAT_SendCMD(uint8_t* cmdstring)
{
    //定义局部变量
    uint32_t i;
    uint8_t retryCount = 0;
    //1.初始化全局变量
    CAT_CMD_FLAG = 0;
    CAT_RECVSTOP = CAT_RECV_CMD;//切换接收模式为接收命令返回
    //2.向串口发送指令
    Delay_Ms(3000);
    while(retryCount>=0&&retryCount<4)
    {
    USER_UART_SendString(CAT_USARTx,cmdstring);   
    i=0;
    Delay_Ms(3000);
    //3.监听成功状态和失败状态
    while(1)
    {

        //3.1 监听到成功状态
        if(CAT_CMD_FLAG == 1)
        {
            CAT_RECVSTOP = CAT_RECV_DATA;    //设置中断接收模式为数据接收模式
            return CAT_OK;                    //返回命令成功标志CAT_OK（0）
        }
        //3.2 监听到失败状态
        else if(CAT_CMD_FLAG == 2)
        {
           
            retryCount++;
            break;
            //return CAT_ERROR;                 //返回命令失败标志CAT_ERROR（1）
        }
        i++;                                   //计次标志加一
        //3.3 监听超时
        if(i>0x1ffffff)
        {
            CAT_RECVSTOP = CAT_RECV_DATA;    //设置中断接收模式为数据接收模式
            return CAT_NORESPONSE;            //返回命令超时标志CAT_NORESPONSE（2）
        }
    }
    }
    CAT_RECVSTOP = CAT_RECV_DATA;    //设置中断接收模式为数据接收模式
    return CAT_ERROR;                         //返回命令失败标志CAT_ERROR（1）
}
//与服务器建立TCP连接
uint8_t CAT_TCPConnect(uint8_t* server_ip,uint8_t* server_port)
{
    //定义局部变量
    uint8_t flag,cmd[100];
    //1.检测CAT模块当前状态是否处于发送模式
    if(CAT_SEND_STATE==1)
        return 2;
    //2.检测输入的ip地址是否合法
    if(!CAT_CheckIP(server_ip))
        return CAT_ERROR;
    //3.检测输入的port值是否合法
    if(!CAT_CheckPort(server_port))
        return CAT_ERROR;
    //4.拼接指令
    sprintf((char*)cmd,"%s\"%s\",\"%s\",%s,%d,%d\r\n",CAT_START,"TCP",(char*)server_ip,(char*)server_port,60,0);
    //5.发送指令
    printf("%s\r\n",cmd);
    flag=CAT_SendCMD(cmd);
    if(flag == CAT_OK)
    {
        printf("TCP连接建立成功");
        CAT_CONNECT_STATE = 1;
    }
    return flag;
}
//断开TCP或UDP连接
uint8_t CAT_Disconnnect()
{
    return CAT_SendCMD((uint8_t*)CAT_CLOSE);
}

//===========================内部函数====================================
//======================================================================
//函数名称：CAT_CheckIP
//功能概要：检查ip地址是否合法
//参数说明：ip:需要检查的ip地址字符串,格式"xxx.xxx.xxx.xxx",xxx取值范围0-255
//函数返回：=1合法，=0不合法
//======================================================================
uint8_t CAT_CheckIP(uint8_t* ip)
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
uint8_t CAT_CheckPort(uint8_t* port)
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

void CAT_USART_IRQHandler()
{
    if(USART_GetITStatus(CAT_USARTx, USART_IT_RXNE) != RESET)
    {
        
        static uint16_t index = 0;
        uint8_t  data = USART_ReceiveData(CAT_USARTx);
        switch (CAT_RECVSTOP)
        {
        //数据接收模式
        case 0:
            printf("%s",data);
            break;
        //命令返回接收模式
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
        printf("TCP链接成功\r\n");
        CAT_CMD_FLAG = 1;
    }
    else if(strstr(CAT_CMD,"OK"))
    {
        printf("Command executed successfully\r\n");
        CAT_CMD_FLAG = 1;
    }
    else if(strstr(CAT_CMD,"CONNECT"))
    {
        printf("成功进入透传模式\r\n");
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

