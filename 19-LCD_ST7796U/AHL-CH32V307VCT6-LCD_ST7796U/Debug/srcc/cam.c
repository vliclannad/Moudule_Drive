//=====================================================================
//�ļ����ƣ�ov7670.c
//���ܸ�Ҫ��ov7670����ͷ�ײ����������ļ�
//��    ����2020-08-09
//ʹ��оƬ��STM32L433xx
//=====================================================================

//����ͷ�ļ�
#include "includes.h"
#include "cam.h"

//=================�ڲ����ú�������=====================================
void cam_gpio_init();
void cam_config_window(uint16_t startx,uint16_t starty,uint16_t width, uint16_t height);
uint8_t rgb565togray_256 (uint16_t color);
void cam_reg_write(uint8_t regID, uint8_t regDat);
void cam_reg_read(uint8_t regID, uint8_t *regDat);
void cam_reg_init();
void Delay_us(uint16_t u16ms);
static uint16_t image16[56*56]={0};
//=====================================================================
//�������ƣ�cam_init
//�������أ�0�����óɹ�
//          1��ʧ��
//          2����ȡ�����ݵ��ǲ���ȷ�����³�ʼ��һ��
//����˵������
//���ܸ�Ҫ��cam��ʼ���������
//=====================================================================
uint8_t cam_init()
{
    uint8_t state;

    //��ʼ������
    cam_gpio_init();
    Delay_us(50);

    //��ʼ��SCCBͨ��Э��
    SCCB_Init();
    Delay_us(1);
    cam_reg_write(0x12, 0x80);  //��ȡ0X12�Ĵ���
    Delay_us(50);

//    ��ʼ��ov7670�Ĵ���
    cam_reg_init();
    Delay_us(50);

    //��������ͷ����Ĵ��ڸ�ʽΪ320*240
    cam_config_window(12,174,240,320);
    cam_reg_read(0x79, &state);

    //�ж��Ƿ��ʼ���ɹ�
    if(state==0x26)
    {
      return 0;                   //�ɹ�
    }
    else if(state==0xff)
    {
      return 1;                   //��ȡ�������� Ӳ����������
    }
    else
    {
      return 2;                   //��ȡ�����ݵ��ǲ���ȷ ���³�ʼ��һ��
    }
}

