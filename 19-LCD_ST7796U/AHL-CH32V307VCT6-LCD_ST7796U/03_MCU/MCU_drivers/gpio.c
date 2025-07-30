//===========================================================================
//�ļ����ƣ�gpio.c
//���ܸ�Ҫ��GPIO�ײ���������Դ�ļ�
//��Ȩ���У�SD-Arm(sumcu.suda.edu.cn)
//�汾���£�20200831-20211027
//оƬ���ͣ�CH32V307
//===========================================================================

#include "gpio.h"

//GPIO�ڻ���ַ���볣��������GPIO_ARR[0]~GPIO_ARR[5]��
GPIO_TypeDef * GPIO_ARR[] ={(GPIO_TypeDef *)GPIOA_BASE,(GPIO_TypeDef *)GPIOB_BASE,
                            (GPIO_TypeDef *)GPIOC_BASE,(GPIO_TypeDef *)GPIOD_BASE,(GPIO_TypeDef *)GPIOE_BASE};

//==============������չ�ж�IRQ�Ŷ�Ӧ��================
IRQn_Type table_irq_exti[7] = {EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn,
                               EXTI4_IRQn, EXTI9_5_IRQn, EXTI15_10_IRQn};



//�ڲ���������
void gpio_get_port_pin(uint16_t port_pin,uint8_t* port,uint8_t* pin);
void GPIO_EXTILineConfig(uint8_t port, uint8_t pin);

//=====================================================================
//�������ƣ�gpio_init
//�������أ���
//����˵����port_pin��(�˿ں�)|(���ź�)���磺(PTB_NUM)|(9) ��ʾΪB��9�Žţ�
//         dir�����ŷ���0=���룬1=���,�������ŷ���궨�壩
//         state���˿����ų�ʼ״̬��0=�͵�ƽ��1=�ߵ�ƽ��
//���ܸ�Ҫ����ʼ��ָ���˿�������ΪGPIO���Ź��ܣ�������Ϊ�������������������
//         ��ָ����ʼ״̬�ǵ͵�ƽ��ߵ�ƽ
//=====================================================================
void gpio_init(uint16_t port_pin, uint8_t dir, uint8_t state)
{
    GPIO_TypeDef *gpio_ptr;    //����gpio_ptrΪGPIO�ṹ������ָ��
    uint8_t port,pin;    //�����˿�port������pin����
    uint32_t temp;       //��ʱ��żĴ������ֵ

    //���ݴ������port_pin���������˿������ŷֱ𸳸�port,pin
    gpio_get_port_pin(port_pin,&port,&pin);
    //����port�����ֲ�����gpio_ptr��ֵ��GPIO����ַ��
    gpio_ptr = GPIO_ARR[port];

    //ʹ����ӦGPIOʱ��
    RCC->APB2PCENR |= (RCC_IOPAEN << (port * 1u));

    if(pin <= 0x07)
    {
        temp = gpio_ptr->CFGLR;
        temp &= ~(GPIO_CFGLR_CNF0 << (pin * 4u));

        if(dir == 1)   // ����Ϊ�������
        {
            temp &= ~(GPIO_CFGLR_MODE0 << (pin * 4u));
//            temp |= (GPIO_CFGLR_MODE0 << (pin * 4u));
            temp |= (GPIO_OUTPUT << (pin * 4u));
            gpio_ptr->CFGLR = temp;
            gpio_set(port_pin,state);
        }
        else           // ����Ϊ��������
        {
            temp &= ~(GPIO_CFGLR_MODE0 << (pin * 4u));
            temp |= (GPIO_INPUT << (pin * 4u));
            gpio_ptr->CFGLR = temp;
        }
    }
    if(pin > 0x07)
    {
        //��GPIOģʽ�Ĵ�����Ӧ����λ
        temp = gpio_ptr->CFGHR;
        temp &= ~(GPIO_CFGHR_CNF8 << (pin * 4u));

        if(dir == 1)   // ����Ϊ�������
        {
            temp &= ~(GPIO_CFGLR_MODE0 << ((pin-8) * 4u));
//            temp |= (GPIO_CFGLR_MODE0 << ((pin-8) * 4u));
            temp |= (GPIO_OUTPUT << ((pin-8) * 4u));
            gpio_ptr->CFGHR = temp;
            gpio_set(port_pin,state);
        }
        else           // ����Ϊ��������
        {
            temp &= ~(GPIO_CFGLR_MODE0 << ((pin-8) * 4u));
            temp |= (GPIO_INPUT << ((pin-8) * 4u));
            gpio_ptr->CFGHR = temp;
        }

    }
}

