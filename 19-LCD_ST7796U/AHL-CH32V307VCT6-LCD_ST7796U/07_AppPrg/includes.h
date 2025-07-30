//======================================================================
//�ļ����ƣ�includes.h��Ӧ�ù�����ͷ�ļ���
//����ṩ��SD-Arm��sumcu.suda.edu.cn��
//�汾���£�20170801-20200223
//���������������õ��Ĺ������곣�����塢�Զ����������͡�����ȫ�ֱ�����
//��ֲ���򣺡��̶���
//======================================================================

#ifndef  INCLUDES_H     //��ֹ�ظ����壨INCLUDES_H ��ͷ��
#define  INCLUDES_H

//----------------------------------------------------------------------
//��1�������õ��Ĺ���
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
//��2��ȫ��ʹ�õĺ곣����
//�ڴ��޸�lcd��ʾ������Ӣ������
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
//��3���Զ�����������
#pragma  pack(push,1)
struct shakeData
{
    uint8_t uecomType[25];  //ͨ��ģ������
    uint8_t mcuType[20];    //MCU����
    uint32_t uStartAds;     //User������ʼ��ַ
    uint32_t uCodeSize;     //User�����ܴ����С
    uint32_t replaceNum;    //�滻��������ֽ�
    uint32_t reserveNum;    //������������ֽڣ�������0��ζ����User����
    uint8_t BIOSVersion[10];  //BIOS�汾
};
#pragma  pack(pop)
//----------------------------------------------------------------------

//��4������ȫ�ֱ����������淶��ע2��
//���������궨��ȫ�ֱ���ǰ׺G_VAR_PREFIX��ʵ��ԭ���ע3��
#ifdef GLOBLE_VAR              //GLOBLE_VAR��main.c�ļ����к궨��
  #define G_VAR_PREFIX         //ǰ׺G_VAR_PREFIX����Ϊ��
#else                          //GLOBLE_VAR�ڷ�main.c�ļ����޶���
  #define G_VAR_PREFIX extern  //ǰ׺G_VAR_PREFIX����Ϊ"extern"
#endif

//���ڴ�����ȫ�ֱ�����
G_VAR_PREFIX uint8_t  gFunFlag;        //���������ܱ�ʶ
G_VAR_PREFIX uint16_t gRecvdataLen;    //����1���յ��Ǹ������ݵ����ݳ���
G_VAR_PREFIX vuint16_t gcRecvLen;
G_VAR_PREFIX vuint16_t gcRecvDCLen;
G_VAR_PREFIX vuint8_t gcReccrc32[4];
G_VAR_PREFIX vuint8_t gcRecvBuf[4096];
G_VAR_PREFIX vuint8_t Rst_Count;  //������λ�����Ĵ���
G_VAR_PREFIX vuint8_t gcUpdateFlag[10];
G_VAR_PREFIX struct shakeData gShakeData;   //����֡����

G_VAR_PREFIX uint8_t gTimberFlag1s;
G_VAR_PREFIX uint8_t cam_ready;
G_VAR_PREFIX uint8_t cam_fifo_ready;
G_VAR_PREFIX uint8_t cam_refresh_end;
G_VAR_PREFIX uint8_t cam_rec_flag;
G_VAR_PREFIX uint8_t ov_sta;

//----------------------------------------------------------------------

#endif                  //��ֹ�ظ����壨INCLUDES_H ��β��


/*
 ֪ʶҪ�أ�
 ע1��ȫ��ʹ�õĺ곣����ʹ��ȫ��д
 ע2��ȫ�ֱ�������һ����g��ͷ�������ܱ�֤ȫ������Ψһ�ԣ�����һ��Ҫע��
      �京�壬������ʱ��׼��ֵ��
 ע3��ȫ�ֱ���һ�������ദʹ�ô������������Ϊ�˽��ȫ�ֱ����ظ�����
 �����⣬��GLOBLE_VAR����main.c������������ֻ����main.c�а���
 ��ͷ�ļ�ʱ�Ż�����ȫ�ֱ������������ļ��а������ļ�ʱ������ʱ���ͻ��Զ���
 ��ǰ׺extern��������main������������ȫ�ֱ��������ļ���ֱ��ʹ�á�
 */
