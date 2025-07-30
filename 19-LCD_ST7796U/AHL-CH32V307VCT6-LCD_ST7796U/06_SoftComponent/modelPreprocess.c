//=====================================================================
//文件名称：modelPreprocess.c
//功能概要：模型驱动构件源文件
//制作单位：苏州大学嵌入式系统与物联网研究所(sumcu.suda.edu.cn)
//版    本：  2020-07-19  V1.0
//适用芯片：STM32
//=====================================================================

//包含的头文件
#include "modelPreprocess.h"
//=================内部调用函数声明=====================================
void Inter_Linear_gray(uint8_t **src,uint8_t **dst,int src_rows,
int src_cols,int dst_rows,int dst_cols);
void Inter_Linear_color(uint8_t src[][56][56],uint8_t dst[][28][28]);
void otsu_binarization(uint8_t *image, uint8_t height, uint8_t width);
void Morphology_Dilate(uint8_t* image, uint8_t height, uint8_t width, uint8_t dil_time);
//=====================================================================
//函数名称：rgb565togray_256
//函数返回：gray：转换后的灰阶
//参数说明：color：转换前的16位RGB565格式像素点
//功能概要：将RGB565格式转换为lcd可以显示的256阶的灰度格式。
//=====================================================================
uint8_t rgb565togray_256 (uint16_t color)
{
	uint8_t red,green,blue;
	uint8_t gray;

	//(1)取出每个颜色的值
	red = (color & 0xF800) >> 8;
	green = (color & 0x07E0) >> 3; 
	blue = (color & 0x001F) << 3;
	
	//(2)灰度算法
	gray =(red*77+green*150+blue*29+128)/256;//转换成256的灰阶
			
	return gray;
}

//=====================================================================
//函数名称:Model_GetInputImg_Gray
//函数返回:处理成功返回1
//参数说明:	 image:56*56大小的16位彩色图像
//		   output:推理模型的输入8位灰度图像
//功能概要:灰度图像下采样
//=====================================================================
int Model_GetInputImg_Gray(uint16_t* image,image_32 output)
{
	//(1)初始化局部变量
	//(1.1)初始化压缩参数
	uint8_t ci,cj;
	uint8_t i,j;
	uint8_t array[56][56];
	int count=0;
	//(1.2)初始化模型推理相关参数
	uint8_t image_pred[28][28]={0};//8位灰度数组
	//(2)进行裁剪并压缩
	
	//(2.1)转换成二维灰度数组
	for(ci=0;ci<56;ci++)
	{
		for(cj=0;cj<56;cj++)
		{
			array[ci][cj]=rgb565togray_256(image[count++]);
			
		}
	}
	//（2.2）下采样至28*28
	Inter_Linear_gray((uint8_t **)array,(uint8_t **)image_pred,56,56,28,28);
	//（2.3）大津法二值化
	otsu_binarization((uint8_t *)image_pred, 28, 28);
	//（2.4）膨胀
	//经过二值化和膨胀后图像目标可以在亮度较低的环境下也能更好的识别
	Morphology_Dilate((uint8_t *)image_pred, 28, 28, 1);
	//(3)赋值输出图像矩阵
	for(i=2;i<30;i++)
	{
		for(j=2;j<30;j++)
		{		
			output[i][j]=image_pred[i-2][j-2];
		}
	} 
		
	return 1;//提取成功
}

//=====================================================================
//函数名称:Model_GetInputImg_Color
//函数返回:输出激活值数组指针
//参数说明:image:56*56大小的16位RGB一维图像数组
//		   output:推理模型的输入三维彩色数组图像
//功能概要:彩色图像下采样
//=====================================================================
int Model_GetInputImg_Color(uint16_t* image,image_3_32 output)
{
	//(1)初始化局部变量
	//(1.1)初始化池化压缩参数
	uint8_t ci,cj;
	uint8_t i,j;
	uint8_t array[3][56][56];
	int count=0;
	//(1.2)初始化模型推理相关参数
	uint8_t image16_pred[3][28][28]={0};//16位RGB处理数组
	//(2)进行裁剪并压缩
	
	//(2.1)转换成三维RGB数组
    for(ci=0;ci<56;ci++)
	{
		for(cj=0;cj<56;cj++)
		{
			array[0][ci][cj]=(image[count]>> 11)& 0x001F; //取红色;
			array[1][ci][cj]=(image[count]>> 5)& 0x003F; //取绿色
			array[2][ci][cj]=(image[count]) & 0x001F; //取蓝色
			count++;
		}
	}
	//（2.2）下采样至28*28
	Inter_Linear_color(array,image16_pred);
    //(3)赋值输出图像矩阵
	for(i=2;i<30;i++)
	{
		for(j=2;j<30;j++)
		{	
			output[0][i][j] = image16_pred[0][i-2][j-2];
			output[1][i][j] = image16_pred[1][i-2][j-2];
			output[2][i][j] = image16_pred[2][i-2][j-2];
		}
	}

	return 1;//提取成功
}