void gpio_init_MOD(uint16_t port_pin, uint8_t dir, uint8_t state)
{
    GPIO_TypeDef *gpio_ptr;    //����gpio_ptrΪGPIO�ṹ������ָ��
    uint8_t port,pin;    //�����˿�port������pin����
    uint32_t temp;       //��ʱ��żĴ������ֵ

    //���ݴ������port_pin���������˿������ŷֱ𸳸�port,pin
    gpio_get_port_pin(port_pin,&port,&pin);
    //����port�����ֲ�����gpio_ptr��ֵ��GPIO����ַ��
    gpio_ptr = GPIO_ARR[port];

    //ʹ����ӦGPIOʱ��
    RCC->APB2PCENR |= (RCC_IOPAEN << (port * 1u));

    if(pin <= 0x07)
    {
        temp = gpio_ptr->CFGLR;
        temp &= ~( (GPIO_CFGLR_CNF0 << (pin * 4u)) | (GPIO_CFGLR_MODE0 << (pin * 4u)) );

        if(dir == 1)   // ����Ϊ�������
        {
            temp |= (0xF << (pin * 4u));
            gpio_ptr->CFGLR = temp;
            gpio_set(port_pin,state);
        }
        else           // ����Ϊ��������
        {
            temp |= (GPIO_INPUT << (pin * 4u));
            gpio_ptr->CFGLR = temp;
        }
    }
    if(pin > 0x07)
    {
        //��GPIOģʽ�Ĵ�����Ӧ����λ
        temp = gpio_ptr->CFGHR;
        temp &= ~( (GPIO_CFGHR_CNF8 << (pin * 4u)) | (GPIO_CFGHR_MODE8 << (pin * 4u)) );

        if(dir == 1)   // ����Ϊ�������
        {
            temp |= (3 << ((pin-8) * 4u));
            gpio_ptr->CFGHR = temp;
            gpio_set(port_pin,state);
        }
        else           // ����Ϊ��������
        {
            temp |= (GPIO_INPUT << ((pin-8) * 4u));
            gpio_ptr->CFGHR = temp;
        }

    }
}
//=====================================================================
//�������ƣ�gpio_set
//�������أ���
//����˵����port_pin���˿ں�|���źţ��磺(PTB_NUM)|(9) ��ʾΪB��9�Žţ�
//       state�����ų�ʼ״̬��0=�͵�ƽ��1=�ߵ�ƽ��
//���ܸ�Ҫ���趨����״̬Ϊ�͵�ƽ��ߵ�ƽ
//=====================================================================
void gpio_set(uint16_t port_pin, uint8_t state)
{
    //�ֲ���������
    GPIO_TypeDef *gpio_ptr;                      //����port_ptrΪGPIO�ṹ������ָ�루�׵�ַ��
    uint8_t port,pin;                          //�����˿�port������pin����
    //���ݴ������port_pin���������˿������ŷֱ𸳸�port,pin
    gpio_get_port_pin(port_pin,&port,&pin);

    //����port�����ֲ�����gpio_ptr��ֵ��GPIO����ַ��
    gpio_ptr = GPIO_ARR[port];

    //���ݴ������state����������Ϊ���1����0
    if(1==state)
    {
        gpio_ptr->BSHR = (uint16_t)(1u << pin);
    }
    else
    {
        gpio_ptr->BCR = (uint16_t)(1u << pin);
    }

}

