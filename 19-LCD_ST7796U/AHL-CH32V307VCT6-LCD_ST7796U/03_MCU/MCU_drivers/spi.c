    //=====================================================================
//�ļ����ƣ�spi.c
//���ܸ�Ҫ��spi�ײ���������Դ�ļ�
//������λ�����ݴ�ѧǶ��ʽϵͳ���������о���(sumcu.suda.edu.cn)
//��    ����  2021-11-3  V1.0
//����оƬ��CH32v103��CH32V307
//=====================================================================
#include "includes.h"
#include "spi.h"

SPI_TypeDef *SPI_ARR[] = {(SPI_TypeDef*)SPI1_BASE, (SPI_TypeDef*)SPI2_BASE,(SPI_TypeDef*)SPI3_BASE};
IRQn_Type table_irq_spi[3] = {SPI1_IRQn, SPI2_IRQn,SPI3_IRQn};
//=====================================================================
//�������ƣ�spi_init��
//����˵����SPI��ʼ��
//����������No��ģ��ţ����ò����ɲμ�gec.h�ļ�
//       MSTR��SPI���ӻ�ѡ��0ѡ��Ϊ�ӻ�,1ѡ��Ϊ������
//       BaudRate�������ʣ���ȡ9000000��4500000��2250000��1125000��562500��281250��140625��λ��bps
//       CPOL��CPOL=0������ЧSPIʱ�ӣ�����Ч����CPOL=1������ЧSPIʱ�ӣ�����Ч��
//       CPHA��CPHA=0��λΪ0�� CPHA=1��λΪ1��
//�������أ���
//=====================================================================
void spi_init(uint8_t No,uint8_t MSTR,uint32_t BaudRate,\
            uint8_t CPOL,uint8_t CPHA)
{
    uint16_t Freq_div;
    uint8_t BaudRate_Mode;
    GPIO_TypeDef *gpio_ptr;
    if(No<SPI_1||No>SPI_3)   No=SPI_1;    //���SPI�Ų���������ǿ��ѡ�� SPI1
    //��1��ʹ��SPI�Ͷ�ӦGPIOʱ��
    switch(No)
    {
        case SPI_1:
        RCC->APB2PCENR |= RCC_SPI1RST;
        RCC->APB2PCENR |= RCC_IOPAEN;
        gpio_ptr = (GPIO_TypeDef *)GPIOA_BASE;
        if(MSTR == 1)    //����ģʽ
        {
            //ʹ��PTA4(74),PTA5(77),PTA6(78),PTA7(79)ΪSPI(NSS,SCK,MISO,MOSI)����
            //����PTA4����Ϊͨ���������������ٶ� 50MHz
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE4);
            gpio_ptr->CFGLR |= GPIO_CFGLR_MODE4;
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF4);
            gpio_ptr->BSHR = (((uint32_t)0x01) << 4);

            //����PTA5����Ϊ�����������������ٶ� 50MHz
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE5);
            gpio_ptr->CFGLR |= (GPIO_CFGLR_MODE5);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF5);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF5_1;

            //����PTA6����Ϊ��������
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE6);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF6);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF6_0;

            //����PTA7����Ϊ�����������������ٶ� 50MHz
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE7);
            gpio_ptr->CFGLR |= (GPIO_CFGLR_MODE7);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF7);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF7_1;
        }
        else    //�ӻ�ģʽ
        {
            //ʹ��PTA4,PTA5,PTA6,PTA7ΪSPI(NSS,SCK,MISO,MOSI)����
            //����PTA4����Ϊ��������
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE4);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF4);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF4_1;
            gpio_ptr->BCR = (((uint32_t)0x01) << 4);

            //����PTA5����Ϊ��������
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE5);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF5);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF5_0;

            //����PTA6����ΪΪ�����������������ٶ� 50MHz
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE6);
            gpio_ptr->CFGLR |= (GPIO_CFGLR_MODE6);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF6);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF6_1;

            //����PTA7����Ϊ��������
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE7);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF7);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF7_0;
        }
        break;

        case SPI_2:
        RCC->APB1PCENR |= RCC_SPI2RST;
        RCC->APB2PCENR |= RCC_IOPBEN;
        gpio_ptr = (GPIO_TypeDef *)GPIOB_BASE;
        if(MSTR == 1)    //����ģʽ
        {
            //ʹ��PTB12(4),PTB13(5),PTB14(6),PTB15(7)ΪSPI(NSS,SCK,MISO,MOSI)����
            //����PTB12����Ϊ�������������ٶ� 50MHz
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE12);
            gpio_ptr->CFGHR |= GPIO_CFGHR_MODE12;
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF12);
            gpio_ptr->BSHR = (((uint32_t)0x01) << 12);

            //����PTB13����Ϊ�����������������ٶ� 50MHz
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE13);
            gpio_ptr->CFGHR |= (GPIO_CFGHR_MODE13);
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF13);
            gpio_ptr->CFGHR |= GPIO_CFGHR_CNF13_1;

            //����PTB14����Ϊ��������
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE14);
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF14);
            gpio_ptr->CFGHR |= GPIO_CFGHR_CNF14_0;

            //����PTB15����Ϊ�����������������ٶ� 50MHz
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE15);
            gpio_ptr->CFGHR |= (GPIO_CFGHR_MODE15);
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF15);
            gpio_ptr->CFGHR |= GPIO_CFGHR_CNF15_1;
        }
        else    //�ӻ�ģʽ
        {
            //ʹ��PTB12,PTB13,PTB14,PTB15ΪSPI(NSS,SCK,MISO,MOSI)����
            //����PTB12����Ϊ��������
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE12);
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF12);
            gpio_ptr->CFGHR |= GPIO_CFGHR_CNF12_1;
            gpio_ptr->BCR = (((uint32_t)0x01) << 12);

            //����PTB13����Ϊ��������
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE13);
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF13);
            gpio_ptr->CFGHR |= GPIO_CFGHR_CNF13_0;

            //����PTB14����Ϊ�����������������ٶ� 50MHz
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE14);
            gpio_ptr->CFGHR |= (GPIO_CFGHR_MODE14);
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF14);
            gpio_ptr->CFGHR |= GPIO_CFGHR_CNF14_1;

            //����PTB15����Ϊ��������
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_MODE15);
            gpio_ptr->CFGHR &= ~(GPIO_CFGHR_CNF15);
            gpio_ptr->CFGHR |= GPIO_CFGHR_CNF15_0;
        }
        break;
        
        case SPI_3:
        RCC->APB1PCENR |= RCC_SPI3RST;
        RCC->APB2PCENR |= (RCC_IOPBEN|RCC_IOPAEN);
        gpio_ptr = (GPIO_TypeDef *)GPIOB_BASE;
        if(MSTR == 1)    //����ģʽ
        {
            //ʹ��PTA15,PTB3,PTB4,PTB5ΪSPI(NSS,SCK,MISO,MOSI)����
            //����PTA15����Ϊ�������������ٶ� 50MHz
            GPIOA->CFGHR &= ~(GPIO_CFGHR_MODE15);
            GPIOA->CFGHR |= GPIO_CFGHR_MODE15;
            GPIOA->CFGHR &= ~(GPIO_CFGHR_CNF15);
            GPIOA->BSHR = (((uint32_t)0x01) << 15);

            //����PTB3����Ϊ�����������������ٶ� 50MHz
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE3);
            gpio_ptr->CFGLR |= (GPIO_CFGLR_MODE3);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF3);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF3_1;

            //����PTB4����Ϊ��������
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE4);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF4);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF4_0;

            //����PTB5����Ϊ�����������������ٶ� 50MHz
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE5);
            gpio_ptr->CFGLR |= (GPIO_CFGLR_MODE5);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF5);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF5_1;
        }
        else    //�ӻ�ģʽ
        {
            //ʹ��PTA15,PTB3,PTB4,PTB5ΪSPI(NSS,SCK,MISO,MOSI)����
            //����PTB12����Ϊ��������
            GPIOA->CFGHR &= ~(GPIO_CFGHR_MODE15);
            GPIOA->CFGHR &= ~(GPIO_CFGHR_CNF15);
            GPIOA->CFGHR |= GPIO_CFGHR_CNF15_1;
            GPIOA->BCR = (((uint32_t)0x01) << 15);

            //����PTB3����Ϊ��������
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE3);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF3);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF3_0;

            //����PTB4����Ϊ�����������������ٶ� 50MHz
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE4);
            gpio_ptr->CFGLR |= (GPIO_CFGLR_MODE4);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF4);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF4_1;

            //����PTB5����Ϊ��������
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_MODE5);
            gpio_ptr->CFGLR &= ~(GPIO_CFGLR_CNF5);
            gpio_ptr->CFGLR |= GPIO_CFGLR_CNF5_0;
        }
        break;
        
    }

    SPI_ARR[No]->CTLR1 &= CTLR1_CLEAR_Mask;
    SPI_ARR[No]->CTLR1 |= (uint16_t)((CPOL<<1) | CPHA);     //����ʱ�Ӽ��Լ���λ
    SPI_ARR[No]->CTLR1 &= ~SPI_CTLR1_DFF;                   //���������ֳ�
    SPI_ARR[No]->CTLR1 &= ~SPI_CTLR1_LSBFIRST;              //����֡��ʽ
    SPI_ARR[No]->CTLR1 |= SPI_CTLR1_SSM;                    //����NSS���ţ����������
    if(MSTR == 1)    //����ģʽ
    {
        //SPI2,SPI3ʹ��APB1ʱ��, SPI1ʹ��APB2ʱ��
        //����SPI������
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

        SPI_ARR[No]->CTLR1 |= SPI_CTLR1_SSI;                //����NSS����Ϊ�ߵ�ƽ
        SPI_ARR[No]->CTLR1 |= SPI_CTLR1_MSTR;               //����Ϊ����ģʽ
    }
    else            //�ӻ�ģʽ
    {
        SPI_ARR[No]->CTLR1 &= ~SPI_CTLR1_SSI;               //�ӻ�ģʽ���� SSI������λ
        SPI_ARR[No]->CTLR1 &= ~SPI_CTLR1_MSTR;              //����Ϊ�ӻ�ģʽ
    }
    SPI_ARR[No]->CTLR1 |= SPI_CTLR1_SPE;            //SPIʹ��
}

