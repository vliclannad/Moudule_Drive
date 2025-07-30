//======================================================================
//�ļ����ƣ�uart.c
//���ܸ�Ҫ��uart�ײ���������Դ�ļ�
//��Ȩ���У����ݴ�ѧǶ��ʽϵͳ���������о���(sumcu.suda.edu.cn)
//���¼�¼��2020-0918 V1.0 GXY
//         2022-0124    �޸Ĵ������ų�ʼ��
//======================================================================

#include "uart.h"


USART_TypeDef *USART_ARR[] =  {(USART_TypeDef*)USART1_BASE, (USART_TypeDef*)USART2_BASE, (USART_TypeDef*)USART3_BASE,
        (USART_TypeDef*)UART4_BASE,(USART_TypeDef*)UART5_BASE,(USART_TypeDef*)UART6_BASE,(USART_TypeDef*)UART7_BASE,
        (USART_TypeDef*)UART8_BASE,};

//====���崮��IRQ�Ŷ�Ӧ��====
IRQn_Type table_irq_uart[8] = {USART1_IRQn, USART2_IRQn, USART3_IRQn,UART4_IRQn,UART5_IRQn,UART6_IRQn,UART7_IRQn,UART8_IRQn};


//�ڲ���������
uint8_t uart_is_uartNo(uint8_t uartNo);