//----------------------以下为内部函数存放处----------------------------
//=====================================================================
//函数名称:Inter_Linear_gray
//函数返回:无
//参数说明:src:原灰度图像数组    dst:目标灰度图像数组
//		   src_rows：原灰度图像数组行    src_cols：原灰度图像数组列
//         dst_rows：目标灰度图像数组列    dst_cols：目标灰度图像数组列
//功能概要:灰度图像下采样
//=====================================================================
void Inter_Linear_gray(uint8_t **src,uint8_t **dst,int src_rows,int src_cols,
                        int dst_rows,int dst_cols)
{
	for (int i=0;i<dst_rows;i++)
	{
		double index_i=(i+0.5)*(src_rows/dst_rows)-0.5;     //几何中心对齐
		//防止越界
		if(index_i<0) index_i=0;
		if(index_i>=src_rows-1) index_i=src_rows-1;
		//相邻4*4像素的行（坐标）
		int i1=(int)index_i;                               //向下取整
		int i2=(int)(index_i+1);                           //向上取整
		double u=index_i - i1;                            //u为得到浮点型坐标行的小数部分
        for(int j=0;j<dst_cols;j++)
        {
        	double index_j=(j+0.5)*(src_cols/dst_cols)-0.5;     //几何中心对齐
        	//防止越界
			if(index_j<0) index_j=0;
			if(index_j>=src_cols-1) index_j=src_cols-1;
			//相邻4*4像素的行（坐标）
			int j1=(int)index_j;                               //向下取整
			int j2=(int)(index_j+1);                           //向上取整
			double v=index_j-j1;                             //v为得到浮点型坐标行的小数部分
			*((uint8_t*)dst+dst_cols*i+j)=255-(uint8_t)((1-u)*(1-v)*(*((uint8_t*)src+src_cols*i1+j1))+
			(1-u)*v*(*((uint8_t*)src+src_cols*i1+j2))+u*(1-v)*(*((uint8_t*)src+src_cols*i2+j1))+
			u*v*(*((uint8_t*)src+src_cols*i2+j2)));
			
        }
		
	}
}

//=====================================================================
//函数名称:Inter_Linear_color
//函数返回:无
//参数说明:src:原彩色图像数组    dst:目标彩色图像数组
//功能概要:彩色图像下采样
//=====================================================================
void Inter_Linear_color(uint8_t src[][56][56],uint8_t dst[][28][28])
{
	for (int i=0;i<28;i++)
	{
		double index_i=(i+0.5)*2-0.5;     //几何中心对齐
		//防止越界
		if(index_i<0) index_i=0;
		if(index_i>=56-1) index_i=56-1;
		//相邻4*4像素的行（坐标）
		int i1=(int)index_i;                               //向下取整
		int i2=(int)(index_i+1);                           //向上取整
		double u=index_i - i1;                            //u为得到浮点型坐标行的小数部分
        for(int j=0;j<28;j++)
        {
        	double index_j=(j+0.5)*2-0.5;     //几何中心对齐
        	//防止越界
			if(index_j<0) index_j=0;
			if(index_j>=55) index_j=55;
			//相邻4*4像素的行（坐标）
			int j1=(int)index_j;                               //向下取整
			int j2=(int)(index_j+1);                           //向上取整
			double v=index_j-j1;                             //v为得到浮点型坐标行的小数部分
			dst[0][i][j]=(1-u)*(1-v)*src[0][i1][j1]+(1-u)*v*src[0][i1][j2]+u*(1-v)*src[0][i2][j1]+u*v*src[0][i2][j2];
			dst[1][i][j]=(1-u)*(1-v)*src[1][i1][j1]+(1-u)*v*src[1][i1][j2]+u*(1-v)*src[1][i2][j1]+u*v*src[1][i2][j2];
			dst[2][i][j]=(1-u)*(1-v)*src[2][i1][j1]+(1-u)*v*src[2][i1][j2]+u*(1-v)*src[2][i2][j1]+u*v*src[2][i2][j2];
        }
		
	}
}


