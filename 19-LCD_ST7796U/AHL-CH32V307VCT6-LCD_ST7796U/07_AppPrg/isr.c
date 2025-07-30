//=====================================================================
//文件名称：isr.c（中断处理程序源文件）
//框架提供：SD-ARM（sumcu.suda.edu.cn）
//版本更新：20170801-20191020
//功能描述：提供中断处理程序编程框架
//移植规则：【固定】
//=====================================================================
#include "includes.h"


//======================================================================
//程序名称：UART_User_Handler
//触发条件：UART_User串口收到一个字节触发
//备    注：进入本程序后，可使用uart_get_re_int函数可再进行中断标志判断
//          （1-有UART接收中断，0-没有UART接收中断）
//======================================================================
void UART_User_Handler(void)
{
	 //（1）变量声明
    uint8_t flag,ch;
    DISABLE_INTERRUPTS;      //关总中断
    //（2）未触发串口接收中断，退出
    if(!uart_get_re_int(UART_User)) goto UART_User_Handler_EXIT;
    //（3）收到一个字节，读出该字节数据
    ch = uart_re1(UART_User,&flag);        //调用接收一个字节的函数
    if(!flag) goto UART_User_Handler_EXIT; //实际未收到数据，退出
  //（4）以下代码根据是否使用模板提供的User串口通信帧结构，及是否利用User串口
    //     进行带有设备序列号的进行程序更新而选择
    //（4.1）【自行组帧使用（开始)】
        uart_send1(UART_User,ch);     //例如，收到一个字节回发一个字节
       //    【自行组帧使用（结束)】
    //（4.2）【使用模板提供的User串口通信帧结构（开始)】
    /*
       User_SysFun(ch);          //利用User串口进行程序更新
       if (gcRecvLen == 0) goto UART_User_Handler_EXIT; 
       //至此，不仅收到完整帧，且序号比较也一致，可以根据命令字节gcRecvBuf[16]进行编程
       switch(gcRecvBuf[16])  //帧标识
       {
           case 1:  //0之外的数据，自身命令
           break;
           default:
           break;
        }
        gcRecvLen = 0;   //帧已经使用完毕，下次若收到一个字节，可以继续组帧
        //【使用模板提供的User串口通信帧结构（结束)】
     */
    //（5）【公共退出区】
UART_User_Handler_EXIT:
    ENABLE_INTERRUPTS;//开总中断
}

//======================================================================
//程序名称：Cam_User_Handler
//触发条件：摄像头模块发送完整一帧数据后从VSYNC脚发出上升沿中断
//备    注：
//          
//======================================================================
void Cam_User_Handler(void)
{
	DISABLE_INTERRUPTS;
    //接收到摄像头模块引脚VSYNC中断触发
    if(gpio_get_int(cam_VSYNC))
    {
    	//(1)清除该引脚中断
    	gpio_clear_int(cam_VSYNC);
//    	printf("cam_ready = %d\r\n",cam_ready);
//   		(2.1)判断摄像头是否发送完成完整帧数据
   		if(cam_ready==0)
   		{
   			//(2.1.1)停止FIFO接收摄像头发送的新数据
   			cam_FIFO_WRST_0();
			cam_FIFO_WRST_1();
			cam_FIFO_WR_1();
			cam_fifo_ready=1;
			cam_ready=1;
    	}
    	//(2.2)判断MCU是否已经读完缓冲区数据
   		else if(cam_fifo_ready==1)
        {
            //(2.2.1)关闭中断
            cam_refresh_end=1;
            cam_FIFO_WR_0();
//            gpio_disable_int(cam_VSYNC);
        }
    }
    ENABLE_INTERRUPTS;
}

//=====================================================================
//函数名称：TIMER_USER_Handler（TPM1定时器中断处理程序）
//参数说明：无
//函数返回：无
//功能概要：（1）每20ms中断触发本程序一次；（2）达到一秒时，调用秒+1
//           程序，计算“时、分、秒”
//特别提示：（1）使用全局变量字节型数组gTime[3]，分别存储“时、分、秒”
//        （2）注意其中静态变量的使用
//=====================================================================
void TIMER_USER_Handler(void)
{
    printf("触发了timer中断\r\n");
	DISABLE_INTERRUPTS;                        //关总中断
	//------------------------------------------------------------------
	//（在此处增加功能）
	//申请一个静态的计数器值
	static uint8_t TimerCount = 0;
	//获取当前时钟溢出标志位
	if(timer_get_int(TIMER_USER))
	{
		TimerCount++;           //计数器累加
		if (TimerCount >= 50)
		{
			TimerCount = 0;    //时钟计数器清零
			gTimberFlag1s=1;
		}
		TIM7->CNT = 0;
	}
	//------------------------------------------------------------------
	timer_clear_int(TIMER_USER);    //清时钟溢出标志位
	ENABLE_INTERRUPTS;                        //开总中断
}
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void HardFault_Handler(void)
{
  printf("mcause:%08x\r\n",__get_MCAUSE());
  printf("mepc:%08x\r\n",__get_MEPC());
  printf("mtval:%08x\r\n",__get_MTVAL());

  while (1)
  {

  }
}

/*
知识要素：
1.本文件中的中断处理函数调用的均是相关设备封装好的具体构件，在更换芯片
时，只需保证设备的构件接口一致，即可保证本文件的相关中断处理函数不做任何
更改，从而达到芯片无关性的要求。
*/

 /* -----------------------------------------内部函数--------------------------------------- */