//=====================================================================
//�������ƣ�cam_getimg3d
//�������أ�ͼ������ָ��
//����˵������
//���ܸ�Ҫ�����56*56��С��RGBͼ��
//=====================================================================
uint16_t* cam_getimg_5656()
{

//    //(1)��ʼ����ر���
    register uint16_t j,i;
    register uint16_t color=0;

    uint16_t count=0;


    //����port_ptrΪGPIO�ṹ������ָ�루�׵�ַ��
    //ֱ��ͨ���Ĵ�����ַ����GPIO B��ض˿�
    //gpio_ptr->BSR = (uint32_t)(1u<<10); ��RCK���õ͵�ƽ����
    //gpio_ptr->BSHR = (uint32_t)(1u<<10); ��RCK���øߵ�ƽ����
    //Ϊ������ͼ�����ٶ�����ֱ��ʹ��
//    GPIO_TypeDef *gpio_ptr;
//    gpio_ptr=(GPIO_TypeDef *)GPIOB_BASE;
//
//
//    //(2)�õ�FIFO��������������ź�
    if(cam_refresh_end)
    {

//        (2.1)ֹͣov7670���ͼ��
        cam_FIFO_OE_0();

//        (2.2)FIFO��������ʹ��
        cam_FIFO_RRST_0();
        cam_FIFO_RCK_0();
        cam_FIFO_RCK_1();
        cam_FIFO_RCK_0();
        cam_FIFO_RRST_1();
        cam_FIFO_RCK_1();

        //ͼƬ��ʾ��С168*168
        //Address_set(2,1,168,168);//Address_set(40,25,206,192);
        Address_set(40,25,206,192);
        //(3)��ȡFIFO���������������ݲ���ʾ
        for(i=0;i<240;i++)
        {

            for(j=0;j<320;j++)
            {
                GPIOA->BCR = (uint32_t)(1u<<15);     //ʱ�ӿ��õ͵�ƽ

                if(i>36&&i<204&&j>76&&j<244)           //168*168if(i>36&&i<204&&j>76&&j<244)
                {
                    //(3.1)���ȶ�ȡRGB���ص�ĸ߰�λ
                    color=(GPIOD->INDR & 0x000000FF);

                    GPIOA->BSHR = (uint32_t)(1u<<15);//ʱ�ӿ��øߵ�ƽ

                    //(3.2)���ص�����8λ��ȡ�Ͱ�λ����
                    color<<=8;
                    __asm("NOP");

                    //(3.3)��ȡRGB���ص�ĵͰ�λ���ϳ�16λRGB����
                    GPIOA->BCR = (uint32_t)(1u<<15); //ʱ�ӿ��õ͵�ƽ
                    __asm("NOP");


                    //��ȡ��������16λ��ɫ��������
                    color|=(GPIOD->INDR & 0x000000FF);

                    //(3.4)ֱ��ͨ����SPI���л���ַ������ʵ�ֿ���LCD���� �滻LCD_WR_DATA(color);

                    GPIOE->BSHR = (uint32_t)(1u<<0); //lcd_dc ��e0 ��1
                    GPIOE->BCR = (uint32_t)(1u<<1);  //lcd cs ��e1 ��0

                    spi_send1(LCD_SPI,color>>8);
                    spi_send1(LCD_SPI,color);
                    GPIOE->BSHR = (uint32_t)(1u<<1); //lcd cs ��b6 ��1


                }
                else
                {
                    GPIOA->BSHR = (uint32_t)(1u<<15);
                    GPIOA->BCR = (uint32_t)(1u<<15);
                }

                GPIOA->BSHR = (uint32_t)(1u<<15);

                //(3.4)�洢56*56�����ص�����
                if(i%3==2&&j%3==2&&i>36&&i<204&&j>76&&j<244)
                {


                    //������������ͼ������
                    image16[count]=color;
                    count++;
                }
            }
        }

        //(4)���¸�λov7670���ʹ��
        cam_FIFO_OE_1();
        cam_FIFO_RCK_0();
        cam_FIFO_RCK_1();

        //(5)���������־
        cam_ready=0;
        cam_fifo_ready=0;
        cam_refresh_end=0;
        //(6)ʹ������֡�ж�
        gpio_enable_int(cam_VSYNC,FALLING_EDGE);
    }
    //��7������56*56��С�Ĳ�ɫ��������
    return image16;
}

//=====================================================================
//�������ƣ�cam_open
//�������أ���
//����˵������
//���ܸ�Ҫ��������ͷ
//=====================================================================
void cam_open()
{
   gpio_enable_int(cam_VSYNC,FALLING_EDGE);
}

//=====================================================================
//�������ƣ�cam_close
//�������أ���
//����˵������
//���ܸ�Ҫ���ر�����ͷ
//=====================================================================
void cam_close()
{
    gpio_disable_int(cam_VSYNC);
}

//=====================================================================
//�������ƣ�cam_FIFO_WRST_1
//�������أ���
//����˵������
//���ܸ�Ҫ��FIFO��ʹ�ܸ�λ�ڸߵ�ƽ���
//=====================================================================
inline void cam_FIFO_WRST_1()
{
    gpio_init(cam_FIFO_WRST,1,1);
}

//=====================================================================
//�������ƣ�cam_FIFO_WRST_0
//�������أ���
//����˵������
//���ܸ�Ҫ��FIFO��ʹ�ܸ�λ�ڵ͵�ƽ���
//=====================================================================
inline void cam_FIFO_WRST_0()
{
    gpio_init(cam_FIFO_WRST,1,0);
}