////=====================================================================
////函数名称：Is_2mPic
////函数返回：判断真假值
////参数说明: TestGram:待测试灰度图像矩阵
////功能概要：判断图像分布是否可以
////=====================================================================
//uint8_t Is_2mPic(float *TestGram)
//{
// 	uint8_t m_cnt = 0;
// 	uint8_t i;
// 	
//    for (i = 1; i < 255; i++)
//    {
//    	if (TestGram[i - 1] < TestGram[i] && TestGram[i + 1] < TestGram[i])
//        {
//            m_cnt++; 
//        }
//     }
//    
//    if (m_cnt == 2)
//    {
//        return 1;
//    }
//    else
//    {
//        return 0;
//	}
//}
//
////=====================================================================
////函数名称：ArrayCopy
////函数返回：无
////参数说明：dest：复制后存放的数组；source：被复制的数组；len:复制的长度
////功能概要：从源数组复制指定长度的内容到目标数组
////=====================================================================
//void ArrayCopy(uint8_t * dest,uint8_t*source,uint16_t len)
//{
//	uint16_t i = 0;
//	for(i=0;i<len;i++)
//		dest[i]=source[i];
//}
//
//
////=====================================================================
////函数名称：Pic_Threshold_2m
////函数返回：无
////参数说明: image:28*28大小的二维灰度图像矩阵
////			    h:输入图像高度
////				w:输入图像宽度
////功能概要：将图像通过双峰法均值法进行阈值滤波，找并通过串口发送
////=====================================================================
//int  Model_PicThreshold_2m(image_28 img2d,int h,int w)
//{
//	uint16_t i,j;//打印图像用
//	float Gram_2m[256]={0};//灰度频率数组
//	float Gram_2m_1[256]={0};//灰度频率数组1
//	uint16_t arr_temp;
//	uint16_t cnt=0;
//	uint16_t temp_ts=0;
//
//	//(1)统计输入数组的所有灰度值 处理成一维数组大小为256
//	//   第i个数组成员的值j代表着灰度为i的像素点有j个
//	for(i=0;i<h;i++)
//	{
//		for(j=0;j<w;j++)
//		{
//			arr_temp=img2d[i][j];
//			Gram_2m[arr_temp]++;
//			Gram_2m_1[arr_temp]++;
//		}
//	}
//
//	//(2)处理数组 将数组不断平滑成峰值 最终为只有两个峰值的统计数组
//	while(Is_2mPic(Gram_2m)==0)
//	{
//		Gram_2m[0] = (Gram_2m_1[0] + Gram_2m_1[0] + Gram_2m_1[1]) / 3;
//		for (i = 1; i < 255; i++)
//		{
//			Gram_2m[i] = (Gram_2m_1[i-1]+Gram_2m_1[i] + Gram_2m_1[i+1])/3;
//		}
//
//		Gram_2m[255] = (Gram_2m_1[254] + Gram_2m_1[255] + Gram_2m_1[255]) / 3;
//
//		ArrayCopy((uint8_t * )Gram_2m_1,(uint8_t * )Gram_2m,256 * sizeof(float));
//
//		cnt++;
//
//		if(cnt%50==0)
//		{
//			
//			Is_2mPic(Gram_2m);//判断是否只有两个局部最大值
//		}
//
//		if(cnt==1000)
//		{
//			return 0;
//		}
//
//	}
//
//	//(3)得到两个局部最大值的灰度值取二者均值为滤波阈值
//    for (i=1;i<255;i++)
//    {
//        if (Gram_2m[i-1] < Gram_2m[i] && Gram_2m[i+1]<Gram_2m[i])
//        {
//        	temp_ts=temp_ts+i-1;
//		}
//	}
//
//	temp_ts = temp_ts/2;
//
//	//(4)将背景像素值置0
//	for(i=0;i<h;i++)
//	{
//		for(j=0;j<w;j++)
//		{
//			if(img2d[i][j]<temp_ts)
//			{
//				img2d[i][j]=25;
//			}
//			else
//			{
//				img2d[i][j]=195;
//			}
//		}
//
//	}
//
//	return 1;
//
//}
//=====================================================================
//函数名称:otsu_binarization
//函数返回:无
//参数说明: image:28*28大小的灰度图像
//功能概要:大津法二值化
//=====================================================================
void otsu_binarization(uint8_t *image, uint8_t height, uint8_t width)
{
	uint8_t _t = 0, i = 0, j = 0;
	//分别是前景的长度，前景的像素平均值，前景的比例，后面是背景的
	uint16_t v0_len = 0;
	float v0_mean = 0.0;
	float v0_w = 0.0;
	uint16_t v1_len = 0;
	float v1_mean = 0.0;
	float v1_w = 0.0;
	//max_sigma，max_t分别是最大类间方差，最大阈值
	float max_sigma = 0.0;
	float sigma = 0.0;
	uint8_t max_t = 0;
	for(_t = 1; _t < 255; _t++){
		//重置数值 
		v0_len = 0, v0_mean = 0.0, v0_w = 0.0;
		v1_len = 0, v1_mean = 0.0, v1_w = 0.0;
		for(i = 0; i < height; i++){
			for(j = 0; j < width; j++){
				//像素点
				uint8_t image_dot = *((uint8_t*)image+width*i+j);
				if(image_dot < _t){
					//前景
					v0_len++;
					v0_mean+=image_dot;
				}else{
					//背景
					v1_len++;
					v1_mean+=image_dot;
				}
			}
		}
		//得到前景的比例，平均值
		if(v0_len > 0){
			v0_mean = v0_mean / (float)v0_len;
		}else{
			v0_mean = 0.0;
		}
		v0_w = (float)v0_len / (float)(height * width);
		//得到背景的比例，平均值
		if(v1_len > 0){
			v1_mean = v1_mean / (float)v1_len;
		}else{
			v1_mean = 0.0;
		}
		v1_w = 1 - v0_w;
		//得到类间方差
		sigma = v0_w * v1_w * ((v0_mean - v1_mean) * (v0_mean - v1_mean));
		if(sigma > max_sigma){
			//更新
			max_sigma = sigma;
			max_t = _t;
		}
//		printf("v0_len=%d, v0_mean=%f, v0_w=%f, v1_len=%d, v1_mean=%f, v1_w=%f\r\n",
//		      v0_len, v0_mean, v0_w, v1_len, v1_mean, v1_w);
	}
//	printf("max_sigma = %d, max_t = %d\r\n", max_sigma, max_t);
	//二值化
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			if(*((uint8_t*)image+width*i+j) < max_t){
				*((uint8_t*)image+width*i+j) = 0;
			}else{
				*((uint8_t*)image+width*i+j) = 255;
			}
		}
	}
	return;
}

