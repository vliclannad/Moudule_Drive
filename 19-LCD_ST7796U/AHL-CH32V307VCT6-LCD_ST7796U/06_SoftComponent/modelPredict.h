//=====================================================================
//�ļ����ƣ�model_Predict_Gary.h
//���ܸ�Ҫ��ģ����������Դ�ļ�
//������λ�����ݴ�ѧǶ��ʽϵͳ���������о���(sumcu.suda.edu.cn)
//��    ����  2020-08-09  V1.0
//����оƬ��STM32
//=====================================================================
//#ifndef MODELPREDICT_H_
//#define MODELPREDICT_H_


#include "includes.h"
#include "modelPreprocess.h"

// ��������ģ�͵�ͼ������С
typedef uint8_t image_28[28][28];

typedef uint8_t image_32[32][32];

typedef uint8_t image_3_32[3][32][32];

// ����ģ�ʹ�����ʹ�õ�����ͼ��ṹ��
typedef float Model_Conv1_Input[1][32][32];   //��һ����������

typedef float Model_Maxp1_Input[6][28][28];   //�ڶ���ػ�������

typedef float Model_Conv2_Input[6][14][14];   //��������������

typedef float Model_Maxp2_Input[10][10][10];  //���Ĳ�ػ�������

typedef float Model_Conv3_Input[10][5][5];    //�������������

typedef float Model_F1_Input[10][1][1];       //������ȫ���Ӳ�����

typedef float Model_F2_Input[20][SortNum];    //���߲�ȫ���Ӳ�����

typedef float Model_Output[SortNum];          //ģ�����


//=====================================================================
//��������:Model_SoftmaxOut
//��������:�ع������ָ��
//����˵��:input:�����Ԫ������ָ��
//���ܸ�Ҫ:���������softmax�ع�
//=====================================================================
float *Model_SoftmaxOut(float *input,int count);


//=====================================================================
//��������:Model_AverageOut
//��������:�ع������ָ�� 
//����˵��:input:�����Ԫ������ָ��
//���ܸ�Ҫ:������ֵ�㷨�ó�ÿ�������ʶ�����
//=====================================================================
float * Model_AverageOut(float *input,int count);

//=====================================================================
//��������:Model_PredictImage
//��������:�ع������ָ��
//����˵��:input:�����Ԫ������ָ��
//���ܸ�Ҫ:���������softmax�ع�
//=====================================================================
float *Model_PredictImage(image_32 input);