//=====================================================================
//�������ƣ�cam_FIFO_WR_1
//�������أ���
//����˵������
//���ܸ�Ҫ��FIFOдʹ�ܿڸߵ�ƽ���
//=====================================================================
inline void cam_FIFO_WR_1()
{
    gpio_init(cam_FIFO_WR,1,1);
}

//=====================================================================
//�������ƣ�cam_FIFO_WR_0
//�������أ���
//����˵������
//���ܸ�Ҫ��FIFOдʹ�ܿڵ͵�ƽ���
//=====================================================================
inline void cam_FIFO_WR_0()
{
    gpio_init(cam_FIFO_WR,1,0);
}

//=====================================================================
//�������ƣ�cam_FIFO_RCK_0
//�������أ���
//����˵������
//���ܸ�Ҫ��FIFOʱ�ӿڵ͵�ƽ���
//=====================================================================
inline void cam_FIFO_RCK_0()
{
    gpio_set(cam_FIFO_RCK,0);
}

//=====================================================================
//�������ƣ�cam_FIFO_RCK_1
//�������أ���
//����˵������
//���ܸ�Ҫ��FIFOʱ�ӿڸߵ�ƽ���
//=====================================================================
inline void cam_FIFO_RCK_1()
{
    gpio_set(cam_FIFO_RCK,1);
}

//=====================================================================
//�������ƣ�cam_FIFO_RRST_0
//�������أ���
//����˵������
//���ܸ�Ҫ��FIFOдʹ�ܸ�λ�͵�ƽ���
//=====================================================================
inline void cam_FIFO_RRST_0()
{
    gpio_init(cam_FIFO_RRST,1,0);
}

//=====================================================================
//�������ƣ�cam_FIFO_RRST_1
//�������أ���
//����˵������
//���ܸ�Ҫ��FIFOдʹ�ܸ�λ�ߵ�ƽ���
//=====================================================================
inline void cam_FIFO_RRST_1()
{
    gpio_init(cam_FIFO_RRST,1,1);
}


//=====================================================================
//�������ƣ�cam_FIFO_OE_0
//�������أ���
//����˵������
//���ܸ�Ҫ��FIFO���ʹ�ܵ͵�ƽ���
//=====================================================================
inline void cam_FIFO_OE_0()
{
    gpio_init(cam_FIFO_OE,1,0);
}

//=====================================================================
//�������ƣ�cam_FIFO_OE_0
//�������أ���
//����˵������
//���ܸ�Ҫ��FIFO���ʹ�ܸߵ�ƽ���
//=====================================================================
inline void cam_FIFO_OE_1()
{
    gpio_init(cam_FIFO_OE,1,1);
}

//----------------------����Ϊ�ڲ�������Ŵ�----------------------------

//=====================================================================
//�������ƣ�cam_gpio_init
//�������أ���
//����˵������
//���ܸ�Ҫ����ʼ��ov7670ʹ�õ�gpio����
//=====================================================================
void cam_gpio_init()
{
    //��ʼ�����ƽ�
    gpio_init(cam_FIFO_RCK,1,0);
    gpio_init(cam_VSYNC, 0, 1);         //��ʼ��֡�ж϶�ȡ����
    printf("��ʼ������");

    //��ʼ��ͼ�����ݽ���8������
    gpio_init(cam_DATA_IO_0, 0, 1);
    gpio_init(cam_DATA_IO_1, 0, 1);
    gpio_init(cam_DATA_IO_2, 0, 1);
    gpio_init(cam_DATA_IO_3, 0, 1);
    gpio_init(cam_DATA_IO_4, 0, 1);
    gpio_init(cam_DATA_IO_5, 0, 1);
    gpio_init(cam_DATA_IO_6, 0, 1);
    gpio_init(cam_DATA_IO_7, 0, 1);

    //��ʼ������ͷ���ʹ�ܽ�
    cam_FIFO_OE_0();
}

//=====================================================================
//�������ƣ�cam_config_window
//�������أ���
//����˵����startx������ͼ��ʼ��ȡ��x����
//          starty������ͼ��ʼ��ȡ��y����
//          width��������ʾͼ�񴰿ڵĿ��
//          height��������ʾͼ�񴰿ڵĸ߶�
//���ܸ�Ҫ������ov7670�������ʾͼ��Ĵ��ڲ���
//          (140,16,640,480)�ʺ�VGA��ʽ�����ͼ��
//          (272,16,320,240)�ʺ�QVGA��ʽ�����ͼ��
//=====================================================================
void cam_config_window(uint16_t startx,uint16_t starty,uint16_t width, uint16_t height)
{
    uint16_t endx;                      //ͼ��������x����
    uint16_t endy;                      //ͼ��������y����
    uint8_t temp_reg1, temp_reg2;

    //����ͼ����ʾ������ĺ�������
    endx=(startx+width*2);
    endy=(starty+height+height);

    //�����ж�
    if(endy>784) endy-=784;

    //�޸�ͼ������Ŀ�ʼ��������ص�
    cam_reg_read(0x03, &temp_reg1 );
    temp_reg1 &= 0xf0;
    temp_reg1|=((endx&0X03)<<2)|(startx&0X03);
    cam_reg_write(0X03,temp_reg1);
    cam_reg_write(0X19,startx>>2);
    cam_reg_write(0X1A,endx>>2);

    cam_reg_read(0x32, &temp_reg2 );
    temp_reg2 &= 0xc0;
    temp_reg2|=((endy&0x7)<<3)|(starty&0x7);
    cam_reg_write(0x17, starty>>3 );
    cam_reg_write(0x18, endy>>3);
}

//=====================================================================
//�������ƣ�cam_reg_write
//�������أ���
//����˵����regID���Ĵ�����ַ
//          regDat��д����ֵ
//���ܸ�Ҫ������ov7670ָ����ַ�Ĵ�������
//=====================================================================
 void cam_reg_write(uint8_t regID, uint8_t regDat)
{
    SCCB_Write(cam_OV7670_ADRESS, regID, regDat);
}

//=====================================================================
//�������ƣ�cam_reg_read
//�������أ���
//����˵����regID���Ĵ�����ַ
//          regDat��д����ֵ
//���ܸ�Ҫ����ȡov7670ָ����ַ�Ĵ�������
//=====================================================================
 void cam_reg_read(uint8_t regID, uint8_t *regDat)
{
    SCCB_Read(cam_OV7670_ADRESS, regID, regDat);
}

//=====================================================================
//�������ƣ�cam_reg_init
//�������أ���
//����˵����
//���ܸ�Ҫ���ο��ٷ��ֲ�����ov7670�Ĵ���
//=====================================================================
void cam_reg_init()
{
    cam_reg_write(0x3a, 0x04);
    cam_reg_write(0x40, 0xd0);          //565
    cam_reg_write(0x12, 0x14);          //QVGA,RGB���
    //�����������
    cam_reg_write(0x32, 0x80);
    cam_reg_write(0x17, 0x16);
    cam_reg_write(0x18, 0x04);
    cam_reg_write(0x19, 0x02);
    cam_reg_write(0x1a, 0x7b);
    cam_reg_write(0x03, 0x06);          //֡��ֱ�������

    cam_reg_write(0x0c, 0x00);
    cam_reg_write(0x15, 0x00);
    cam_reg_write(0x3e, 0x00);
    cam_reg_write(0x70, 0x3a);
    cam_reg_write(0x71, 0x35);
    cam_reg_write(0x72, 0x11);
    cam_reg_write(0x73, 0x00);

    cam_reg_write(0xa2, 0x02);
    cam_reg_write(0x11, 0x81);           //ʱ�ӷ�Ƶ���ã�0������Ƶ
    cam_reg_write(0x7a, 0x20);
    cam_reg_write(0x7b, 0x1c);
    cam_reg_write(0x7c, 0x28);

    cam_reg_write(0x7d, 0x3c);
    cam_reg_write(0x7e, 0x55);
    cam_reg_write(0x7f, 0x68);
    cam_reg_write(0x80, 0x76);
    cam_reg_write(0x81, 0x80);

    cam_reg_write(0x82, 0x88);
    cam_reg_write(0x83, 0x8f);
    cam_reg_write(0x84, 0x96);
    cam_reg_write(0x85, 0xa3);
    cam_reg_write(0x86, 0xaf);

    cam_reg_write(0x87, 0xc4);
    cam_reg_write(0x88, 0xd7);
    cam_reg_write(0x89, 0xe8);
    cam_reg_write(0x13, 0xe0);
    cam_reg_write(0x00, 0x00);

    cam_reg_write(0x10, 0x00);
    cam_reg_write(0x0d, 0x30);            //ȫ���ڣ� λ[5:4]: 01 �봰�ڣ�10 1/4���ڣ�11 1/4����
    cam_reg_write(0x14, 0x28);
    cam_reg_write(0xa5, 0x05);
    cam_reg_write(0xab, 0x07);

    cam_reg_write(0x24, 0x75);
    cam_reg_write(0x25, 0x63);
    cam_reg_write(0x26, 0xA5);
    cam_reg_write(0x9f, 0x78);
    cam_reg_write(0xa0, 0x68);

    cam_reg_write(0xa1, 0x03);          //0x0b,
    cam_reg_write(0xa6, 0xdf);          //0xd8,
    cam_reg_write(0xa7, 0xdf);          //0xd8,
    cam_reg_write(0xa8, 0xf0);
    cam_reg_write(0xa9, 0x90);

    cam_reg_write(0xaa, 0x94);
    cam_reg_write(0x13, 0xe5);
    cam_reg_write(0x0e, 0x61);
    cam_reg_write(0x0f, 0x4b);
    cam_reg_write(0x16, 0x02);

    cam_reg_write(0x1e, 0x37);          //ͼ������������.0x07
    cam_reg_write(0x21, 0x02);
    cam_reg_write(0x22, 0x91);
    cam_reg_write(0x29, 0x07);
    cam_reg_write(0x33, 0x0b);

    cam_reg_write(0x35, 0x0b);
    cam_reg_write(0x37, 0x1d);
    cam_reg_write(0x38, 0x71);
    cam_reg_write(0x39, 0x2a);
    cam_reg_write(0x3c, 0x78);

    cam_reg_write(0x4d, 0x40);
    cam_reg_write(0x4e, 0x20);
    cam_reg_write(0x69, 0x00);
    cam_reg_write(0x6b, 0x40);
    cam_reg_write(0x74, 0x19);
    cam_reg_write(0x8d, 0x4f);

    cam_reg_write(0x8e, 0x00);
    cam_reg_write(0x8f, 0x00);
    cam_reg_write(0x90, 0x00);
    cam_reg_write(0x91, 0x00);
    cam_reg_write(0x92, 0x00);

    cam_reg_write(0x96, 0x00);
    cam_reg_write(0x9a, 0x80);
    cam_reg_write(0xb0, 0x84);
    cam_reg_write(0xb1, 0x0c);
    cam_reg_write(0xb2, 0x0e);

    cam_reg_write(0xb3, 0x82);
    cam_reg_write(0xb8, 0x0a);
    cam_reg_write(0x43, 0x14);
    cam_reg_write(0x44, 0xf0);
    cam_reg_write(0x45, 0x34);

    cam_reg_write(0x46, 0x58);
    cam_reg_write(0x47, 0x28);
    cam_reg_write(0x48, 0x3a);
    cam_reg_write(0x59, 0x88);
    cam_reg_write(0x5a, 0x88);

    cam_reg_write(0x5b, 0x44);
    cam_reg_write(0x5c, 0x67);
    cam_reg_write(0x5d, 0x49);
    cam_reg_write(0x5e, 0x0e);
    cam_reg_write(0x64, 0x04);
    cam_reg_write(0x65, 0x20);

    cam_reg_write(0x66, 0x05);
    cam_reg_write(0x94, 0x04);
    cam_reg_write(0x95, 0x08);
    cam_reg_write(0x6c, 0x0a);
    cam_reg_write(0x6d, 0x55);

    cam_reg_write(0x4f, 0x80);
    cam_reg_write(0x50, 0x80);
    cam_reg_write(0x51, 0x00);
    cam_reg_write(0x52, 0x22);
    cam_reg_write(0x53, 0x5e);
    cam_reg_write(0x54, 0x80);

    cam_reg_write(0x09, 0x03);           //�����������

    cam_reg_write(0x6e, 0x11);
    cam_reg_write(0x6f, 0x9f);
    cam_reg_write(0x55, 0x40);          //���ȿ���  30
    cam_reg_write(0x56, 0x40);          //�Աȶ� 0x40
    cam_reg_write(0x57, 0x40);

    cam_reg_write(0x6a, 0x40);
    cam_reg_write(0x01, 0x40);
    cam_reg_write(0x02, 0x40);
    cam_reg_write(0x13, 0xe7);
    cam_reg_write(0x15, 0x00);

    cam_reg_write(0x58, 0x9e);

    cam_reg_write(0x41, 0x08);
    cam_reg_write(0x3f, 0x00);
    cam_reg_write(0x75, 0x05);
    cam_reg_write(0x76, 0xe1);
    cam_reg_write(0x4c, 0x00);
    cam_reg_write(0x77, 0x01);
    cam_reg_write(0x3d, 0xc2);
    cam_reg_write(0x4b, 0x09);
    cam_reg_write(0xc9, 0x60);
    cam_reg_write(0x41, 0x38);

    cam_reg_write(0x34, 0x11);
    cam_reg_write(0x3b, 0x02);

    cam_reg_write(0xa4, 0x89);
    cam_reg_write(0x96, 0x00);
    cam_reg_write(0x97, 0x30);
    cam_reg_write(0x98, 0x20);
    cam_reg_write(0x99, 0x30);
    cam_reg_write(0x9a, 0x84);
    cam_reg_write(0x9b, 0x29);
    cam_reg_write(0x9c, 0x03);
    cam_reg_write(0x9d, 0x4c);
    cam_reg_write(0x9e, 0x3f);
    cam_reg_write(0x78, 0x04);

    cam_reg_write(0x79, 0x01);
    cam_reg_write(0xc8, 0xf0);
    cam_reg_write(0x79, 0x0f);
    cam_reg_write(0xc8, 0x00);
    cam_reg_write(0x79, 0x10);
    cam_reg_write(0xc8, 0x7e);
    cam_reg_write(0x79, 0x0a);
    cam_reg_write(0xc8, 0x80);
    cam_reg_write(0x79, 0x0b);
    cam_reg_write(0xc8, 0x01);
    cam_reg_write(0x79, 0x0c);
    cam_reg_write(0xc8, 0x0f);
    cam_reg_write(0x79, 0x0d);
    cam_reg_write(0xc8, 0x20);
    cam_reg_write(0x79, 0x09);
    cam_reg_write(0xc8, 0x80);
    cam_reg_write(0x79, 0x02);
    cam_reg_write(0xc8, 0xc0);
    cam_reg_write(0x79, 0x03);
    cam_reg_write(0xc8, 0x40);
    cam_reg_write(0x79, 0x05);
    cam_reg_write(0xc8, 0x30);
    cam_reg_write(0x79, 0x26);
    cam_reg_write(0x09, 0x00);


}


//======================================================================
//�������ƣ�Delay_us
//�������أ���
//����˵������
//���ܸ�Ҫ����ʱ - ΢�뼶
//======================================================================
void Delay_us(uint16_t u16ms)
{
    uint32_t u32ctr;
    for(u32ctr = 0; u32ctr < (SystemCoreClock/1000*u16ms/100000); u32ctr++)
    {
         __asm ("NOP") ;
    }
}