//=====================================================================
//函数名称:Morphology_Dilate
//函数返回:无
//参数说明: image:28*28大小的灰度图像
//功能概要:膨胀图像中的目标对象
//=====================================================================
void Morphology_Dilate(uint8_t* image, uint8_t height, uint8_t width, uint8_t dil_time)
{
	//用于膨胀的卷积核 
	uint8_t kernel[3][3]={{0, 1, 0},{1, 0, 1},{0, 1, 0}};
	uint8_t image_pad[30][30]={0};
	uint16_t kernel_sum=0;
	for(uint8_t i = 0;i<height;i++){
		for(uint8_t j = 0;j<width;j++){
			image_pad[i+1][j+1] = *(image + i*width + j);
		}
	}
	for(uint8_t t = 0;t<dil_time;t++){
		for(uint8_t i = 1;i<height+1;i++){
			for(uint8_t j = 1;j<width+1;j++){
				//进行卷积运算 
				kernel_sum = image_pad[i-1][j-1]*kernel[0][0];
				kernel_sum += image_pad[i-1][j]*kernel[0][1];
				kernel_sum += image_pad[i-1][j+1]*kernel[0][2];
				kernel_sum += image_pad[i][j-1]*kernel[1][0];
				kernel_sum += image_pad[i][j]*kernel[1][1];
				kernel_sum += image_pad[i][j+1]*kernel[1][2];
				kernel_sum += image_pad[i+1][j-1]*kernel[2][0];
				kernel_sum += image_pad[i+1][j]*kernel[2][1];
				kernel_sum += image_pad[i+1][j+1]*kernel[2][2];
				//只要卷积结果超过255，就将对应位置赋值为255
				if(kernel_sum >= 255){
					*(image + (i-1)*width + (j-1)) = 255;
				}	
			}
		}	
	}
	return;
}