//======================================================================
//�������ƣ�uart_init
//���ܸ�Ҫ����ʼ��uartģ��
//����˵����uartNo:���ںţ�UART_1��UART_2��UART_3
//          baud:�����ʣ�300��600��1200��2400��4800��9600��19200��115200...
//�������أ���
//======================================================================
void uart_init(uint8_t uartNo, uint32_t baud_rate)
{
    uint16_t  DIV_M,DIV_F; //BRR�Ĵ���Ӧ����ֵ

    //�жϴ��봮�ںŲ����Ƿ�����,����ֱ���˳�
    if(!uart_is_uartNo(uartNo))
    {
        return;
    }

    //����UARTģ���GPIOģ�����Χʱ�ӣ���ʹ�����ŵ�UART����
    switch(uartNo)
    {
    case UART_1:  //��Ϊ����1
#ifdef UART1_GROUP
        //����ѡ��ʹ�ܶ�Ӧʱ�ӣ������ö�Ӧ����ΪUART_1
        switch(UART1_GROUP)
        {
        case 0:
            //ʹ��USART1��GPIOAʱ��
            RCC->APB2PCENR |= RCC_USART1EN;
            RCC->APB2PCENR |= RCC_AFIOEN;
            RCC->APB2PCENR |= RCC_IOPAEN;
            //ʹ��PTA9,PTA10ΪUSART(Tx,Rx)����
            GPIOA->CFGHR &= ~(GPIO_CFGHR_MODE9|GPIO_CFGHR_CNF9|GPIO_CFGHR_CNF10|GPIO_CFGHR_MODE10);
            GPIOA->CFGHR |= ((GPIO_CFGHR_MODE9_1|GPIO_CFGHR_CNF9_1)|(GPIO_CFGHR_CNF10_1));
            AFIO->PCFR1 |= 0;

            break;
        case 1:
            //ʹ��USART1��GPIOBʱ��
            RCC->APB2PCENR |= RCC_USART1EN;
            RCC->APB2PCENR |= RCC_IOPBEN;
            RCC->APB2PCENR |= RCC_AFIOEN;
            //ʹ��PTB6,PTB7ΪUSART(Tx,Rx)����
            GPIOB->CFGLR &= ~(GPIO_CFGLR_MODE6|GPIO_CFGLR_MODE7|GPIO_CFGLR_CNF6|GPIO_CFGLR_CNF7);
            GPIOB->CFGLR |= ((GPIO_CFGLR_CNF6_1|GPIO_CFGLR_MODE6_1)|GPIO_CFGLR_CNF7_0);
            AFIO->PCFR1 |= AFIO_PCFR1_USART1_REMAP;
            break;
        default:
            break;
        }
#endif
        break;

    case UART_2:  //��Ϊ����2
#ifdef UART2_GROUP
        //����ѡ��ʹ�ܶ�Ӧʱ�ӣ������ö�Ӧ����ΪUART_2
        switch(UART2_GROUP)
        {
        case 0:
            //ʹ��USART2��GPIOAʱ��
            RCC->APB1PCENR |= RCC_USART2EN;
            RCC->APB2PCENR |= RCC_IOPAEN;
            //ʹ��PTA2,PTA3ΪUSART(Tx,Rx)����
            GPIOA->CFGLR &= ~(GPIO_CFGLR_MODE2|GPIO_CFGLR_MODE3|GPIO_CFGLR_CNF2|GPIO_CFGLR_CNF3);
            GPIOA->CFGLR |= ((GPIO_CFGLR_CNF2_1|GPIO_CFGLR_MODE2)|(GPIO_CFGLR_CNF3_0));
            break;
        case 1:
            //ʹ��USART1��GPIOBʱ��
            RCC->APB2PCENR |= RCC_USART2EN;
            RCC->APB2PCENR |= RCC_IOPDEN;
            RCC->APB2PCENR |= RCC_AFIOEN;
            //ʹ��PTD5,PTD6ΪUSART(Tx,Rx)����
            GPIOD->CFGLR &= ~(GPIO_CFGLR_MODE5|GPIO_CFGLR_MODE6|GPIO_CFGLR_CNF5|GPIO_CFGLR_CNF6);
            GPIOD->CFGLR |= ((GPIO_CFGLR_CNF5_1|GPIO_CFGLR_MODE5_1)|GPIO_CFGLR_CNF6_0);
            AFIO->PCFR1 |= AFIO_PCFR1_USART2_REMAP;

            break;
        default:
            break;
        }
#endif
        break;

    case UART_3:  //��Ϊ����3
#ifdef UART3_GROUP
        //����ѡ��ʹ�ܶ�Ӧʱ�ӣ������ö�Ӧ����ΪUART_3
        switch(UART3_GROUP)
        {
        case 0:
            //ʹ��USART3��GPIOBʱ��
            RCC->APB1PCENR |= RCC_USART3EN;
            RCC->APB2PCENR |= RCC_IOPBEN;
            //ʹ��PTB10,PTB11ΪUSART(Tx,Rx)����
            GPIOB->CFGHR &= ~(GPIO_CFGHR_MODE10|GPIO_CFGHR_MODE11|GPIO_CFGHR_CNF10|GPIO_CFGHR_CNF11);
            GPIOB->CFGHR |= ((GPIO_CFGHR_CNF10_1|GPIO_CFGHR_MODE10_1)|(GPIO_CFGHR_CNF11_0));
            break;
        case 1:
            //ʹ��USART1��GPIOCʱ��
            RCC->APB1PCENR |= RCC_USART3EN;
            RCC->APB2PCENR |= RCC_IOPCEN;
            RCC->APB2PCENR |= RCC_AFIOEN;
            //ʹ��PTC10,P11ΪUSART(Tx,Rx)����
            GPIOC->CFGHR &= ~(GPIO_CFGHR_MODE10|GPIO_CFGHR_MODE11|GPIO_CFGHR_CNF10|GPIO_CFGHR_CNF11);
            GPIOC->CFGHR |= ((GPIO_CFGHR_CNF10_1|GPIO_CFGHR_MODE10_1)|(GPIO_CFGHR_CNF11_0));
            AFIO->PCFR1 |= (AFIO_PCFR1_USART3_REMAP_0);

            break;
        default:
            break;
        }
#endif
        break;

    case UART_4:  //��Ϊ����4
#ifdef UART4_GROUP
        //����ѡ��ʹ�ܶ�Ӧʱ�ӣ������ö�Ӧ����ΪUART_3
        switch(UART4_GROUP)
        {
        case 0:
            //ʹ��UART4��GPIOCʱ��
            RCC->APB1PCENR |= RCC_UART4EN;
            RCC->APB2PCENR |= RCC_IOPCEN;
            //ʹ��PTC10,PTC11ΪUART4(Tx,Rx)����
            GPIOC->CFGHR &= ~(GPIO_CFGHR_MODE10|GPIO_CFGHR_MODE11|GPIO_CFGHR_CNF10|GPIO_CFGHR_CNF11);
            GPIOC->CFGHR |= ((GPIO_CFGHR_CNF10_1|GPIO_CFGHR_MODE10_1)|(GPIO_CFGHR_CNF11_0));
            break;
        case 1:
            //ʹ��UART4��GPIOBʱ��
            RCC->APB1PCENR |= RCC_UART4EN;
            RCC->APB2PCENR |= RCC_IOPBEN;
            RCC->APB2PCENR |= RCC_AFIOEN;
            //ʹ��PTB0,PTB1ΪUSART(Tx,Rx)����
            GPIOB->CFGLR &= ~(GPIO_CFGLR_MODE0|GPIO_CFGLR_MODE1|GPIO_CFGLR_CNF0|GPIO_CFGLR_CNF1);
            GPIOB->CFGLR |= ((GPIO_CFGLR_CNF0_1|GPIO_CFGLR_MODE0_1)|(GPIO_CFGLR_CNF1_0));
            AFIO->PCFR2 |= ((uint32_t)0x00010000);

            break;
        case 2:
            //ʹ��UART4��GPIOEʱ��
            RCC->APB1PCENR |= RCC_UART4EN;
            RCC->APB2PCENR |= RCC_IOPEEN;
            RCC->APB2PCENR |= RCC_AFIOEN;
            //ʹ��PTE0,PTE1ΪUSART(Tx,Rx)����
            GPIOE->CFGLR &= ~(GPIO_CFGLR_MODE0|GPIO_CFGLR_MODE1|GPIO_CFGLR_CNF0|GPIO_CFGLR_CNF1);
            GPIOE->CFGLR |= ((GPIO_CFGLR_CNF0_1|GPIO_CFGLR_MODE0_1)|(GPIO_CFGLR_CNF1_0));
            AFIO->PCFR2 |= ((uint32_t)0x00020000);

            break;
        default:
            break;
        }
#endif
        break;

    case UART_5:  //��Ϊ����5
#ifdef UART5_GROUP
        //����ѡ��ʹ�ܶ�Ӧʱ�ӣ������ö�Ӧ����ΪUART_5
        switch(UART5_GROUP)
        {
        case 0:
            //ʹ��UART5��GPIOC��GPIODʱ��
            RCC->APB1PCENR |= RCC_UART5EN;
            RCC->APB2PCENR |= RCC_IOPCEN;
            RCC->APB2PCENR |= RCC_IOPDEN;
            //ʹ��PTC12,PTD2ΪUART4(Tx,Rx)����
            GPIOC->CFGHR &= ~(GPIO_CFGHR_MODE12|GPIO_CFGHR_CNF12);
            GPIOD->CFGLR &= ~(GPIO_CFGLR_MODE2|GPIO_CFGLR_CNF2);
            GPIOC->CFGHR |= ((GPIO_CFGHR_CNF12_1|GPIO_CFGHR_MODE12_1));
            GPIOD->CFGLR |= (GPIO_CFGLR_CNF2_1);
            break;
        case 1:
            //ʹ��UART5��GPIOBʱ��
            RCC->APB1PCENR |= RCC_UART5EN;
            RCC->APB2PCENR |= RCC_IOPBEN;
            RCC->APB2PCENR |= RCC_AFIOEN;
            //ʹ��PTB4,PTB5ΪUSART(Tx,Rx)����
            GPIOB->CFGLR &= ~(GPIO_CFGLR_MODE4|GPIO_CFGLR_MODE5|GPIO_CFGLR_CNF4|GPIO_CFGLR_CNF5);
            GPIOB->CFGLR |= ((GPIO_CFGLR_CNF4_1|GPIO_CFGLR_MODE4_1)|(GPIO_CFGLR_CNF5_1));
            AFIO->PCFR2 |= ((uint32_t)0x00040000);
            break;
        case 2:
            //ʹ��UART5��GPIOEʱ��
            RCC->APB1PCENR |= RCC_UART5EN;
            RCC->APB2PCENR |= RCC_IOPEEN;
            RCC->APB2PCENR |= RCC_AFIOEN;
            //ʹ��PTE8,PTE9ΪUSART(Tx,Rx)����
            GPIOE->CFGHR &= ~(GPIO_CFGHR_MODE8|GPIO_CFGHR_MODE9|GPIO_CFGHR_CNF8|GPIO_CFGHR_CNF9);
            GPIOE->CFGHR |= ((GPIO_CFGHR_CNF8_1|GPIO_CFGHR_MODE8_1)|(GPIO_CFGHR_CNF9_0));
            AFIO->PCFR2 |= ((uint32_t)0x00080000);
            break;
        default:
            break;
        }
#endif
        break;
    case UART_6:  //��Ϊ����6
#ifdef UART6_GROUP
        //����ѡ��ʹ�ܶ�Ӧʱ�ӣ������ö�Ӧ����ΪUART_6
        switch(UART6_GROUP)
        {
        case 0:
            //ʹ��UART6��GPIOCʱ��
            RCC->APB1PCENR |= RCC_UART6EN;
            RCC->APB2PCENR |= RCC_IOPCEN;
            //ʹ��PTC0,PTC1ΪUSART(Tx,Rx)����
            GPIOC->CFGLR &= ~(GPIO_CFGLR_MODE0|GPIO_CFGLR_MODE1|GPIO_CFGLR_CNF0|GPIO_CFGLR_CNF1);
            GPIOC->CFGLR |= ((GPIO_CFGLR_CNF0_1|GPIO_CFGLR_MODE0_1)|(GPIO_CFGLR_CNF1_0));
            break;
        case 1:
            //ʹ��UART6��GPIOBʱ��
            RCC->APB1PCENR |= RCC_UART6EN;
            RCC->APB2PCENR |= RCC_IOPBEN;
            RCC->APB2PCENR |= RCC_AFIOEN;
            //ʹ��PTB8,PTB9ΪUSART(Tx,Rx)����
            GPIOB->CFGHR &= ~(GPIO_CFGHR_MODE8|GPIO_CFGHR_MODE9|GPIO_CFGHR_CNF8|GPIO_CFGHR_CNF9);
            GPIOB->CFGHR |= ((GPIO_CFGHR_CNF8_1|GPIO_CFGHR_MODE8_1)|(GPIO_CFGHR_CNF9_0));
            AFIO->PCFR2 |= ((uint32_t)0x00100000);
            break;
        case 2:
            //ʹ��UART6��GPIOEʱ��
            RCC->APB1PCENR |= RCC_UART6EN;
            RCC->APB2PCENR |= RCC_IOPEEN;
            RCC->APB2PCENR |= RCC_AFIOEN;
            //ʹ��PTE10,PTE11ΪUSART(Tx,Rx)����
            GPIOE->CFGHR &= ~(GPIO_CFGHR_MODE10|GPIO_CFGHR_MODE11|GPIO_CFGHR_CNF10|GPIO_CFGHR_CNF11);
            GPIOE->CFGHR |= ((GPIO_CFGHR_CNF10_1|GPIO_CFGHR_MODE10_1)|(GPIO_CFGHR_CNF11_0));
            AFIO->PCFR2 |= ((uint32_t)0x00200000);
            break;
        default:
            break;
        }
#endif
        break;

    case UART_7:  //��Ϊ����7
#ifdef UART7_GROUP
      //����ѡ��ʹ�ܶ�Ӧʱ�ӣ������ö�Ӧ����ΪUART_7
      switch(UART7_GROUP)
      {
      case 0:
          //ʹ��UART7��GPIOCʱ��
          RCC->APB1PCENR |= RCC_UART7EN;
          RCC->APB2PCENR |= RCC_IOPCEN;
          //ʹ��PTC2,PTC3ΪUSART(Tx,Rx)����
          GPIOC->CFGLR &= ~(GPIO_CFGLR_MODE2|GPIO_CFGLR_MODE3|GPIO_CFGLR_CNF2|GPIO_CFGLR_CNF3);
          GPIOC->CFGLR |= ((GPIO_CFGLR_CNF2_1|GPIO_CFGLR_MODE2_1)|(GPIO_CFGLR_CNF3_0));
          break;
      case 1:
          //ʹ��UART7��GPIOAʱ��
          RCC->APB1PCENR |= RCC_UART7EN;
          RCC->APB2PCENR |= RCC_IOPAEN;
          RCC->APB2PCENR |= RCC_AFIOEN;
          //ʹ��PTA6,PTA7ΪUSART(Tx,Rx)����
          GPIOA->CFGLR &= ~(GPIO_CFGLR_MODE6|GPIO_CFGLR_MODE7|GPIO_CFGLR_CNF6|GPIO_CFGLR_CNF7);
          GPIOA->CFGLR |= ((GPIO_CFGLR_CNF6_1|GPIO_CFGLR_MODE6_1)|(GPIO_CFGLR_CNF7_0));
          AFIO->PCFR2 |= ((uint32_t)0x00400000);
          break;
      case 2:
          //ʹ��UART7��GPIOEʱ��
          RCC->APB1PCENR |= RCC_UART7EN;
          RCC->APB2PCENR |= RCC_IOPEEN;
          RCC->APB2PCENR |= RCC_AFIOEN;
          //ʹ��PTE12,PTE13ΪUSART(Tx,Rx)����
          GPIOE->CFGHR &= ~(GPIO_CFGHR_MODE12|GPIO_CFGHR_MODE13|GPIO_CFGHR_CNF12|GPIO_CFGHR_CNF13);
          GPIOE->CFGHR |= ((GPIO_CFGHR_CNF12_1|GPIO_CFGHR_MODE12_1)|(GPIO_CFGHR_CNF13_0));
          AFIO->PCFR2 |= ((uint32_t)0x00800000);
          break;
      default:
          break;
      }
#endif
      break;

    case UART_8:  //��Ϊ����8
#ifdef UART8_GROUP
        //����ѡ��ʹ�ܶ�Ӧʱ�ӣ������ö�Ӧ����ΪUART_8
        switch(UART8_GROUP)
        {
        case 0:
            //ʹ��UART8��GPIOCʱ��
            RCC->APB1PCENR |= RCC_UART8EN;
            RCC->APB2PCENR |= RCC_IOPCEN;
            //ʹ��PTC4,PTC5ΪUSART(Tx,Rx)����
            GPIOC->CFGLR &= ~(GPIO_CFGLR_MODE4|GPIO_CFGLR_MODE5|GPIO_CFGLR_CNF4|GPIO_CFGLR_CNF5);
            GPIOC->CFGLR |= ((GPIO_CFGLR_CNF4_1|GPIO_CFGLR_MODE4_1)|(GPIO_CFGLR_CNF5_0));
            break;
        case 1:
            //ʹ��UART8��GPIOAʱ��
            RCC->APB1PCENR |= RCC_UART8EN;
            RCC->APB2PCENR |= RCC_IOPAEN;
            RCC->APB2PCENR |= RCC_AFIOEN;
            //ʹ��PTA14,PTA15ΪUSART(Tx,Rx)����
            GPIOA->CFGHR &= ~(GPIO_CFGHR_MODE14|GPIO_CFGHR_MODE15|GPIO_CFGHR_CNF14|GPIO_CFGHR_CNF15);
            GPIOA->CFGHR |= ((GPIO_CFGHR_CNF14_1|GPIO_CFGHR_MODE14_1)|(GPIO_CFGHR_CNF15_0));
            AFIO->PCFR2 |= ((uint32_t)0x01000000);
            break;
        case 2:
            //ʹ��UART8��GPIOEʱ��
            RCC->APB1PCENR |= RCC_UART8EN;
            RCC->APB2PCENR |= RCC_IOPEEN;
            RCC->APB2PCENR |= RCC_AFIOEN;
            //ʹ��PTE14,PTE15ΪUSART(Tx,Rx)����
            GPIOE->CFGHR &= ~(GPIO_CFGHR_MODE14|GPIO_CFGHR_MODE15|GPIO_CFGHR_CNF14|GPIO_CFGHR_CNF15);
            GPIOE->CFGHR |= ((GPIO_CFGHR_CNF14_1|GPIO_CFGHR_MODE14_1)|(GPIO_CFGHR_CNF15_0));
            AFIO->PCFR2 |= ((uint32_t)0x02000000);
            break;
        default:
            break;
        }
#endif
    break;

    }
    //��ʱ����UART����
    USART_ARR[uartNo-1]->CTLR1 &= ~USART_CTLR1_UE;
    //��ʱ�رմ��ڷ�������չ���
    USART_ARR[uartNo-1]->CTLR1 &= ~(USART_CTLR1_TE|USART_CTLR1_RE);
    //���ô��ڲ�����

    if(USART_ARR[uartNo-1]==(USART_TypeDef*)USART1_BASE)
    {
        DIV_M = (uint16_t)(SYSCLK_FREQ_72MHz/(16*baud_rate));
        DIV_F = (uint8_t)(((((((SYSCLK_FREQ_72MHz/16)*100)/baud_rate)%100)*16)+50)/100);
        USART_ARR[uartNo-1]->BRR = (uint16_t)(DIV_M<<4|DIV_F);
    }
    else
    {
        DIV_M = (uint16_t)(SYSCLK_FREQ_72MHz/(16*2*baud_rate));
        DIV_F = (uint8_t)((((((SYSCLK_FREQ_72MHz/2/16)*100)/baud_rate)%100)*16)/100);
        USART_ARR[uartNo-1]->BRR = (uint32_t)(((DIV_M)<<4)|DIV_F);
    }
    //��ʼ�����ƼĴ������ж�״̬�Ĵ��������־λ
    USART_ARR[uartNo-1]->STATR = 0;
    USART_ARR[uartNo-1]->CTLR2 &= ~(USART_CTLR2_LINEN | USART_CTLR2_CLKEN);
    USART_ARR[uartNo-1]->CTLR3 &= ~(USART_CTLR3_SCEN | USART_CTLR3_HDSEL | USART_CTLR3_IREN);
    //�������ڷ�������չ���
    USART_ARR[uartNo-1]->CTLR1 |= (USART_CTLR1_TE|USART_CTLR1_RE);

    //����UART����
    USART_ARR[uartNo-1]->CTLR1 |= USART_CTLR1_UE;

}

