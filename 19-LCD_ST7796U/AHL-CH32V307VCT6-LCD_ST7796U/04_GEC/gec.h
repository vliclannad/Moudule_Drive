//======================================================================
//�ļ����ƣ�gec.h��GECоƬ����ͷ�ļ���
//������λ���մ�arm��������(sumcu.suda.edu.cn)
//���¼�¼��2018.12-20200627
//======================================================================
#ifndef GEC_H    //��ֹ�ظ����壨GEC_H ��ʼ��
#define GEC_H
#include "flash.h"
#include "printf.h"
#include "user.h"

//���䶯��GEC������Ϣ==================================================
#define GEC_USER_SECTOR_START       (452)
#define GEC_COMPONENT_LST_START     (276)   //�����⺯���б�ʼ������
#define BIOS_SYSTICK_IRQn           (12+14)
#define BIOS_SW_IRQn                (14+14)
#define BIOS_UPDATE_IRQn            (53+14)   //BIOS����д�봮�ڵ��жϺ�

//���䶯����̬������ʼ��������������
#define GEC_DYNAMIC_START        (260)
#define GEC_DYNAMIC_END          (275)
//���̶��������⺯��ָ���ϵͳ���ܺ�������===========================================
void **  component_fun;

//void  Vectors_Init();
#endif  //��ֹ�ظ����壨GEC_H ��β��
