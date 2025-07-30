//=====================================================================
//�ļ����ƣ�model_Predict_Gray.c
//���ܸ�Ҫ��ģ����������Դ�ļ�
//������λ�����ݴ�ѧǶ��ʽϵͳ���������о���(sumcu.suda.edu.cn)
//��    ����  2020-08-05  V1.0
//����оƬ��STM32
//=====================================================================

//������ͷ�ļ�
#include  "modelPredict.h"
 
//=====================================================================
//��������:GETLENGTH
//��������:���鳤��
//����˵��:array:��������
//���ܸ�Ҫ:��������Ԫ�ظ���
//=====================================================================
#define GETLENGTH(array) (sizeof(array)/sizeof(*(array)))

//=====================================================================
//��������:GETCOUNT
//��������:float����Ԫ�ظ���
//����˵��:array:��������
//���ܸ�Ҫ:����float���͵��������
//=====================================================================
#define GETCOUNT(array)  (sizeof(array)/sizeof(float))

//=====================================================================
//��������:FOREACH
//��������:��
//����˵��:   i:��ʱ��������
//        count:��ѭ������
//���ܸ�Ҫ:����forѭ��
//=====================================================================
#define FOREACH(i,count) for (int i = 0; i < count; ++i)


//=====================================================================
//��������:CONVOLUTE_VALID
//��������:��
//����˵��:input:�����Ԫ�������׵�ַָ��
//		  output:�����Ԫ�������׵�ַָ��
//        weight:������������֮��Ȩ������ָ��
//���ܸ�Ҫ:����Ȩ�صĸ�ֵ����
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
//��������:CONVOLUTE_FULL
//��������:
//����˵��:input:�����Ԫ�������׵�ַָ��
//		  output:�����Ԫ�������׵�ַָ��
//        weight:������������֮��ƫ��������ָ��
//���ܸ�Ҫ:����ƫ����ĸ�ֵ����
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
//��������:CONVOLUTION_FORWARD
//��������:
//����˵��:input:�����Ԫ�������׵�ַָ��
//		  output:�����Ԫ�������׵�ַָ��
//        weight:������������֮��Ȩֵ����ָ��
//          bias:������������֮��ƫ������ָ��
//        action:ʹ�õļ����ָ��
//���ܸ�Ҫ:���о��ǰ��ת��
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
//��������:SUBSAMP_MAX_FORWARD
//��������:
//����˵��:input:�����Ԫ�������׵�ַָ��
//		  output:�����Ԫ�������׵�ַָ��
//���ܸ�Ҫ:�������ֵ�ػ�����
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
//��������:DOT_PRODUCT_FORWARD
//��������:
//����˵��:input:�����Ԫ�������׵�ַָ��
//		  output:�����Ԫ�������׵�ַָ��
//        weight:������������֮��Ȩֵ����ָ��
//          bias:������������֮��ƫ������ָ��
//        action:ʹ�õļ����ָ��
//���ܸ�Ҫ;����ȫ����ǰ�򴫲�
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
//��������:Sqrt
//��������:����ֵ�Ŀ�ƽ����
//����˵��:number:����ֵ
//���ܸ�Ҫ:��ƽ��������
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
//��������:Exp
//��������:��Ȼָ��ֵ
//����˵��:x:����ֵ
//���ܸ�Ҫ:������Ȼ��eΪ�ף�����ֵΪָ����ֵ
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
//��������:relu
//��������:����ֵ
//����˵��:x:������ֵ
//���ܸ�Ҫ:
//=====================================================================
float Relu(float x)
{
	return x*(x > 0);
}

//=====================================================================
//��������:relugrad
//��������:
//����˵��:y:����ֵ
//���ܸ�Ҫ:�ж�����ֵ�Ƿ�����
//=====================================================================
float relugrad(float y)
{
	return y > 0;
}

//=====================================================================
//��������:Model_SoftmaxOut
//��������:�ع������ָ��
//����˵��:input:�����Ԫ������ָ��
//���ܸ�Ҫ:���������softmax�ع�
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
//��������:Model_AverageOut
//��������:�ع������ָ��
//����˵��:input:�����Ԫ������ָ��
//���ܸ�Ҫ:���������softmax�ع�
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
//��������:Model_PredictImage
//��������:�ع������ָ��
//����˵��:input:�����Ԫ������ָ��
//���ܸ�Ҫ:���������softmax�ع�
//=====================================================================
float *Model_PredictImage(image_32 input)
{
	int h,w;
	
	//����ģ�ʹ����õ�����ͼ������
	Model_Conv1_Input model_input={0};
	
	Model_Maxp1_Input mp_input={0};
	
	Model_Conv2_Input c2_input={0};
		
	Model_Maxp2_Input mp2_input={0};
	
	Model_Conv3_Input c3_input={0};
	
	Model_F1_Input f1_input={0};
	
	Model_F2_Input f2_input={0};
		
	Model_Output result_ouput={0};
	
	float *output;
	
	//(1)��ͼ����������ģ�͵������
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
	//wdog_feed();				//ι��	
	
	//(2)����ģ��ǰ�򴫲�
	
	//(2.1)��һ�����㴫��
	CONVOLUTION_FORWARD(model_input, mp_input, Model_Conv1_W, Model_Conv1_B, Relu);//conv1�Ĺ���
	
	//(2.2)�ڶ���ػ��㴫��
	SUBSAMP_MAX_FORWARD(mp_input, c2_input);

	//(2.3)���������㴫��
	CONVOLUTION_FORWARD(c2_input, mp2_input,Model_Conv2_W, Model_Conv2_B, Relu);//conv1�Ĺ���
	
	//(2.4)���Ĳ�ػ��㴫��
	SUBSAMP_MAX_FORWARD(mp2_input, c3_input);
	
	//(2.5)���������Ĵ���
	CONVOLUTION_FORWARD(c3_input, f1_input,Model_Conv3_W, Model_Conv3_B, Relu);//conv1�Ĺ���
	
	//(2.6)������ȫ���Ӳ㴫��
	DOT_PRODUCT_FORWARD(f1_input, f2_input, Model_F1_W, Model_F1_B, Relu);
	
	//(2.7)���߲�ȫ��������㴫��
	DOT_PRODUCT_FORWARD(f2_input, result_ouput, Model_F2_W, Model_F2_B, Relu);
		
	//wdog_feed();				//ι��
	
	
	//(3)������������softmax�ع�
	output=result_ouput;
	
	Model_SoftmaxOut(output,3);	
	

	//(4)���������������
	return output;
}