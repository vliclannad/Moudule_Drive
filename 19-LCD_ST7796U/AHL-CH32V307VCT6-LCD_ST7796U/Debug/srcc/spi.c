    //=====================================================================
//文件名称：spi.c
//功能概要：spi底层驱动构件源文件
//制作单位：苏州大学嵌入式系统与物联网研究所(sumcu.suda.edu.cn)
//版    本：  2021-11-3  V1.0
//适用芯片：CH32v103、CH32V307
//=====================================================================
#include "includes.h"
#include "spi.h"

SPI_TypeDef *SPI_ARR[] = {(SPI_TypeDef*)SPI1_BASE, (SPI_TypeDef*)SPI2_BASE,(SPI_TypeDef*)SPI3_BASE};
IRQn_Type table_irq_spi[3] = {SPI1_IRQn, SPI2_IRQn,SPI3_IRQn};
//=====================================================================
//函数名称：spi_init。
//功能说明：SPI初始化
//函数参数：No：模块号，可用参数可参见gec.h文件
//       MSTR：SPI主从机选择，0选择为从机,1选择为主机。
//       BaudRate：波特率，可取9000000、4500000、2250000、1125000、562500、281250，140625单位：bps
//       CPOL：CPOL=0：高有效SPI时钟（低无效）；CPOL=1：低有效SPI时钟（高无效）
//       CPHA：CPHA=0相位为0； CPHA=1相位为1；
//函数返回：无
//=====================================================================
void spi_init(uint8_t No,uint8_t MSTR,uint32_t BaudRate,\
            uint8_t CPOL,uint8_t CPHA)
{
    uint16_t Freq_div;
    uint8_t BaudRate_Mode;
    GPIO_TypeDef *gpio_ptr;
    if(No<SPI_1||No>SPI_3)   No=SPI_1;    //如果SPI号参数错误则强制选择 SPI1
    //（1）使能SPI和对应GPIO时钟
    switch(No)
    {
        case SPI_1:
        RCC->APB2PCENR |= RCC_SPI1RST;
        RCC->APB2PCENR |= RCC_IOPAEN;
        gpio_ptr = (GPIO_TypeDef *)GPIOA_BASE;
        if(MSTR == 1)    //主机模式
        {
            //使能PTA4(74),PTA5(77),PTA6(78),PTA7(79)为SPI(NSS,SCK,MISO,MOSI)功能
            //配置PTA4引脚为通用推挽输出，最大速度 50MHz
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE4);
            gpio_ptr->CFGLR |= GPIO_CFGLR_MODE4;
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF4);
            gpio_ptr->BSHR = (((uint32_t)0x01) << 4);

            //配置PTA5引脚为复用推挽输出，最大速度 50MHz
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE5);
            gpio_ptr->CFGLR |= (GPIO_CFGLR_MODE5);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF5);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF5_1;

            //配置PTA6引脚为浮空输入
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE6);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF6);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF6_0;

            //配置PTA7引脚为复用推挽输出，最大速度 50MHz
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE7);
            gpio_ptr->CFGLR |= (GPIO_CFGLR_MODE7);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF7);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF7_1;
        }
        else    //从机模式
        {
            //使能PTA4,PTA5,PTA6,PTA7为SPI(NSS,SCK,MISO,MOSI)功能
            //配置PTA4引脚为下拉输入
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE4);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF4);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF4_1;
            gpio_ptr->BCR = (((uint32_t)0x01) << 4);

            //配置PTA5引脚为浮空输入
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE5);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF5);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF5_0;

            //配置PTA6引脚为为复用推挽输出，最大速度 50MHz
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE6);
            gpio_ptr->CFGLR |= (GPIO_CFGLR_MODE6);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF6);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF6_1;

            //配置PTA7引脚为浮空输入
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE7);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF7);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF7_0;
        }
        break;

        case SPI_2:
        RCC->APB1PCENR |= RCC_SPI2RST;
        RCC->APB2PCENR |= RCC_IOPBEN;
        gpio_ptr = (GPIO_TypeDef *)GPIOB_BASE;
        if(MSTR == 1)    //主机模式
        {
            //使能PTB12(4),PTB13(5),PTB14(6),PTB15(7)为SPI(NSS,SCK,MISO,MOSI)功能
            //配置PTB12引脚为推挽输出，最大速度 50MHz
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE12);
            gpio_ptr->CFGHR |= GPIO_CFGHR_MODE12;
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF12);
            gpio_ptr->BSHR = (((uint32_t)0x01) << 12);

            //配置PTB13引脚为复用推挽输出，最大速度 50MHz
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE13);
            gpio_ptr->CFGHR |= (GPIO_CFGHR_MODE13);
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF13);
            gpio_ptr->CFGHR |= GPIO_CFGHR_CNF13_1;

            //配置PTB14引脚为浮空输入
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE14);
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF14);
            gpio_ptr->CFGHR |= GPIO_CFGHR_CNF14_0;

            //配置PTB15引脚为复用推挽输出，最大速度 50MHz
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE15);
            gpio_ptr->CFGHR |= (GPIO_CFGHR_MODE15);
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF15);
            gpio_ptr->CFGHR |= GPIO_CFGHR_CNF15_1;
        }
        else    //从机模式
        {
            //使能PTB12,PTB13,PTB14,PTB15为SPI(NSS,SCK,MISO,MOSI)功能
            //配置PTB12引脚为下拉输入
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE12);
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF12);
            gpio_ptr->CFGHR |= GPIO_CFGHR_CNF12_1;
            gpio_ptr->BCR = (((uint32_t)0x01) << 12);

            //配置PTB13引脚为浮空输入
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE13);
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF13);
            gpio_ptr->CFGHR |= GPIO_CFGHR_CNF13_0;

            //配置PTB14引脚为复用推挽输出，最大速度 50MHz
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE14);
            gpio_ptr->CFGHR |= (GPIO_CFGHR_MODE14);
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF14);
            gpio_ptr->CFGHR |= GPIO_CFGHR_CNF14_1;

            //配置PTB15引脚为浮空输入
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE15);
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF15);
            gpio_ptr->CFGHR |= GPIO_CFGHR_CNF15_0;
        }
        break;
        
        case SPI_3:
        RCC->APB1PCENR |= RCC_SPI3RST;
        RCC->APB2PCENR |= (RCC_IOPBEN|RCC_IOPAEN);
        gpio_ptr = (GPIO_TypeDef *)GPIOB_BASE;
        if(MSTR == 1)    //主机模式
        {
            //使能PTA15,PTB3,PTB4,PTB5为SPI(NSS,SCK,MISO,MOSI)功能
            //配置PTA15引脚为推挽输出，最大速度 50MHz
            GPIOA->CFGHR &= ~(GPIO_CFGHR_MODE15);
            GPIOA->CFGHR |= GPIO_CFGHR_MODE15;
            GPIOA->CFGHR &= ~(GPIO_CFGHR_CNF15);
            GPIOA->BSHR = (((uint32_t)0x01) << 15);

            //配置PTB3引脚为复用推挽输出，最大速度 50MHz
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE3);
            gpio_ptr->CFGLR |= (GPIO_CFGLR_MODE3);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF3);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF3_1;

            //配置PTB4引脚为浮空输入
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE4);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF4);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF4_0;

            //配置PTB5引脚为复用推挽输出，最大速度 50MHz
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE5);
            gpio_ptr->CFGLR |= (GPIO_CFGLR_MODE5);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF5);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF5_1;
        }
        else    //从机模式
        {
            //使能PTA15,PTB3,PTB4,PTB5为SPI(NSS,SCK,MISO,MOSI)功能
            //配置PTB12引脚为下拉输入
            GPIOA->CFGHR &= ~(GPIO_CFGHR_MODE15);
            GPIOA->CFGHR &= ~(GPIO_CFGHR_CNF15);
            GPIOA->CFGHR |= GPIO_CFGHR_CNF15_1;
            GPIOA->BCR = (((uint32_t)0x01) << 15);

            //配置PTB3引脚为浮空输入
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE3);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF3);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF3_0;

            //配置PTB4引脚为复用推挽输出，最大速度 50MHz
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE4);
            gpio_ptr->CFGLR |= (GPIO_CFGLR_MODE4);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF4);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF4_1;

            //配置PTB5引脚为浮空输入
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE5);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF5);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF5_0;
        }
        break;
        
    }

    SPI_ARR[No]->CTLR1 &= CTLR1_CLEAR_Mask;
    SPI_ARR[No]->CTLR1 |= (uint16_t)((CPOL<<1) | CPHA);     //配置时钟极性及相位
    SPI_ARR[No]->CTLR1 &= ~SPI_CTLR1_DFF;                   //配置数据字长
    SPI_ARR[No]->CTLR1 &= ~SPI_CTLR1_LSBFIRST;              //配置帧格式
    SPI_ARR[No]->CTLR1 |= SPI_CTLR1_SSM;                    //配置NSS引脚，由软件控制
    if(MSTR == 1)    //主机模式
    {
        //SPI2,SPI3使用APB1时钟, SPI1使用APB2时钟
        //配置SPI波特率
        if(No == SPI_1)
        {
            Freq_div = SystemCoreClock/4/BaudRate;
        }
        else
        {
            Freq_div = SystemCoreClock/2/BaudRate;
        }
        BaudRate_Mode = 0;
        while(Freq_div/2 >= 2)
        {
            BaudRate_Mode++;
            Freq_div = Freq_div/2;
        }
        SPI_ARR[No]->CTLR1 |= (BaudRate_Mode<<3);

        SPI_ARR[No]->CTLR1 |= SPI_CTLR1_SSI;                //配置NSS引脚为高电平
        SPI_ARR[No]->CTLR1 |= SPI_CTLR1_MSTR;               //配置为主机模式
    }
    else            //从机模式
    {
        SPI_ARR[No]->CTLR1 &= ~SPI_CTLR1_SSI;               //从机模式保持 SSI不被置位
        SPI_ARR[No]->CTLR1 &= ~SPI_CTLR1_MSTR;              //配置为从机模式
    }
    SPI_ARR[No]->CTLR1 |= SPI_CTLR1_SPE;            //SPI使能
}

