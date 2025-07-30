//=====================================================================
//文件名称：model_Predict_Gray.c
//功能概要：模型驱动构件源文件
//制作单位：苏州大学嵌入式系统与物联网研究所(sumcu.suda.edu.cn)
//版    本：  2020-08-05  V1.0
//适用芯片：STM32
//=====================================================================

//包含的头文件
#include  "modelPredict.h"
 
//=====================================================================
//函数名称:GETLENGTH
//函数返回:数组长度
//参数说明:array:输入数组
//功能概要:计算数组元素个数
//=====================================================================
#define GETLENGTH(array) (sizeof(array)/sizeof(*(array)))

//=====================================================================
//函数名称:GETCOUNT
//函数返回:float数组元素个数
//参数说明:array:输入数组
//功能概要:计算float类型的数组个数
//=====================================================================
#define GETCOUNT(array)  (sizeof(array)/sizeof(float))

//=====================================================================
//函数名称:FOREACH
//函数返回:无
//参数说明:   i:临时计数变量
//        count:总循环次数
//功能概要:进行for循环
//=====================================================================
#define FOREACH(i,count) for (int i = 0; i < count; ++i)


//=====================================================================
//函数名称:CONVOLUTE_VALID
//函数返回:无
//参数说明:input:输入层元素数组首地址指针
//		  output:输出层元素数组首地址指针
//        weight:输入层与输出层之间权重数组指针
//功能概要:进行权重的赋值传播
//=====================================================================
#define CONVOLUTE_VALID(input,output,weight)													\
{																								\
	FOREACH(o0,GETLENGTH(output))																\
		FOREACH(o1,GETLENGTH(*(output)))														\
			FOREACH(w0,GETLENGTH(weight))														\
				FOREACH(w1,GETLENGTH(*(weight)))												\
					(output)[o0][o1] += (input)[o0 + w0][o1 + w1] * (weight)[w0][w1];			\
}


//=====================================================================
//函数名称:CONVOLUTE_FULL
//函数返回:
//参数说明:input:输入层元素数组首地址指针
//		  output:输出层元素数组首地址指针
//        weight:输入层与输出层之间偏置项数组指针
//功能概要:进行偏置项的赋值传播
//=====================================================================
#define CONVOLUTE_FULL(input,output,weight)														\
{																								\
	FOREACH(i0,GETLENGTH(input))																\
		FOREACH(i1,GETLENGTH(*(input)))														\
			FOREACH(w0,GETLENGTH(weight))														\
				FOREACH(w1,GETLENGTH(*(weight)))												\
					(output)[i0 + w0][i1 + w1] += (input)[i0][i1] * (weight)[w0][w1];			\
}

//=====================================================================
//函数名称:CONVOLUTION_FORWARD
//函数返回:
//参数说明:input:输入层元素数组首地址指针
//		  output:输出层元素数组首地址指针
//        weight:输入层与输出层之间权值数组指针
//          bias:输入层与输出层之间偏置数组指针
//        action:使用的激活函数指针
//功能概要:进行卷积前向转播
//=====================================================================
#define CONVOLUTION_FORWARD(input,output,weight,bias,action)									\
{																								\
	for (int x = 0; x < GETLENGTH(weight); ++x)												\
		for (int y = 0; y < GETLENGTH(*weight); ++y)											\
			CONVOLUTE_VALID(input[x], output[y], weight[x][y]);								\
	FOREACH(j, GETLENGTH(output))																\
		FOREACH(i, GETCOUNT(output[j]))														\
		((float *)output[j])[i] = action(((float *)output[j])[i] + bias[j]);					\
}

//=====================================================================
//函数名称:SUBSAMP_MAX_FORWARD
//函数返回:
//参数说明:input:输入层元素数组首地址指针
//		  output:输出层元素数组首地址指针
//功能概要:进行最大值池化操作
//=====================================================================
#define SUBSAMP_MAX_FORWARD(input,output)														\
{																								\
	const int len0 = GETLENGTH(*(input)) / GETLENGTH(*(output));								\
	const int len1 = GETLENGTH(**(input)) / GETLENGTH(**(output));							\
	FOREACH(i, GETLENGTH(output))																\
	FOREACH(o0, GETLENGTH(*(output)))															\
	FOREACH(o1, GETLENGTH(**(output)))														\
	{																							\
		int x0 = 0, x1 = 0, ismax;																\
		FOREACH(l0, len0)																		\
			FOREACH(l1, len1)																	\
		{																						\
			ismax = input[i][o0*len0 + l0][o1*len1 + l1] > input[i][o0*len0 + x0][o1*len1 + x1];\
			x0 += ismax * (l0 - x0);															\
			x1 += ismax * (l1 - x1);															\
		}																						\
		output[i][o0][o1] = input[i][o0*len0 + x0][o1*len1 + x1];								\
	}																							\
}

//=====================================================================
//函数名称:DOT_PRODUCT_FORWARD
//函数返回:
//参数说明:input:输入层元素数组首地址指针
//		  output:输出层元素数组首地址指针
//        weight:输入层与输出层之间权值数组指针
//          bias:输入层与输出层之间偏置数组指针
//        action:使用的激活函数指针
//功能概要;进行全连接前向传播
//=====================================================================
#define DOT_PRODUCT_FORWARD(input,output,weight,bias,action)									\
{																								\
	for (int x = 0; x < GETLENGTH(weight); ++x)												\
		for (int y = 0; y < GETLENGTH(*weight); ++y)											\
			((float *)output)[y] += ((float *)input)[x] * weight[x][y];							\
	FOREACH(j, GETLENGTH(bias))																\
		((float *)output)[j] = action(((float *)output)[j] + bias[j]);							\
}