//======================================================================
//�������ƣ�uart_send1
//����˵����uartNo: ���ں�:UART_1��UART_2��UART_3
//          ch:Ҫ���͵��ֽ�
//�������أ�����ִ��״̬��1=���ͳɹ���0=����ʧ�ܡ�
//���ܸ�Ҫ�����з���1���ֽ�
//======================================================================
uint8_t uart_send1(uint8_t uartNo, uint8_t ch)
{
    uint32_t t;

    //�жϴ��ںŲ����Ƿ���������ֱ���˳�
    if(!uart_is_uartNo(uartNo))
    {
        return 0;
    }
    for(t = 0; t< 0xFBBB; t++)
    {
        //���ͻ�����Ϊ����������
        if(USART_ARR[uartNo-1]->STATR & USART_STATR_TXE)
        {
            USART_ARR[uartNo-1]->DATAR = (ch & USART_DATAR_DR);
            break;
        }
    }
    if(t >= 0xFBBB)
    {
        return 0; //���ͳ�ʱ������ʧ��

    }
    else
        return 1;
}

//======================================================================
//�������ƣ�uart_sendN
//����˵����uartNo: ���ں�:UART_1��UART_2��UART_3
//         buff: ���ͻ�����
//         len:���ͳ���
//�������أ� ����ִ��״̬��1=���ͳɹ���0=����ʧ��
//���ܸ�Ҫ������ ����n���ֽ�
//======================================================================
uint8_t uart_sendN(uint8_t uartNo ,uint16_t len ,uint8_t* buff)
{
    uint16_t i;

    //�жϴ��ںŲ����Ƿ���������ֱ���˳�
    if(!uart_is_uartNo(uartNo))
    {
        return 0;
    }
    for(i=0;i<len;i++)
    {
        if(!uart_send1(uartNo, buff[i]))   //����һ���ֽ����ݣ�ʧ��������ѭ��
        {
            break;
        }
    }
    if(i<len)
    {
        return 0;   //���ͳ���
    }
    else
        return 1;  //���ͳɹ�

}