//=====================================================================
//函数名称：spi_send1.
//功能说明：SPI发送一字节数据。
//函数参数：No：模块号，可用参数可参见gec.h文件
//       data：     需要发送的一字节数据。
//函数返回：0：发送失败；1：发送成功。
//=====================================================================
uint8_t spi_send1(uint8_t No,uint8_t data)
{
    volatile uint32_t i;

    i = 0;

    if(No<SPI_1||No>SPI_3)   return 0;  //如果SPI号参数错误则发送失败

    //若SPI未使能，则使能
    if ((SPI_ARR[No]->CTLR1 & SPI_CTLR1_SPE) != SPI_CTLR1_SPE)
    {
        SPI_ARR[No]->CTLR1 |= SPI_CTLR1_SPE;
    }
    //判断发送缓冲区是否为空。若为空，则发送数据
    while((SPI_ARR[No]->STATR & SPI_STATR_TXE) !=  SPI_STATR_TXE)
    {
        i++;
        if(i>0xfffe) return 0;
    }
    *((__IO uint8_t *)&SPI_ARR[No]->DATAR) = data;
//    printf("1");
//    uart_send1(UART_1,data);
    i = 0;
    //接收回发数据，防止发送缓冲区溢出
    while((SPI_ARR[No]->STATR & SPI_STATR_RXNE) !=  SPI_STATR_RXNE)
    {
        i++;
        if(i>0xfffe) return 0;
    }
    //读一次DR,SR，防止DR,SR不被清空
    i = SPI_ARR[No]->DATAR;
    i = SPI_ARR[No]->STATR;
    return 1;
}