//=====================================================================
//�������ƣ�gpio_get
//�������أ�ָ�����ŵ�״̬��1��0��
//����˵����port_pin���˿ں�|���źţ��磺(PTB_NUM)|(9) ��ʾΪB��9�Žţ�
//���ܸ�Ҫ����ȡָ�����ŵ�״̬��1��0��
//=====================================================================
uint8_t gpio_get(uint16_t port_pin)
{
    //�ֲ���������
    GPIO_TypeDef *gpio_ptr;              //����port_ptrΪGPIO�ṹ������ָ�루�׵�ַ��
    uint8_t port,pin;                    //�����˿�port������pin����
    uint32_t temp;
    uint8_t value;

    //���ݴ������port_pin���������˿������ŷֱ𸳸�port,pin
    gpio_get_port_pin(port_pin,&port,&pin);

    //����port�����ֲ�����gpio_ptr��ֵ��GPIO����ַ��
    gpio_ptr = GPIO_ARR[port];

    // ���״̬�Ĵ�����ֵ
    if(pin < 0x08)
    {
        temp =gpio_ptr->CFGLR;
        if((temp & (3u<<(pin*4)))==0x00u)   // GPIO����
        {
            //��ȡInPut data�Ĵ�����Ӧ���ŵ�ֵ
            temp = gpio_ptr->INDR;
            if((temp & (1u<<pin)) != 0x00u)
                value = 1;
            else
                value = 0;
        }
        else
        {
            //��ȡOutPut data�Ĵ�����Ӧ���ŵ�ֵ
            temp = gpio_ptr->OUTDR;
            if((temp & (1u<<pin) )!= 0x00u)
                value = 1;
            else
                value = 0;
        }
    }
    else
    {
        temp = gpio_ptr->CFGHR;
        if((temp & (3u<<((pin-8)*4)))==0x00u)   // GPIO����
        {
            //��ȡInPut data�Ĵ�����Ӧ���ŵ�ֵ
            temp = gpio_ptr->INDR;
            if((temp & (1u<<pin)) != 0x00u)
                value = 1;
            else
                value = 0;
        }
        else
        {
            //��ȡOutPut data�Ĵ�����Ӧ���ŵ�ֵ
            temp = gpio_ptr->OUTDR;
            if((temp & (1u<<pin)) != 0x00u)
                value = 1;
            else
                value = 0;
        }
    }
    return value;
}

//=====================================================================
//�������ƣ�gpio_reverse
//�������أ���
//����˵����port_pin���˿ں�|���źţ��磺(PTB_NUM)|(9) ��ʾΪB��9�Žţ�
//���ܸ�Ҫ����ָ���˿����ű�����ΪGPIO������Ϊ���ʱ����תָ���������״̬��
//=====================================================================
void gpio_reverse(uint16_t port_pin)
{
    //�ֲ���������
    GPIO_TypeDef *gpio_ptr;              //����port_ptrΪGPIO�ṹ������ָ�루�׵�ַ��
    uint8_t port,pin;                    //�����˿�port������pin����

    //���ݴ������port_pin���������˿������ŷֱ𸳸�port,pin
    gpio_get_port_pin(port_pin,&port,&pin);

    //����port�����ֲ�����gpio_ptr��ֵ��GPIO����ַ��
    gpio_ptr = GPIO_ARR[port];

    //�ж�����״̬
    if((gpio_ptr->OUTDR & (1u<<pin)) != 0x00)
    {
        //�ߵ�ƽ��תΪ�͵�ƽ
        gpio_ptr->BCR = (uint32_t)(1u<<pin);
    }
    else
    {
        // �͵�ƽ��תΪ�ߵ�ƽ
        gpio_ptr->BSHR = (uint32_t)(1u<<pin);
    }
}

//=====================================================================
//�������ƣ�gpio_pull
//�������أ���
//����˵����port_pin���˿ں�|���źţ��磺(PTB_NUM)|(9) ��ʾΪB��9�Žţ�
//        pullselect���������ߵ͵�ƽ�� 0=���͵�ƽ��1=���ߵ�ƽ��
//���ܸ�Ҫ��ʹָ�����������ߵ�ƽ�������͵�ƽ
//=====================================================================
void gpio_pull(uint16_t port_pin, uint8_t pullselect)
{
    //�ֲ���������
    GPIO_TypeDef *gpio_ptr;              //����port_ptrΪGPIO�ṹ������ָ�루�׵�ַ��
    uint8_t port,pin;                    //�����˿�port������pin����

    //���ݴ������port_pin���������˿������ŷֱ𸳸�port,pin
    gpio_get_port_pin(port_pin,&port,&pin);

    //����port�����ֲ�����gpio_ptr��ֵ��GPIO����ַ��
    gpio_ptr = GPIO_ARR[port];

    if(pin<=0x07)
    {
        //�趨Ϊ�ٶ�10MHz
        gpio_ptr->CFGLR |= (GPIO_CFGLR_MODE0 <<(4u * pin));

        if(1 == pullselect)
        {
            //��������
            gpio_ptr->OUTDR = (uint32_t)(PULL_UP<<(1u*pin));
        }
        else
        {
            //��������
            gpio_ptr->OUTDR = (uint32_t)(PULL_DOWN<<(1u*pin));
        }
    }
    if(pin>0x07)
    {
        //�趨Ϊ�ٶ�10MHz
        gpio_ptr->CFGHR |= (GPIOSpeed_10MHz <<(4u * pin));

        if(1 == pullselect)
        {
            //��������
            gpio_ptr->OUTDR = (uint32_t)(PULL_UP<<(1u*pin));
        }
        else
        {
            //��������
            gpio_ptr->OUTDR = (uint32_t)(PULL_DOWN<<(1u*pin));

        }
    }
}

//=====================================================================
//�������ƣ�gpio_disable_int
//�������أ���
//����˵����port_pin��(�˿ں�)|(���ź�)���磺(PTB_NUM)|(9) ��ʾΪB��9�Žţ�
//         irqtype:�����ж����ͣ��ɺ궨��������ٴ��о����£�
//���ܸ�Ҫ����ָ���˿����ű�����ΪGPIO������Ϊ����ʱ���������ر������ж�
//ע          �⣺ KL25оƬ��ֻ��PORTA��PORTD�ھ���GPIO���жϹ���
//           KW01оƬ��ֻ��PORTA��PORTC��PORTD�ھ���GPIO���жϹ���
//           KL36оƬ��ֻ��PORTA��PORTC��PORTD�ھ���GPIO���жϹ���
//           CH32v103��PORTA��PORTB��PORTC��PORTD�ھ���GPIO���жϹ���
//=====================================================================
void gpio_enable_int(uint16_t port_pin, uint8_t irqtype)
{
    //�ֲ���������
    uint8_t port,pin;                    //�����˿�port������pin����
    uint32_t temp = 0;                       //��ʱ��żĴ�����ֵ
    //���ݴ������port_pin���������˿������ŷֱ𸳸�port,pin
    gpio_get_port_pin(port_pin,&port,&pin);

    //ʹ��AFIOʱ��
    RCC->APB2PCENR|=RCC_AFIOEN;

    //���ö�Ӧ���ŵ���չ�ж�
    GPIO_EXTILineConfig(port,pin);
//
    temp = (uint32_t)EXTI_BASE;
//    //����
    EXTI->INTENR &= ~(EXTI_INTENR_MR0 << pin);
    EXTI->EVENR &= ~(EXTI_EVENR_MR0 << pin);
    EXTI->RTENR &= ~(EXTI_RTENR_TR0 << pin);
    EXTI->FTENR &= ~(EXTI_FTENR_TR0 << pin);
//
//    //���õ�ǰ�Ǵ����ж�
    temp += EXTI_Mode_Interrupt;
//    //�����Ӧ��չ�ж��е�����
    *(__IO uint32_t *) temp |= (EXTI_INTENR_MR0 << pin);
//
    //����˫����
    if(DOUBLE_EDGE == irqtype)
    {
        EXTI->RTENR |= (EXTI_RTENR_TR0 << pin);
        EXTI->FTENR |= (EXTI_FTENR_TR0 << pin);
    }
//    //����
    else
    {
        temp = (uint32_t)EXTI_BASE;
        temp += irqtype;
        *(__IO uint32_t *) temp |= (EXTI_FTENR_TR0 << pin);
    }

//    if(pin == 12)
//        NVIC_EnableIRQ(table_irq_exti[0]);
    if(pin>=0 && pin<=4)
        // ���жϿ�����IRQ�ж�(EXTIx,x=0-4)
        NVIC_EnableIRQ(table_irq_exti[pin]);
    else if(pin>=5 && pin<=9)
        // ���жϿ�����IRQ�ж�(EXTI9-5)
        NVIC_EnableIRQ(table_irq_exti[5]);
    else
        // ���жϿ�����IRQ�ж�(EXTI15-10)
        NVIC_EnableIRQ(table_irq_exti[6]);

}