//======================================================================
//�������ƣ�uart_send_string
//����˵����uartNo:UARTģ���:UART_1��UART_2��UART_3
//          buff:Ҫ���͵��ַ������׵�ַ
//�������أ� ����ִ��״̬��1=���ͳɹ���0=����ʧ��
//���ܸ�Ҫ����ָ��UART�˿ڷ���һ����'\0'�������ַ���
//======================================================================
uint8_t uart_send_string(uint8_t uartNo, uint8_t* buff)
{
    uint16_t i = 0;
    uint8_t *buff_ptr = (uint8_t *)buff;     //����ָ��ָ��Ҫ�����ַ����׵�ַ

    //�жϴ��봮�ںŲ����Ƿ���������ֱ���˳�
    if(!uart_is_uartNo(uartNo))
    {
        return 0;
    }

    for(i = 0; buff_ptr[i] != '\0'; i++)   //�����ַ�������ַ�
    {
        if (!uart_send1(uartNo,buff_ptr[i]))//����ָ���Ӧ���ַ�
            return 0;  //����ʧ��,����
    }
    return 1;          //���ͳɹ�
}

//======================================================================
//�������ƣ�uart_re1
//����˵����uartNo: ���ں�:UART_1��UART_2��UART_3
//        *fp:���ճɹ���־��ָ��:*fp=1:���ճɹ���*fp=0:����ʧ��
//�������أ����շ����ֽ�
//���ܸ�Ҫ�����н���1���ֽ�
//======================================================================
uint8_t uart_re1(uint8_t uartNo,uint8_t *fp)
{
    uint32_t t;
    uint8_t dat;

    //�жϴ��봮�ںŲ����Ƿ���������ֱ���˳�
    if(!uart_is_uartNo(uartNo))
    {
        *fp = 0;
        return 0;
    }

    for(t=0; t< 0xFBBB; t++)
    {
        //�жϻ������Ƿ���
        if(USART_ARR[uartNo-1]->STATR & USART_STATR_RXNE)
        {
            dat = (USART_ARR[uartNo-1]->DATAR & USART_DATAR_DR); // ��ȡ���ݣ�������ж�λ
            *fp = 1; //���ճɹ�
            break;
        }
    }//����
    if(t>=0xFBBB)
    {
        dat = 0xFF;
        *fp = 0;
    }

    return dat;

}