//=====================================================================
//�������ƣ�spi_send1.
//����˵����SPI����һ�ֽ����ݡ�
//����������No��ģ��ţ����ò����ɲμ�gec.h�ļ�
//       data��     ��Ҫ���͵�һ�ֽ����ݡ�
//�������أ�0������ʧ�ܣ�1�����ͳɹ���
//=====================================================================
uint8_t spi_send1(uint8_t No,uint8_t data)
{
    volatile uint32_t i;

    i = 0;

    if(No<SPI_1||No>SPI_3)   return 0;  //���SPI�Ų�����������ʧ��

    //��SPIδʹ�ܣ���ʹ��
    if ((SPI_ARR[No]->CTLR1 & SPI_CTLR1_SPE) != SPI_CTLR1_SPE)
    {
        SPI_ARR[No]->CTLR1 |= SPI_CTLR1_SPE;
    }
    //�жϷ��ͻ������Ƿ�Ϊ�ա���Ϊ�գ���������
    while((SPI_ARR[No]->STATR & SPI_STATR_TXE) !=  SPI_STATR_TXE)
    {
        i++;
        if(i>0xfffe) return 0;
    }
    *((__IO uint8_t *)&SPI_ARR[No]->DATAR) = data;
//    printf("1");
//    uart_send1(UART_1,data);
    i = 0;
    //���ջط����ݣ���ֹ���ͻ��������
    while((SPI_ARR[No]->STATR & SPI_STATR_RXNE) !=  SPI_STATR_RXNE)
    {
        i++;
        if(i>0xfffe) return 0;
    }
    //��һ��DR,SR����ֹDR,SR�������
    i = SPI_ARR[No]->DATAR;
    i = SPI_ARR[No]->STATR;
    return 1;
}


