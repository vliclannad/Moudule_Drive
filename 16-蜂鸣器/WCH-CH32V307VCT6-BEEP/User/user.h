#ifndef _USER__H
#define _USER__H

#include "debug.h"

//此处添加全局变量
#ifdef GLOBLE_VAR              //GLOBLE_VAR在main.c文件中有宏定义
  #define G_VAR_PREFIX         //前缀G_VAR_PREFIX定义为空
#else                          //GLOBLE_VAR在非main.c文件中无定义
  #define G_VAR_PREFIX extern  //前缀G_VAR_PREFIX定义为"extern"
#endif

G_VAR_PREFIX uint16_t gCS100ACount; //超声波传感器计数
G_VAR_PREFIX float    gCS100A_Distance;//超声波传感器距离
G_VAR_PREFIX uint8_t  gOD_flag;





#endif