//======================================================================
//�������ƣ�uart_reN
//����˵����uartNo: ���ں�:UART_1��UART_2��UART_3
//          buff: ���ջ�����
//          len:���ճ���
//�������أ�����ִ��״̬ 1=���ճɹ�;0=����ʧ��
//���ܸ�Ҫ������ ����n���ֽ�,����buff��
//======================================================================
uint8_t uart_reN(uint8_t uartNo ,uint16_t len ,uint8_t* buff)
{
    uint16_t i;
    uint8_t flag = 0;

    //�жϴ��봮�ںŲ����Ƿ���������ֱ���˳�
    if(!uart_is_uartNo(uartNo))
    {
        return 0;
    }
    //�ж��Ƿ��ܽ�������
    for(i=0;i<len && flag == 1;i++)
    {
        buff[i] = uart_re1(uartNo, &flag);  //��������

    }
    if(i<len)
    {
        return 0;
    }
    else
        return 1;

}

//======================================================================
//�������ƣ�uart_enable_re_int
//����˵����uartNo: ���ں�:UART_1��UART_2��UART_3
//�������أ���
//���ܸ�Ҫ�������ڽ����ж�
//======================================================================
void uart_enable_re_int(uint8_t uartNo)
{
    //�жϴ��봮�ںŲ����Ƿ���������ֱ���˳�
    if(!uart_is_uartNo(uartNo))
    {
        return;
    }

    USART_ARR[uartNo-1]->CTLR1 |= USART_CTLR1_RXNEIE;    //����UART�����ж�
//    USART_ARR[uartNo-1]->CTLR1 |= USART_CTLR1_TXEIE;
//    USART_ARR[uartNo-1]->CTLR1 |= ((uint16_t)0x0525);    //����UART�����ж�
    NVIC_EnableIRQ(table_irq_uart[uartNo-1]);            //���жϿ�����IRQ�ж�
    //USART_ARR[uartNo-1]->CTLR1 |= USART_CTLR1_RXNEIE;    //����UART�����ж�
}