//=====================================================================
//�������ƣ�gpio_disable_int
//�������أ���
//����˵����port_pin��(�˿ں�)|(���ź�)���磺(PTB_NUM)|(9) ��ʾΪB��9�Žţ�
//���ܸ�Ҫ����ָ���˿����ű�����ΪGPIO������Ϊ����ʱ���������ر������ж�
//ע          �⣺ KL25оƬ��ֻ��PORTA��PORTD�ھ���GPIO���жϹ���
//           KW01оƬ��ֻ��PORTA��PORTC��PORTD�ھ���GPIO���жϹ���
//           KL36оƬ��ֻ��PORTA��PORTC��PORTD�ھ���GPIO���жϹ���
//=====================================================================
void gpio_disable_int(uint16_t port_pin)
{

    //�ֲ���������
    uint8_t port,pin;                    //�����˿�port������pin����
    //uint32_t temp;                       //��ʱ��żĴ�����ֵ
    //���ݴ������port_pin���������˿������ŷֱ𸳸�port,pin
    gpio_get_port_pin(port_pin,&port,&pin);

    //��ֹAFIOʱ��
    RCC->APB2PCENR &=~RCC_AFIOEN;

    //���Ӧ���ŵ���չ�ж�
    AFIO->EXTICR[pin >> 2u] &= ~(0x0FuL << (4u * (pin & 0x03u)));

    //���ζ�Ӧ��չ�ж��е�����
    EXTI->INTENR &= ~(EXTI_INTENR_MR0<<pin);

    //��ֹ�������½��ش���
    EXTI->RTENR &= ~(EXTI_RTENR_TR0 << pin);
    EXTI->FTENR &= ~(EXTI_FTENR_TR0 << pin);

    if(pin>=0 && pin<=4)
        // ���жϿ�����IRQ�ж�(EXTIx,x=0-4)
        NVIC_DisableIRQ(table_irq_exti[pin]);
    else if(pin>=5 && pin<=9)
        // ���жϿ�����IRQ�ж�(EXTI9-5)
        NVIC_DisableIRQ(table_irq_exti[5]);
    else
        // ���жϿ�����IRQ�ж�(EXTI15-10)
        NVIC_DisableIRQ(table_irq_exti[6]);

}

//=====================================================================
//�������ƣ�gpio_drive_strength
//�������أ���
//����˵����port_pin��(�˿ں�)|(���ź�)���磺(PTB_NUM)|(9) ��ʾΪB��9�Žţ�
//       control���������ŵ�����������1=��������0=��������
//         �����ű�����Ϊ�������ʱ�����ſ��ƼĴ�����DSE=1���������� DSE=0������������
//���ܸ�Ҫ������������������ָ�����������������ĳ�������һ����mA��λ����
//            ��������������5mA,����������18mA���������ű�����Ϊ�������ʱ,
//            �����ŵ����������������ã�ֻ��PTB0,PTB1,PTD6,PTD7ͬʱ���и���
//            ��������������������,��Щ���ſ�����ֱ������LED���MOSFET������
//            �볡Ч����ܣ����磬�ú���ֻ����������4�����š�
//=====================================================================
void gpio_drive_strength(uint16_t port_pin, uint8_t control)
{
   //�ֲ���������
   GPIO_TypeDef *gpio_ptr;              //����port_ptrΪGPIO�ṹ������ָ�루�׵�ַ��
   uint8_t port,pin;                    //�����˿�port������pin����

   //���ݴ������port_pin���������˿������ŷֱ𸳸�port,pin
   gpio_get_port_pin(port_pin,&port,&pin);

   //����port�����ֲ�����gpio_ptr��ֵ��GPIO����ַ��
   gpio_ptr = GPIO_ARR[port];

   //�������ŵ���������
   if(pin <= 0x07)
   {
       gpio_ptr->CFGLR |= (uint32_t)(control<<(pin * 4u));
   }
   if(pin > 0x07)
   {
       gpio_ptr->CFGHR |= (uint32_t)(control<<(pin * 4u));
   }

}

//=====================================================================
//�������ƣ�gpio_get_int
//�������أ�����GPIO�жϱ�־��1��0��1��ʾ������GPIO�жϣ�0��ʾ����û��GPIO�жϡ�
//����˵����port_pin��(�˿ں�)|(���ź�)���磺(PTB_NUM)|(9) ��ʾΪB��9�Žţ�
//���ܸ�Ҫ����ָ���˿����ű�����ΪGPIO������Ϊ����ʱ,��ȡ�жϱ�־��
//ע          �⣺ KL25оƬ��ֻ��PORTA��PORTD�ھ���GPIO���жϹ���
//           KW01оƬ��ֻ��PORTA��PORTC��PORTD�ھ���GPIO���жϹ���
//           KL36оƬ��ֻ��PORTA��PORTC��PORTD�ھ���GPIO���жϹ���
//           CH32оƬ��ֻ��GPIOA��GPIOB��GPIOC��GPIOD�ھ���GPIO���жϹ���
//=====================================================================
uint8_t gpio_get_int(uint16_t port_pin)
{
    uint8_t port,pin;           //�����˿�port������pin����

    //���ݴ������port_pin���������˿������ŷֱ𸳸�port,pin
    gpio_get_port_pin(port_pin,&port,&pin);

    // ��ȡGPIO�жϱ�־
    if((EXTI->INTFR & (1u << pin)) == (1u << pin))
    {
        return 1;
    }
    else
        return 0;
}

