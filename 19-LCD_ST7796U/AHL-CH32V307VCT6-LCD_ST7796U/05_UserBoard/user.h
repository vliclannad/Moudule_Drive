//======================================================================
//�ļ����ƣ�user.h��userͷ�ļ���
//������λ��SD-Arm(sumcu.suda.edu.cn)
//���¼�¼��20200831-20200903
//ʹ��˵�����û�ʹ�õ���оƬ�����ڱ��ļ��н��к궨�壬�Ա�06��07�ļ��е�
//          �������㲻ͬоƬ�Ŀ���ֲ��
//======================================================================

#ifndef __USER_H
#define __USER_H

//��1���ļ�����
#include "emuart.h"
#include "gpio.h"
#include "flash.h"
#include "uart.h"
#include "adc.h"
#include "wdog.h"
#include "gec.h"
#include "printf.h"
#include "math.h"
#include "Temp_Convert.h"

//��2���곣������
//Ϊ��06��07�ļ��пɸ��ã�main.c��isr.h���õ��ĺ곣�������ﶨ��


//��3��Ӳ�����ż�ģ�����ƺ궨��
//Ϊ��06��07�ļ��пɸ��ã�Ӳ�����ż�ģ��������������û��ӽǽ��к궨��
//ָʾ�����Ŷ���
#define  LIGHT_RED     (PTC_NUM|0)   //���
#define  LIGHT_GREEN   (PTC_NUM|1)   //�̵�
#define  LIGHT_BLUE    (PTC_NUM|2)   //����
#define  LIGHT_ON    0   //��������״̬�궨�壬��Ӳ���ӷ�������
#define  LIGHT_OFF   1   //�ư�
//�û�����UART_User����
#define UART_User    UART_2  //�û����ڣ���-GND;��-TX;��-RX��
//ADCģ�鶨��
#define AD_MCU_TEMP   16	//�ڲ��¶Ȳɼ�ͨ��
#define AD_TEMP       14	//�����¶Ȳɼ�ͨ��14((PTC_NUM|4))
//���������Ŷ���
#define  GPIO_TSI      (PTD_NUM|2)  	 //����GPIO_Tsi����

//��4���û��жϷ�����������������ȱʡ�жϷ������̵Ĺ���
//�û�����UART_User���жϷ�������
extern void UART_User_Handler(void);
#define UART2_Handler UART_User_Handler  //UART_User_Handler��isr.c��ʵ��
extern void Cam_User_Handler(void);
#define Cam_Handler    Cam_User_Handler   //�û�����ͷ��ȡ��־�жϺ���
#define TIMER_USER           TIMER7             //�û���ʱ��TIMER
extern void TIMER_USER_Handler(void);
#define TIMER_Handler   TIMER_USER_Handler    //�û���ʱ���жϺ���






#endif 