//======================================================================
//�������ƣ�uart_disable_re_int
//����˵����uartNo: ���ں� :UART_1��UART_2��UART_3
//�������أ���
//���ܸ�Ҫ���ش��ڽ����ж�
//======================================================================
void uart_disable_re_int(uint8_t uartNo)
{
    //�жϴ��봮�ںŲ����Ƿ���������ֱ���˳�
    if(!uart_is_uartNo(uartNo))
    {
        return;
    }

    USART_ARR[uartNo-1]->CTLR1 &= ~USART_CTLR1_RXNEIE;    //��ֹUART�����ж�
    NVIC_DisableIRQ(table_irq_uart[uartNo-1]);    //��ֹ�жϿ�����IRQ�ж�
}

//======================================================================
//�������ƣ�uart_get_re_int
//����˵����uartNo: ���ں� :UART_1��UART_2��UART_3
//�������أ������жϱ�־ 1=�н����ж�;0=�޽����ж�
//���ܸ�Ҫ����ȡ���ڽ����жϱ�־,ͬʱ���÷����ж�
//======================================================================
uint8_t uart_get_re_int(uint8_t uartNo)
{
    //�жϴ��봮�ںŲ����Ƿ���������ֱ���˳�
    if(!uart_is_uartNo(uartNo))
    {
        return 0;
    }

    //��ȡ�����жϱ�־����ͬʱ�ж�RXNE��RXNEIE
    if(((USART_ARR[uartNo-1]->STATR & USART_STATR_RXNE) == USART_STATR_RXNE)
       && ((USART_ARR[uartNo-1]->CTLR1 & USART_CTLR1_RXNEIE) == USART_CTLR1_RXNEIE))
        return 1;
    else
        return 0;
}