//=====================================================================
//�������ƣ�spi_sendN
//����˵����SPI�������ݡ�
//����������No��ģ��ţ����ò����ɲμ�gec.h�ļ�
//       n:     Ҫ���͵��ֽڸ�������ΧΪ(1~255)
//       data[]:����������׵�ַ��
//�������أ��ޡ�
//=====================================================================
uint8_t spi_sendN(uint8_t No,uint8_t n,uint8_t data[])
{
    uint32_t i;

    if(No<SPI_1||No>SPI_3)   return 0;     //���SPI�Ų�����������ʧ��

    for (i = 0; i < n; i++)
    {
        if (!spi_send1(No, data[i])) //����һ���ֽ����ݣ�ʧ��������ѭ��
        {
            break;
        }
    }
    if(i<n)
        return 0;    //���ͳ���
    else
        return 1;    //���ͳɹ�
}

//=====================================================================
//�������ƣ�spi_receive1.
//����˵����SPI����һ���ֽڵ�����
//����������No��ģ��ţ����ò����ɲμ�gec.h�ļ�
//�������أ����յ������ݡ�
//=====================================================================
uint8_t spi_receive1(uint8_t No)
{
    if(No<SPI_1||No>SPI_3)   return 0xff;     //���SPI�Ų�����������ʧ��
    uint32_t i = 0;
    //��SPIδʹ�ܣ���ʹ��
    if ((SPI_ARR[No]->CTLR1 & SPI_CTLR1_SPE) != SPI_CTLR1_SPE)
    {
        SPI_ARR[No]->CTLR1 |= SPI_CTLR1_SPE;
    }

    //�жϽ��ջ������Ƿ�Ϊ�ա�����Ϊ�գ����������
    while((SPI_ARR[No]->STATR & SPI_STATR_RXNE) !=  SPI_STATR_RXNE)
    {
        i++;
        if(i>0xfffe) return 0xff;
    } 
//    printf("%c",SPI_ARR[No]->DATAR);
    return SPI_ARR[No]->DATAR;
}

