//===================================================================== 
//文件名称：model_int.h 
//功能概要：模型初始化构件文件 
//版    本：V1.0 
//使用芯片：STM32 
//===================================================================== 
#ifndef MODEL_INIT_H 

#define MODEL_INIT_H 



#define SortNum 3

   const float Model_Conv1_W[1][6][5][5]; 

   const float Model_Conv1_B[6]; 

   const float Model_Conv2_W[6][10][5][5];  

   const float Model_Conv2_B[10]; 

    const float Model_Conv3_W[10][10][5][5]; 

   const float Model_Conv3_B[10]; 

   const float Model_F1_W[10][10]; 

   const float Model_F1_B[10]; 

   float Model_F2_W[10][SortNum]; 

   float Model_F2_B[SortNum]; 


#endif /* MODEL_INIT_H */ 