//=====================================================================
//函数名称:Sqrt
//函数返回:输入值的开平方根
//参数说明:number:输入值
//功能概要:求开平方根函数
//=====================================================================
float Sqrt(float number)
{
	float new_guess;
	float last_guess;
	if(number < 0)
	{
		return -1;
	}
	if(number == 0)
	{
		return 0;
	}
	new_guess = 1;
	do
	{
		last_guess = new_guess;
		new_guess = (last_guess+number/last_guess)/2;

	}while(new_guess != last_guess);

	return new_guess;
}

//=====================================================================
//函数名称:Exp
//函数返回:自然指数值
//参数说明:x:输入值
//功能概要:求以自然数e为底，输入值为指数的值
//=====================================================================
float Exp(float x)
{
	int i,k,m,t;
	int xm = (int )x;
	float sum;
	float e;
	float ef;
	float z;
	float sub = x-xm;
	m=1;
	e=1.0;
	ef = 1.0;
	t = 10;
	z=1;
	sum = 1;
	if(xm<0)
	{
		xm=(-xm);
		for(k=0;k<xm;k++)
		{
			ef*=2.718281;
		}
		e/=ef;
	}
	else
	{
		for(k=0;k<xm;k++)
		{
			e*=2.718281;
		}
	}
	for(i=1;i<t;i++)
	{
		m*=i;
		z*=sub;
		sum+=z/m;
	}
	return sum*e;
}


//=====================================================================
//函数名称:relu
//函数返回:激活值
//参数说明:x:待激活值
//功能概要:
//=====================================================================
float Relu(float x)
{
	return x*(x > 0);
}

//=====================================================================
//函数名称:relugrad
//函数返回:
//参数说明:y:输入值
//功能概要:判断输入值是否正负
//=====================================================================
float relugrad(float y)
{
	return y > 0;
}

//=====================================================================
//函数名称:Model_SoftmaxOut
//函数返回:回归后数组指针
//参数说明:input:输入层元素数组指针
//功能概要:将数组进行softmax回归
//=====================================================================
float * Model_SoftmaxOut(float *input,int count)
{
	float exparr[count];
	float expsum=0;
	int i;
	float temp;


	for(i=0;i<count;i++)
	{
		temp=input[i];
		exparr[i]=Exp(temp);
		expsum+=exparr[i];
	}
	
	
	for(i=0;i<count;i++)
	{
		input[i]=exparr[i]/expsum;	

	}

	return input;
}

//=====================================================================
//函数名称:Model_AverageOut
//函数返回:回归后数组指针
//参数说明:input:输入层元素数组指针
//功能概要:将数组进行softmax回归
//=====================================================================
float * Model_AverageOut(float *input,int count)
{
	int i=0;
	float sum=0;

	for(i=0;i<count;i++)
	{
		sum+=input[i];
	}
	
	
	for(i=0;i<count;i++)
	{
		input[i]=input[i]/sum;	

	}

	return input;
}

//=====================================================================
//函数名称:Model_PredictImage
//函数返回:回归后数组指针
//参数说明:input:输入层元素数组指针
//功能概要:将数组进行softmax回归
//=====================================================================
float *Model_PredictImage(image_32 input)
{
	int h,w;
	
	//定义模型传播用的特征图像数组
	Model_Conv1_Input model_input={0};
	
	Model_Maxp1_Input mp_input={0};
	
	Model_Conv2_Input c2_input={0};
		
	Model_Maxp2_Input mp2_input={0};
	
	Model_Conv3_Input c3_input={0};
	
	Model_F1_Input f1_input={0};
	
	Model_F2_Input f2_input={0};
		
	Model_Output result_ouput={0};
	
	float *output;
	
	//(1)将图像数据载入模型的输入层
	for(h=0;h<32;h++)
	{
		for(w=0;w<32;w++)
		{	
			if(w>=2||w<30||h>=2||h<30)
			{
				model_input[0][h][w]=input[h][w];
				
			}
			else
			{
				model_input[0][h][w]=0;
			}
		}
		
	}
	//wdog_feed();				//喂狗	
	
	//(2)进行模型前向传播
	
	//(2.1)第一层卷积层传播
	CONVOLUTION_FORWARD(model_input, mp_input, Model_Conv1_W, Model_Conv1_B, Relu);//conv1的过度
	
	//(2.2)第二层池化层传播
	SUBSAMP_MAX_FORWARD(mp_input, c2_input);

	//(2.3)第三层卷积层传播
	CONVOLUTION_FORWARD(c2_input, mp2_input,Model_Conv2_W, Model_Conv2_B, Relu);//conv1的过度
	
	//(2.4)第四层池化层传播
	SUBSAMP_MAX_FORWARD(mp2_input, c3_input);
	
	//(2.5)第五层卷积层的传播
	CONVOLUTION_FORWARD(c3_input, f1_input,Model_Conv3_W, Model_Conv3_B, Relu);//conv1的过度
	
	//(2.6)第六层全连接层传播
	DOT_PRODUCT_FORWARD(f1_input, f2_input, Model_F1_W, Model_F1_B, Relu);
	
	//(2.7)第七层全连接输出层传播
	DOT_PRODUCT_FORWARD(f2_input, result_ouput, Model_F2_W, Model_F2_B, Relu);
		
	//wdog_feed();				//喂狗
	
	
	//(3)对输出数组进行softmax回归
	output=result_ouput;
	
	Model_SoftmaxOut(output,3);	
	

	//(4)返回输出激活数组
	return output;
}