//======================================================================
//�������ƣ�uart_deinit
//����˵����uartNo: ���ں� :UART_1��UART_2��UART_3
//�������أ���
//���ܸ�Ҫ��uart����ʼ�����رմ���ʱ��
//======================================================================
void uart_deinit(uint8_t uartNo)
{
    //�жϴ��봮�ںŲ����Ƿ�����,����ֱ���˳�
    if(!uart_is_uartNo(uartNo))
    {
        return;
    }

    //����UARTģ���GPIOģ�����Χʱ�ӣ���ʹ�����ŵ�UART����
    switch(uartNo)
    {
    case UART_1:  //��Ϊ����1
#ifdef UART1_GROUP
        //����ѡ��ʹ�ܶ�Ӧʱ�ӣ������ö�Ӧ����ΪUART_1
        switch(UART1_GROUP)
        {
        case 0:
            //ʹ��USART1��GPIOAʱ��
            RCC->APB2PCENR &= ~RCC_USART1EN;
            RCC->APB2PCENR &= ~RCC_IOPAEN;
            //ʹ��PTA9,PTA10ΪUSART(Tx,Rx)����
            GPIOA->CFGHR &= ~(GPIO_CFGHR_MODE9|GPIO_CFGHR_MODE10);
//            AFIO->PCFR1 &= ~(AFIO_PCFR1_USART1_REMAP);
//            AFIO->PCFR1 |= (AFIO_PCFR1_USART1_REMAP);
            break;
        case 1:
            //ʹ��USART1��GPIOBʱ��
            RCC->APB2PCENR &= ~RCC_USART1EN;
            RCC->APB2PCENR &= ~RCC_AFIOEN;
            //ʹ��PTB6,PTB7ΪUSART(Tx,Rx)����
            GPIOB->CFGLR &= ~(GPIO_CFGLR_CNF6_1|GPIO_CFGLR_CNF7_1);
            AFIO->PCFR1 &= ~AFIO_PCFR1_USART1_REMAP;
            break;
        default:
            break;
        }
#endif
        break;

    case UART_2:  //��Ϊ����2
#ifdef UART2_GROUP
        //����ѡ��ʹ�ܶ�Ӧʱ�ӣ������ö�Ӧ����ΪUART_2
        switch(UART2_GROUP)
        {
        case 0:
            //ʹ��USART2��GPIOAʱ��
            RCC->APB1PCENR &= ~RCC_USART2EN;
            RCC->APB2PCENR &= ~RCC_IOPAEN;
            //ʹ��PTA9,PTA10ΪUSART(Tx,Rx)����
            GPIOA->CFGLR &= ~(GPIO_CFGLR_CNF3|GPIO_CFGLR_CNF2);

            break;
        case 1:
            //ʹ��USART1��GPIOBʱ��
            RCC->APB2PCENR &= ~RCC_USART2EN;
            RCC->APB2PCENR &= ~RCC_IOPDEN;
            //ʹ��PTB6,PTB7ΪUSART(Tx,Rx)����
            GPIOD->CFGLR &= ~(GPIO_CFGLR_CNF5_1|GPIO_CFGLR_CNF6_1);
            AFIO->PCFR1 &= ~AFIO_PCFR1_USART2_REMAP;

            break;
        default:
            break;
        }
#endif
        break;

    case UART_3:  //��Ϊ����3
#ifdef UART3_GROUP
        //����ѡ��ʹ�ܶ�Ӧʱ�ӣ������ö�Ӧ����ΪUART_3
        switch(UART3_GROUP)
        {
        case 0:
            //ʹ��USART2��GPIOAʱ��
            RCC->APB1PCENR &= ~RCC_USART3EN;
            RCC->APB2PCENR &= ~RCC_IOPBEN;
            //ʹ��PTA9,PTA10ΪUSART(Tx,Rx)����
            GPIOB->CFGHR &= ~(GPIO_CFGHR_CNF10|GPIO_CFGHR_CNF11);
            //GPIOB->CFGHR |= ((GPIO_CFGHR_CNF10_1|GPIO_CFGHR_MODE10_1)|(GPIO_CFGHR_CNF11_0));
            break;
        case 1:
            //ʹ��USART1��GPIOBʱ��
            RCC->APB2PCENR &= ~RCC_USART3EN;
            RCC->APB2PCENR &= ~RCC_AFIOEN;
            //ʹ��PTB6,PTB7ΪUSART(Tx,Rx)����
            AFIO->PCFR1 &= ~AFIO_PCFR1_USART3_REMAP;
            GPIOD->CFGHR &= ~(GPIO_CFGHR_CNF8|GPIO_CFGHR_CNF9);
            //GPIOD->CFGHR |= ((GPIO_CFGHR_CNF8_1|GPIO_CFGHR_MODE8_1)|(GPIO_CFGHR_CNF9_0));

            break;
        default:
            break;
        }
#endif
        break;
    }

}


//----------------------����Ϊ�ڲ�������Ŵ�--------------------------------------
//=====================================================================
//�������ƣ�uart_is_uartNo
//�������أ�1:���ں��ں���Χ�ڣ�0�����ںŲ�����
//����˵�������ں�uartNo  :UART_1��UART_2��UART_3
//���ܸ�Ҫ��Ϊ����׳�Զ��ж�uartNo�Ƿ��ڴ������ַ�Χ��
//=====================================================================
uint8_t uart_is_uartNo(uint8_t uartNo)
{
    if(uartNo < UART_1 || uartNo > UART_8)
        return 0;
    else
        return 1;
}
