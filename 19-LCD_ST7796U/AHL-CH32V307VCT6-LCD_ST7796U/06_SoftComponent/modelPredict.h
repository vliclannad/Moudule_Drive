//=====================================================================
//文件名称：model_Predict_Gary.h
//功能概要：模型驱动构件源文件
//制作单位：苏州大学嵌入式系统与物联网研究所(sumcu.suda.edu.cn)
//版    本：  2020-08-09  V1.0
//适用芯片：STM32
//=====================================================================
//#ifndef MODELPREDICT_H_
//#define MODELPREDICT_H_


#include "includes.h"
#include "modelPreprocess.h"

// 定义输入模型的图像矩阵大小
typedef uint8_t image_28[28][28];

typedef uint8_t image_32[32][32];

typedef uint8_t image_3_32[3][32][32];

// 定义模型传播所使用的特征图像结构体
typedef float Model_Conv1_Input[1][32][32];   //第一层卷积层输入

typedef float Model_Maxp1_Input[6][28][28];   //第二层池化层输入

typedef float Model_Conv2_Input[6][14][14];   //第三层卷积层输入

typedef float Model_Maxp2_Input[10][10][10];  //第四层池化层输入

typedef float Model_Conv3_Input[10][5][5];    //第五层卷积层输入

typedef float Model_F1_Input[10][1][1];       //第六层全连接层输入

typedef float Model_F2_Input[20][SortNum];    //第七层全连接层输入

typedef float Model_Output[SortNum];          //模型输出


//=====================================================================
//函数名称:Model_SoftmaxOut
//函数返回:回归后数组指针
//参数说明:input:输入层元素数组指针
//功能概要:将数组进行softmax回归
//=====================================================================
float *Model_SoftmaxOut(float *input,int count);


//=====================================================================
//函数名称:Model_AverageOut
//函数返回:回归后数组指针 
//参数说明:input:输入层元素数组指针
//功能概要:经过均值算法得出每个物体的识别概率
//=====================================================================
float * Model_AverageOut(float *input,int count);

//=====================================================================
//函数名称:Model_PredictImage
//函数返回:回归后数组指针
//参数说明:input:输入层元素数组指针
//功能概要:将数组进行softmax回归
//=====================================================================
float *Model_PredictImage(image_32 input);


