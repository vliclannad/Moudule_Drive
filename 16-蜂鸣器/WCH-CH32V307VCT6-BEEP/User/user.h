#ifndef _USER__H
#define _USER__H

#include "debug.h"

//�˴����ȫ�ֱ���
#ifdef GLOBLE_VAR              //GLOBLE_VAR��main.c�ļ����к궨��
  #define G_VAR_PREFIX         //ǰ׺G_VAR_PREFIX����Ϊ��
#else                          //GLOBLE_VAR�ڷ�main.c�ļ����޶���
  #define G_VAR_PREFIX extern  //ǰ׺G_VAR_PREFIX����Ϊ"extern"
#endif

G_VAR_PREFIX uint16_t gCS100ACount; //����������������
G_VAR_PREFIX float    gCS100A_Distance;//����������������
G_VAR_PREFIX uint8_t  gOD_flag;





#endif