//=====================================================================
//函数名称：spi_sendN
//功能说明：SPI发送数据。
//函数参数：No：模块号，可用参数可参见gec.h文件
//       n:     要发送的字节个数。范围为(1~255)
//       data[]:所发数组的首地址。
//函数返回：无。
//=====================================================================
uint8_t spi_sendN(uint8_t No,uint8_t n,uint8_t data[])
{
    uint32_t i;

    if(No<SPI_1||No>SPI_3)   return 0;     //如果SPI号参数错误则发送失败

    for (i = 0; i < n; i++)
    {
        if (!spi_send1(No, data[i])) //发送一个字节数据，失败则跳出循环
        {
            break;
        }
    }
    if(i<n)
        return 0;    //发送出错
    else
        return 1;    //发送成功
}

//=====================================================================
//函数名称：spi_receive1.
//功能说明：SPI接收一个字节的数据
//函数参数：No：模块号，可用参数可参见gec.h文件
//函数返回：接收到的数据。
//=====================================================================
uint8_t spi_receive1(uint8_t No)
{
    if(No<SPI_1||No>SPI_3)   return 0xff;     //如果SPI号参数错误则发送失败
    uint32_t i = 0;
    //若SPI未使能，则使能
    if ((SPI_ARR[No]->CTLR1 & SPI_CTLR1_SPE) != SPI_CTLR1_SPE)
    {
        SPI_ARR[No]->CTLR1 |= SPI_CTLR1_SPE;
    }

    //判断接收缓冲区是否不为空。若不为空，则接收数据
    while((SPI_ARR[No]->STATR & SPI_STATR_RXNE) !=  SPI_STATR_RXNE)
    {
        i++;
        if(i>0xfffe) return 0xff;
    } 
//    printf("%c",SPI_ARR[No]->DATAR);
    return SPI_ARR[No]->DATAR;
}

//=====================================================================
//函数名称：spi_receiveN.
//功能说明：SPI接收数据。当n=1时，就是接受一个字节的数据……
//函数参数：No：模块号，可用参数可参见gec.h文件
//        n:    要发送的字节个数。范围为(1~255),
//       data[]:接收到的数据存放的首地址。
//函数返回：1：接收成功,其他情况：失败。
//=====================================================================
uint8_t spi_receiveN(uint8_t No,uint8_t n,uint8_t data[])
{
    if(No<SPI_1||No>SPI_3)   return 0;   //如果SPI号参数错误则发送失败
    uint32_t i;
    for (i = 0; i < n; i++)
    {
        data[i] = spi_receive1(No);
        if (!data[i]) //发送一个字节数据，失败则跳出循环
        {
            break;
        }
    }
    if(i<n)
        return 0;    //接收出错
    else
        return 1;    //接收成功
}

//=====================================================================
//函数名称：spi_enable_re_int
//功能说明：打开SPI接收中断。
//函数参数：No：模块号，可用参数可参见gec.h文件
//函数返回：无。
//=====================================================================
void spi_enable_re_int(uint8_t No)
{
    if(No<SPI_1||No>SPI_3)   return;    //如果SPI号参数错误则发送失败
    SPI_ARR[No]->CTLR2 |= SPI_CTLR2_RXNEIE;     //开放SPI接收中断
    NVIC_EnableIRQ(table_irq_spi[No]);     //开中断控制器IRQ中断
}

//=====================================================================
//函数名称：spi_disable_re_int
//功能说明：关闭SPI接收中断。
//函数参数：No：模块号，可用参数可参见gec.h文件
//函数返回：无。
//=====================================================================
void spi_disable_re_int(uint8_t No)
{
    if(No<SPI_1||No>SPI_3)   return;   //如果SPI号参数错误则发送失败
    SPI_ARR[No]->CTLR2 &= ~SPI_CTLR2_RXNEIE;    //禁用SPI接收中断
    NVIC_DisableIRQ(table_irq_spi[No]);    //禁止中断控制器IRQ中断
}

