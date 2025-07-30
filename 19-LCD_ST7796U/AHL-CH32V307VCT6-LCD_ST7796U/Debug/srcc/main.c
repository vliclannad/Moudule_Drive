//======================================================================
//文件名称：main.c（应用工程主函数）
//框架提供：SD-Arm（sumcu.suda.edu.cn）
//版本更新：20191108-20200419
//功能描述：见本工程的..\01_Doc\Readme.txt
//移植规则：【固定】
//======================================================================
#define GLOBLE_VAR
#include "includes.h"     //包含总头文件


//----------------------------------------------------------------------
//声明使用到的内部函数
//main.c使用的内部函数声明处
void LCD_show_first();
void LCD_show_status(uint8_t rflag);
void mdelay_ms(uint32_t ms);

//----------------------------------------------------------------------
//主函数，一般情况下可以认为程序从此开始运行
int main(void)
{
    //（1）======启动部分（开头）==========================================
    //（1.1）声明main函数使用的局部变量

    uint16_t*    image_orginal;                 //彩色图像指针

    image_32    image_Gray_predict;         //灰度推理输入图像数组指针
    image_3_32  image_Color_predict;        //彩色输入图像数组指针


    uint8_t mSendModel=0;                  //终端输出模式标志位，1为彩色，0为灰色
    //（1.2）【不变】关总中断
    DISABLE_INTERRUPTS;

    //（1.3）给主函数使用的局部变量赋初值

    cam_rec_flag=0;                 //接收图像标志位
    uint8_t mflag=0;                //设备初始化状态
    uint8_t mCnt=0;                 //摄像头初始化次数

    //（1.4）给全局变量赋初值

    //初始化摄像头参数
    cam_ready=0;
    cam_fifo_ready=0;
    cam_refresh_end=0;
    //（1.5）用户外设模块初始化
    gpio_init(LIGHT_RED,GPIO_OUTPUT,LIGHT_OFF);     //初始化红灯
    gpio_init(LIGHT_BLUE,GPIO_OUTPUT,LIGHT_OFF);    //初始化蓝灯
    gpio_init(LIGHT_GREEN,GPIO_OUTPUT,LIGHT_OFF);   //初始化绿灯
    uart_init(UART_1,115200);                    //初始化User串口
    timer_init(TIMER_USER,20);          //LPTMR计时器初始化为1000毫秒
    gpio_init(LIGHT_BLUE,GPIO_OUTPUT,LIGHT_ON);
    uart_init(UART_User,115200);
    LCD_Init();
    //(1.5.3) 初始化摄像头，若不成功，停在此处
    printf("金葫芦提示： 正在初始化摄像头.....\r\n");
    mflag=cam_init();           //初始化摄像头
    while(mflag)
    {
        mCnt++;
        LCD_show_status(CAMERROR);
        printf("金葫芦提示： 正在初始化摄像头.....");
        printf("失败，可重新拔插摄像头！\r\n");
        printf("金葫芦提示： 系统将重新初始化摄像头....\r\n");
        mdelay_ms(200);
        mflag=cam_init();
    }
    if(mflag==0)
    {
        printf("金葫芦提示： 正在初始化摄像头.....成功\r\n");
        LCD_show_status(INITSUCCESS);     //LCD 系统初始化成功
    }
    LCD_show_first();                     //显示LCD预加载框架
     //（1.6）使能模块中断
    uart_enable_re_int(UART_User);
    gpio_enable_int(cam_VSYNC,FALLING_EDGE);   //使能摄像头帧中断
    LCD_DrawRectangle(30,24,214,195,RED); //标注屏幕红框//30,24,214,195

    //（1.7）【不变】开总中断
     ENABLE_INTERRUPTS;

    //（1）======启动部分（结尾）==========================================

    //（2）======主循环部分（开头）========================================
for(;;)   //for(;;)（开头）
    {
        //(2.1)显示正常运行状态
       //LCD_show_status(RUN);                 //LCD显示 正常运行
       
       //(2.2) 判断是否切换输出模式

       //(2.2)从摄像头模块中获得56*56大小的彩色图像

       //LCD_show_status(GETIMG);          //LCD显示 获取图像
//
//       //获取56*56大小的16位一维图像数组 并 在LCD显示图像
       image_orginal=cam_getimg_5656();
       //mSendModel=0;//写死为灰色
//       //(2.3)将1维数组转换为3维RGB数组
//
//
//       //(2.3.1)将一维图像数组转换为28*28的灰度数组 同时对数组进行滤波操作
       //LCD_show_status(SENDINGDATA);        //LCD显示  发送数据
/*
       if(mSendModel==1)       //彩色处理区域
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

       if(mSendModel==0) //灰度处理区域
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
    }  //for(;;)结尾
      

    //（2）======主循环部分（结尾）========================================
}   //main函数（结尾）


//======以下为主函数调用的子函数===========================================
//========================================================================
//函数名称:LCD_show_first
//函数返回:无
//参数说明:
//功能概要:显示出框架
//========================================================================
void LCD_show_first()
{
    printf("******************************************************\r\n");
    printf("                  EORS 数据集采集工程\r\n\r\n");
    printf("使用说明:\r\n");
    printf("(1)默认发送灰度数据集，通过触摸开发板右上角区域改变发送模式\r\n");
    printf("(2)触摸十次即Tsi Count达到十次后 切换输出模式 共有灰度与彩色两种模式\r\n");
    printf("(3)若触摸后Tsi Count值不增加，请查看板子上是否有异物，是否有焊渣，如有，请清除干净\r\n");
    printf("(4)将物体的位置放置在LCD给出的红框中\r\n");
    printf("(5)打开数据接收工程接收数据\r\n");
    printf("   注意事项:\r\n");
    printf("            (1)LCD红框中无其他多余物体\r\n");
    printf("            (2)保持识别场景光照良好\r\n");
    printf("******************************************************\r\n");
    printf("\r\n");

//    LCD显示框架加载
    LCD_ShowString(30,2,BLUE,WHITE,(const char *)"AI-EORS Detect Device");

    LCD_ShowString(30,215,BLUE,WHITE,(const char *)"Data Format:");

    LCD_ShowString(140,215,RED,WHITE,(const char *)"Gray");

    LCD_ShowString(30,245,BLUE,WHITE,(const char *)"Tsi Count:");

    LCD_DrawLine(0,293, 240, 293,BLACK); //系统状态分割线

    LCD_ShowString(10,300,BLUE,WHITE,(const char *)"Device Status:");
    LCD_show_status(INIT);
}
//========================================================================
//函数名称:LCD_show_status
//函数返回:无
//参数说明:rflag;设备运行状态参数
//功能概要:显示设备运行状态
//========================================================================
void LCD_show_status(uint8_t rflag)
{
    //定义设备状态数组
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

    LCD_ShowString(130,300,RED,WHITE,LcdShowResultArr[rflag-1]);//显示设备状态

}

void mdelay_ms(uint32_t ms)
{
    for(volatile uint32_t i= 0; i < (7200*ms); i++) __asm("nop");
}


//=========================================================================
/*
知识要素：
（1）main.c是一个模板，该文件所有代码均不涉及具体的硬件和环境，通过调用构件
实现对硬件的干预。
（2）本文件中对宏GLOBLE_VAR进行了定义，所以在包含"includes.h"头文件时，会定
义全局变量，在其他文件中包含"includes.h"头文件时，
编译时会自动增加extern
*/