//=====================================================================
//�������ƣ�spi_receiveN.
//����˵����SPI�������ݡ���n=1ʱ�����ǽ���һ���ֽڵ����ݡ���
//����������No��ģ��ţ����ò����ɲμ�gec.h�ļ�
//        n:    Ҫ���͵��ֽڸ�������ΧΪ(1~255),
//       data[]:���յ������ݴ�ŵ��׵�ַ��
//�������أ�1�����ճɹ�,���������ʧ�ܡ�
//=====================================================================
uint8_t spi_receiveN(uint8_t No,uint8_t n,uint8_t data[])
{
    if(No<SPI_1||No>SPI_3)   return 0;   //���SPI�Ų�����������ʧ��
    uint32_t i;
    for (i = 0; i < n; i++)
    {
        data[i] = spi_receive1(No);
        if (!data[i]) //����һ���ֽ����ݣ�ʧ��������ѭ��
        {
            break;
        }
    }
    if(i<n)
        return 0;    //���ճ���
    else
        return 1;    //���ճɹ�
}

//=====================================================================
//�������ƣ�spi_enable_re_int
//����˵������SPI�����жϡ�
//����������No��ģ��ţ����ò����ɲμ�gec.h�ļ�
//�������أ��ޡ�
//=====================================================================
void spi_enable_re_int(uint8_t No)
{
    if(No<SPI_1||No>SPI_3)   return;    //���SPI�Ų�����������ʧ��
    SPI_ARR[No]->CTLR2 |= SPI_CTLR2_RXNEIE;     //����SPI�����ж�
    NVIC_EnableIRQ(table_irq_spi[No]);     //���жϿ�����IRQ�ж�
}

//=====================================================================
//�������ƣ�spi_disable_re_int
//����˵�����ر�SPI�����жϡ�
//����������No��ģ��ţ����ò����ɲμ�gec.h�ļ�
//�������أ��ޡ�
//=====================================================================
void spi_disable_re_int(uint8_t No)
{
    if(No<SPI_1||No>SPI_3)   return;   //���SPI�Ų�����������ʧ��
    SPI_ARR[No]->CTLR2 &= ~SPI_CTLR2_RXNEIE;    //����SPI�����ж�
    NVIC_DisableIRQ(table_irq_spi[No]);    //��ֹ�жϿ�����IRQ�ж�
}

