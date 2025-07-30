//======================================================================
//�ļ����ƣ�main.c��Ӧ�ù�����������
//����ṩ��SD-Arm��sumcu.suda.edu.cn��
//�汾���£�20191108-20200419
//�����������������̵�..\01_Doc\Readme.txt
//��ֲ���򣺡��̶���
//======================================================================
#define GLOBLE_VAR
#include "includes.h"     //������ͷ�ļ�


//----------------------------------------------------------------------
//����ʹ�õ����ڲ�����
//main.cʹ�õ��ڲ�����������
void LCD_show_first();
void LCD_show_status(uint8_t rflag);
void mdelay_ms(uint32_t ms);

//----------------------------------------------------------------------
//��������һ������¿�����Ϊ����Ӵ˿�ʼ����
int main(void)
{
    //��1��======�������֣���ͷ��==========================================
    //��1.1������main����ʹ�õľֲ�����

    uint16_t*    image_orginal;                 //��ɫͼ��ָ��

    image_32    image_Gray_predict;         //�Ҷ���������ͼ������ָ��
    image_3_32  image_Color_predict;        //��ɫ����ͼ������ָ��


    uint8_t mSendModel=0;                  //�ն����ģʽ��־λ��1Ϊ��ɫ��0Ϊ��ɫ
    //��1.2�������䡿�����ж�
    DISABLE_INTERRUPTS;

    //��1.3����������ʹ�õľֲ���������ֵ

    cam_rec_flag=0;                 //����ͼ���־λ
    uint8_t mflag=0;                //�豸��ʼ��״̬
    uint8_t mCnt=0;                 //����ͷ��ʼ������

    //��1.4����ȫ�ֱ�������ֵ

    //��ʼ������ͷ����
    cam_ready=0;
    cam_fifo_ready=0;
    cam_refresh_end=0;
    //��1.5���û�����ģ���ʼ��
    gpio_init(LIGHT_RED,GPIO_OUTPUT,LIGHT_OFF);     //��ʼ�����
    gpio_init(LIGHT_BLUE,GPIO_OUTPUT,LIGHT_OFF);    //��ʼ������
    gpio_init(LIGHT_GREEN,GPIO_OUTPUT,LIGHT_OFF);   //��ʼ���̵�
    uart_init(UART_1,115200);                    //��ʼ��User����
    timer_init(TIMER_USER,20);          //LPTMR��ʱ����ʼ��Ϊ1000����
    gpio_init(LIGHT_BLUE,GPIO_OUTPUT,LIGHT_ON);
    uart_init(UART_User,115200);
    LCD_Init();
    //(1.5.3) ��ʼ������ͷ�������ɹ���ͣ�ڴ˴�
    printf("���«��ʾ�� ���ڳ�ʼ������ͷ.....\r\n");
    mflag=cam_init();           //��ʼ������ͷ
    while(mflag)
    {
        mCnt++;
        LCD_show_status(CAMERROR);
        printf("���«��ʾ�� ���ڳ�ʼ������ͷ.....");
        printf("ʧ�ܣ������°β�����ͷ��\r\n");
        printf("���«��ʾ�� ϵͳ�����³�ʼ������ͷ....\r\n");
        mdelay_ms(200);
        mflag=cam_init();
    }
    if(mflag==0)
    {
        printf("���«��ʾ�� ���ڳ�ʼ������ͷ.....�ɹ�\r\n");
        LCD_show_status(INITSUCCESS);     //LCD ϵͳ��ʼ���ɹ�
    }
    LCD_show_first();                     //��ʾLCDԤ���ؿ��
     //��1.6��ʹ��ģ���ж�
    uart_enable_re_int(UART_User);
    gpio_enable_int(cam_VSYNC,FALLING_EDGE);   //ʹ������ͷ֡�ж�
    LCD_DrawRectangle(30,24,214,195,RED); //��ע��Ļ���//30,24,214,195

    //��1.7�������䡿�����ж�
     ENABLE_INTERRUPTS;

    //��1��======�������֣���β��==========================================

    //��2��======��ѭ�����֣���ͷ��========================================
for(;;)   //for(;;)����ͷ��
    {
        //(2.1)��ʾ��������״̬
       //LCD_show_status(RUN);                 //LCD��ʾ ��������
       
       //(2.2) �ж��Ƿ��л����ģʽ

       //(2.2)������ͷģ���л��56*56��С�Ĳ�ɫͼ��

       //LCD_show_status(GETIMG);          //LCD��ʾ ��ȡͼ��
//
//       //��ȡ56*56��С��16λһάͼ������ �� ��LCD��ʾͼ��
       image_orginal=cam_getimg_5656();
       //mSendModel=0;//д��Ϊ��ɫ
//       //(2.3)��1ά����ת��Ϊ3άRGB����
//
//
//       //(2.3.1)��һάͼ������ת��Ϊ28*28�ĻҶ����� ͬʱ����������˲�����
       //LCD_show_status(SENDINGDATA);        //LCD��ʾ  ��������
/*
       if(mSendModel==1)       //��ɫ��������
       {
           LCD_ShowString(140,215,RED,WHITE,(const char *)"Color");
           Model_GetInputImg_Color(image_orginal,image_Color_predict);
           printf("sc\n");
           for(int c=0;c<3;c++)
           {
               for(int h=2;h<30;h++)
               {
                   for(int w=2;w<30;w++)
                   {
                       printf("%d ",image_Color_predict[c][h][w]);
                   }
                   printf("\n");
               }
           }
           printf("e\n");
       }

       if(mSendModel==0) //�Ҷȴ�������
       {
           Model_GetInputImg_Gray(image_orginal,image_Gray_predict);
           LCD_ShowString(140,215,RED,WHITE,(const char *)"Gary");
           printf("sg\n");
           for(int h=2;h<30;h++)
           {
               for(int w=2;w<30;w++)
               {
                   printf("%d ",image_Gray_predict[h][w]);

               }
               printf("\n");
           }
           printf("e\n");
       }

*/
    }  //for(;;)��β
      

    //��2��======��ѭ�����֣���β��========================================
}   //main��������β��


//======����Ϊ���������õ��Ӻ���===========================================
//========================================================================
//��������:LCD_show_first
//��������:��
//����˵��:
//���ܸ�Ҫ:��ʾ�����
//========================================================================
void LCD_show_first()
{
    printf("******************************************************\r\n");
    printf("                  EORS ���ݼ��ɼ�����\r\n\r\n");
    printf("ʹ��˵��:\r\n");
    printf("(1)Ĭ�Ϸ��ͻҶ����ݼ���ͨ���������������Ͻ�����ı䷢��ģʽ\r\n");
    printf("(2)����ʮ�μ�Tsi Count�ﵽʮ�κ� �л����ģʽ ���лҶ����ɫ����ģʽ\r\n");
    printf("(3)��������Tsi Countֵ�����ӣ���鿴�������Ƿ�������Ƿ��к��������У�������ɾ�\r\n");
    printf("(4)�������λ�÷�����LCD�����ĺ����\r\n");
    printf("(5)�����ݽ��չ��̽�������\r\n");
    printf("   ע������:\r\n");
    printf("            (1)LCD�������������������\r\n");
    printf("            (2)����ʶ�𳡾���������\r\n");
    printf("******************************************************\r\n");
    printf("\r\n");

//    LCD��ʾ��ܼ���
    LCD_ShowString(30,2,BLUE,WHITE,(const char *)"AI-EORS Detect Device");

    LCD_ShowString(30,215,BLUE,WHITE,(const char *)"Data Format:");

    LCD_ShowString(140,215,RED,WHITE,(const char *)"Gray");

    LCD_ShowString(30,245,BLUE,WHITE,(const char *)"Tsi Count:");

    LCD_DrawLine(0,293, 240, 293,BLACK); //ϵͳ״̬�ָ���

    LCD_ShowString(10,300,BLUE,WHITE,(const char *)"Device Status:");
    LCD_show_status(INIT);
}
//========================================================================
//��������:LCD_show_status
//��������:��
//����˵��:rflag;�豸����״̬����
//���ܸ�Ҫ:��ʾ�豸����״̬
//========================================================================
void LCD_show_status(uint8_t rflag)
{
    //�����豸״̬����
    const char * LcdShowResultArr[]=
    {
        "Init Device",
        "CAM Error",
        "Getting Img",
        "Predicting",
        "Running",
        "InitSuccess",
        "SendingData",
        "FilteringError"
    };

    LCD_DrawSurface(130,300,240,320,WHITE);

    LCD_ShowString(130,300,RED,WHITE,LcdShowResultArr[rflag-1]);//��ʾ�豸״̬

}

void mdelay_ms(uint32_t ms)
{
    for(volatile uint32_t i= 0; i < (7200*ms); i++) __asm("nop");
}


//=========================================================================
/*
֪ʶҪ�أ�
��1��main.c��һ��ģ�壬���ļ����д�������漰�����Ӳ���ͻ�����ͨ�����ù���
ʵ�ֶ�Ӳ���ĸ�Ԥ��
��2�����ļ��жԺ�GLOBLE_VAR�����˶��壬�����ڰ���"includes.h"ͷ�ļ�ʱ���ᶨ
��ȫ�ֱ������������ļ��а���"includes.h"ͷ�ļ�ʱ��
����ʱ���Զ�����extern
*/