//=====================================================================
//�������ƣ�gpio_clear_int
//�������أ���
//����˵����port_pin��(�˿ں�)|(���ź�)���磺(PTB_NUM)|(9) ��ʾΪB��9�Žţ�
//���ܸ�Ҫ����ָ���˿����ű�����ΪGPIO������Ϊ����ʱ,����жϱ�־��
//ע          �⣺ KL25оƬ��ֻ��PORTA��PORTD�ھ���GPIO���жϹ���
//           KW01оƬ��ֻ��PORTA��PORTC��PORTD�ھ���GPIO���жϹ���
//           KL36оƬ��ֻ��PORTA��PORTC��PORTD�ھ���GPIO���жϹ���
//           CH32оƬ��ֻ��GPIOA��GPIOB��GPIOC��GPIOD�ھ���GPIO���жϹ���
//=====================================================================
void gpio_clear_int(uint16_t port_pin)
{
    uint8_t port,pin;           //�����˿�port������pin����

   //���ݴ������port_pin���������˿������ŷֱ𸳸�port,pin
   gpio_get_port_pin(port_pin,&port,&pin);

   // ��ȡGPIO�жϱ�־
   EXTI->INTFR |= (1u << pin);

}

//=====================================================================
//�������ƣ�gpio_clear_allint
//�������أ���
//����˵������
//���ܸ�Ҫ��������ж˿ڵ�GPIO�ж�
//ע          �⣺ KL25оƬ��ֻ��PORTA��PORTD�ھ���GPIO���жϹ���
//           KW01оƬ��ֻ��PORTA��PORTC��PORTD�ھ���GPIO���жϹ���
//           KL36оƬ��ֻ��PORTA��PORTC��PORTD�ھ���GPIO���жϹ���
//=====================================================================
void gpio_clear_allint(void)
{
    EXTI->INTFR |= 0xFFFF;  // pin��ȡ0-15

}


//----------------------����Ϊ�ڲ�������Ŵ�-----------------
//=====================================================================
//�������ƣ�gpio_get_port_pin
//�������أ���
//����˵����port_pin���˿ں�|���źţ��磺(PTB_NUM)|(9) ��ʾΪB��9�Žţ�
//       port���˿ںţ���ָ����������
//       pin:���źţ�0~15��ʵ��ȡֵ��оƬ���������ž���������ָ����������
//���ܸ�Ҫ������������port_pin���н������ó�����˿ں������źţ��ֱ�ֵ��port��pin,���ء�
//       ������(PTB_NUM)|(9)����ΪPORTB��9��������ֱ�ֵ��port��pin����
//=====================================================================
void gpio_get_port_pin(uint16_t port_pin,uint8_t* port,uint8_t* pin)
{
    *port = (port_pin>>8);
    *pin = port_pin;
}

//=====================================================================
//�������ƣ�GPIO_EXTILineConfig
//�������أ���
//����˵����
//       port���˿ںţ���ָ����������
//       pin:���źţ�0~15��ʵ��ȡֵ��оƬ���������ž���������ָ����������
//���ܸ�Ҫ��ѡ������EXTI�ߵ�GPIO���š�
//=====================================================================
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
  uint32_t tmp = 0x00;

  tmp = ((uint32_t)0x0F) << (0x04 * (GPIO_PinSource & (uint8_t)0x03));
  AFIO->EXTICR[GPIO_PinSource >> 0x02] &= ~tmp;
  AFIO->EXTICR[GPIO_PinSource >> 0x02] |= (((uint32_t)GPIO_PortSource) << (0x04 * (GPIO_PinSource & (uint8_t)0x03)));
}

//��20211027-hsc��gpio_get���Ժ󣬷��ִ������⣺��Ҫ���ָߵ�λ��
