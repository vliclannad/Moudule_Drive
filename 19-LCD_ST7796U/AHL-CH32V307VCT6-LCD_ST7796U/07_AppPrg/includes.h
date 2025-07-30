//======================================================================
//文件名称：includes.h（应用工程总头文件）
//框架提供：SD-Arm（sumcu.suda.edu.cn）
//版本更新：20170801-20200223
//功能描述：包含用到的构件、宏常数定义、自定义数据类型、声明全局变量。
//移植规则：【固定】
//======================================================================

#ifndef  INCLUDES_H     //防止重复定义（INCLUDES_H 开头）
#define  INCLUDES_H

//----------------------------------------------------------------------
//（1）包含用到的构件
#include "user.h"
#include "lcd.h"
#include "spi.h"
//#include "wdog.h"
#include "sccb.h"
#include "cam.h"
#include "NumToStr.h"
#include "model_init.h"
#include "modelPredict.h"
#include "modelPreprocess.h"
#include "timer.h"

//----------------------------------------------------------------------
//（2）全局使用的宏常数。
//在此修改lcd显示的物体英文名称
#define SORT1 "Num1"
#define SORT2 "Num2"
#define SORT3 "Num3"
#define SORT4 "4"
#define SORT5 "5"
#define SORT6 "6"
#define SORT7 "7"
#define SORT8 "8"
#define SORT9 "9"
#define SORT10 "10"

#define INIT     		 1
#define CAMERROR         2
#define GETIMG           3
#define PRDICT           4
#define RUN              5
#define INITSUCCESS      6
#define SENDINGDATA      7
#define FILTERERROR      8


//----------------------------------------------------------------------
//（3）自定义数据类型
#pragma  pack(push,1)
struct shakeData
{
    uint8_t uecomType[25];  //通信模组类型
    uint8_t mcuType[20];    //MCU类型
    uint32_t uStartAds;     //User程序起始地址
    uint32_t uCodeSize;     //User程序总代码大小
    uint32_t replaceNum;    //替换更新最大字节
    uint32_t reserveNum;    //保留更新最大字节（不等于0意味着有User程序）
    uint8_t BIOSVersion[10];  //BIOS版本
};
#pragma  pack(pop)
//----------------------------------------------------------------------

//（4）声明全局变量。命名规范见注2。
//【不动】宏定义全局变量前缀G_VAR_PREFIX。实现原理见注3。
#ifdef GLOBLE_VAR              //GLOBLE_VAR在main.c文件中有宏定义
  #define G_VAR_PREFIX         //前缀G_VAR_PREFIX定义为空
#else                          //GLOBLE_VAR在非main.c文件中无定义
  #define G_VAR_PREFIX extern  //前缀G_VAR_PREFIX定义为"extern"
#endif

//（在此增加全局变量）
G_VAR_PREFIX uint8_t  gFunFlag;        //主函数功能标识
G_VAR_PREFIX uint16_t gRecvdataLen;    //串口1接收到非更新数据的数据长度
G_VAR_PREFIX vuint16_t gcRecvLen;
G_VAR_PREFIX vuint16_t gcRecvDCLen;
G_VAR_PREFIX vuint8_t gcReccrc32[4];
G_VAR_PREFIX vuint8_t gcRecvBuf[4096];
G_VAR_PREFIX vuint8_t Rst_Count;  //按“复位键”的次数
G_VAR_PREFIX vuint8_t gcUpdateFlag[10];
G_VAR_PREFIX struct shakeData gShakeData;   //握手帧数据

G_VAR_PREFIX uint8_t gTimberFlag1s;
G_VAR_PREFIX uint8_t cam_ready;
G_VAR_PREFIX uint8_t cam_fifo_ready;
G_VAR_PREFIX uint8_t cam_refresh_end;
G_VAR_PREFIX uint8_t cam_rec_flag;
G_VAR_PREFIX uint8_t ov_sta;

//----------------------------------------------------------------------

#endif                  //防止重复定义（INCLUDES_H 结尾）


/*
 知识要素：
 注1：全局使用的宏常数，使用全大写
 注2：全局变量命名一律以g开头，尽可能保证全工程内唯一性，并且一定要注明
      其含义，（声明时不准赋值）
 注3：全局变量一处声明多处使用处理方法代码段是为了解决全局变量重复声明
 的问题，宏GLOBLE_VAR仅在main.c中声明，所以只有在main.c中包含
 本头文件时才会声明全局变量，在其他文件中包含本文件时，编译时，就会自动加
 上前缀extern，表明是main函数中声明的全局变量，该文件中直接使用。
 */
