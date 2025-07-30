//=====================================================================
//�ļ����ƣ�modelPreprocess.c
//���ܸ�Ҫ��ģ����������Դ�ļ�
//������λ�����ݴ�ѧǶ��ʽϵͳ���������о���(sumcu.suda.edu.cn)
//��    ����  2020-07-19  V1.0
//����оƬ��STM32
//=====================================================================

//������ͷ�ļ�
#include "modelPreprocess.h"
//=================�ڲ����ú�������=====================================
void Inter_Linear_gray(uint8_t **src,uint8_t **dst,int src_rows,
int src_cols,int dst_rows,int dst_cols);
void Inter_Linear_color(uint8_t src[][56][56],uint8_t dst[][28][28]);
void otsu_binarization(uint8_t *image, uint8_t height, uint8_t width);
void Morphology_Dilate(uint8_t* image, uint8_t height, uint8_t width, uint8_t dil_time);
//=====================================================================
//�������ƣ�rgb565togray_256
//�������أ�gray��ת����Ļҽ�
//����˵����color��ת��ǰ��16λRGB565��ʽ���ص�
//���ܸ�Ҫ����RGB565��ʽת��Ϊlcd������ʾ��256�׵ĻҶȸ�ʽ��
//=====================================================================
uint8_t rgb565togray_256 (uint16_t color)
{
	uint8_t red,green,blue;
	uint8_t gray;

	//(1)ȡ��ÿ����ɫ��ֵ
	red = (color & 0xF800) >> 8;
	green = (color & 0x07E0) >> 3; 
	blue = (color & 0x001F) << 3;
	
	//(2)�Ҷ��㷨
	gray =(red*77+green*150+blue*29+128)/256;//ת����256�Ļҽ�
			
	return gray;
}

//=====================================================================
//��������:Model_GetInputImg_Gray
//��������:����ɹ�����1
//����˵��:	 image:56*56��С��16λ��ɫͼ��
//		   output:����ģ�͵�����8λ�Ҷ�ͼ��
//���ܸ�Ҫ:�Ҷ�ͼ���²���
//=====================================================================
int Model_GetInputImg_Gray(uint16_t* image,image_32 output)
{
	//(1)��ʼ���ֲ�����
	//(1.1)��ʼ��ѹ������
	uint8_t ci,cj;
	uint8_t i,j;
	uint8_t array[56][56];
	int count=0;
	//(1.2)��ʼ��ģ��������ز���
	uint8_t image_pred[28][28]={0};//8λ�Ҷ�����
	//(2)���вü���ѹ��
	
	//(2.1)ת���ɶ�ά�Ҷ�����
	for(ci=0;ci<56;ci++)
	{
		for(cj=0;cj<56;cj++)
		{
			array[ci][cj]=rgb565togray_256(image[count++]);
			
		}
	}
	//��2.2���²�����28*28
	Inter_Linear_gray((uint8_t **)array,(uint8_t **)image_pred,56,56,28,28);
	//��2.3����򷨶�ֵ��
	otsu_binarization((uint8_t *)image_pred, 28, 28);
	//��2.4������
	//������ֵ�������ͺ�ͼ��Ŀ����������Ƚϵ͵Ļ�����Ҳ�ܸ��õ�ʶ��
	Morphology_Dilate((uint8_t *)image_pred, 28, 28, 1);
	//(3)��ֵ���ͼ�����
	for(i=2;i<30;i++)
	{
		for(j=2;j<30;j++)
		{		
			output[i][j]=image_pred[i-2][j-2];
		}
	} 
		
	return 1;//��ȡ�ɹ�
}

//=====================================================================
//��������:Model_GetInputImg_Color
//��������:�������ֵ����ָ��
//����˵��:image:56*56��С��16λRGBһάͼ������
//		   output:����ģ�͵�������ά��ɫ����ͼ��
//���ܸ�Ҫ:��ɫͼ���²���
//=====================================================================
int Model_GetInputImg_Color(uint16_t* image,image_3_32 output)
{
	//(1)��ʼ���ֲ�����
	//(1.1)��ʼ���ػ�ѹ������
	uint8_t ci,cj;
	uint8_t i,j;
	uint8_t array[3][56][56];
	int count=0;
	//(1.2)��ʼ��ģ��������ز���
	uint8_t image16_pred[3][28][28]={0};//16λRGB��������
	//(2)���вü���ѹ��
	
	//(2.1)ת������άRGB����
    for(ci=0;ci<56;ci++)
	{
		for(cj=0;cj<56;cj++)
		{
			array[0][ci][cj]=(image[count]>> 11)& 0x001F; //ȡ��ɫ;
			array[1][ci][cj]=(image[count]>> 5)& 0x003F; //ȡ��ɫ
			array[2][ci][cj]=(image[count]) & 0x001F; //ȡ��ɫ
			count++;
		}
	}
	//��2.2���²�����28*28
	Inter_Linear_color(array,image16_pred);
    //(3)��ֵ���ͼ�����
	for(i=2;i<30;i++)
	{
		for(j=2;j<30;j++)
		{	
			output[0][i][j] = image16_pred[0][i-2][j-2];
			output[1][i][j] = image16_pred[1][i-2][j-2];
			output[2][i][j] = image16_pred[2][i-2][j-2];
		}
	}

	return 1;//��ȡ�ɹ�
}

//----------------------����Ϊ�ڲ�������Ŵ�----------------------------
//=====================================================================
//��������:Inter_Linear_gray
//��������:��
//����˵��:src:ԭ�Ҷ�ͼ������    dst:Ŀ��Ҷ�ͼ������
//		   src_rows��ԭ�Ҷ�ͼ��������    src_cols��ԭ�Ҷ�ͼ��������
//         dst_rows��Ŀ��Ҷ�ͼ��������    dst_cols��Ŀ��Ҷ�ͼ��������
//���ܸ�Ҫ:�Ҷ�ͼ���²���
//=====================================================================
void Inter_Linear_gray(uint8_t **src,uint8_t **dst,int src_rows,int src_cols,
                        int dst_rows,int dst_cols)
{
	for (int i=0;i<dst_rows;i++)
	{
		double index_i=(i+0.5)*(src_rows/dst_rows)-0.5;     //�������Ķ���
		//��ֹԽ��
		if(index_i<0) index_i=0;
		if(index_i>=src_rows-1) index_i=src_rows-1;
		//����4*4���ص��У����꣩
		int i1=(int)index_i;                               //����ȡ��
		int i2=(int)(index_i+1);                           //����ȡ��
		double u=index_i - i1;                            //uΪ�õ������������е�С������
        for(int j=0;j<dst_cols;j++)
        {
        	double index_j=(j+0.5)*(src_cols/dst_cols)-0.5;     //�������Ķ���
        	//��ֹԽ��
			if(index_j<0) index_j=0;
			if(index_j>=src_cols-1) index_j=src_cols-1;
			//����4*4���ص��У����꣩
			int j1=(int)index_j;                               //����ȡ��
			int j2=(int)(index_j+1);                           //����ȡ��
			double v=index_j-j1;                             //vΪ�õ������������е�С������
			*((uint8_t*)dst+dst_cols*i+j)=255-(uint8_t)((1-u)*(1-v)*(*((uint8_t*)src+src_cols*i1+j1))+
			(1-u)*v*(*((uint8_t*)src+src_cols*i1+j2))+u*(1-v)*(*((uint8_t*)src+src_cols*i2+j1))+
			u*v*(*((uint8_t*)src+src_cols*i2+j2)));
			
        }
		
	}
}

//=====================================================================
//��������:Inter_Linear_color
//��������:��
//����˵��:src:ԭ��ɫͼ������    dst:Ŀ���ɫͼ������
//���ܸ�Ҫ:��ɫͼ���²���
//=====================================================================
void Inter_Linear_color(uint8_t src[][56][56],uint8_t dst[][28][28])
{
	for (int i=0;i<28;i++)
	{
		double index_i=(i+0.5)*2-0.5;     //�������Ķ���
		//��ֹԽ��
		if(index_i<0) index_i=0;
		if(index_i>=56-1) index_i=56-1;
		//����4*4���ص��У����꣩
		int i1=(int)index_i;                               //����ȡ��
		int i2=(int)(index_i+1);                           //����ȡ��
		double u=index_i - i1;                            //uΪ�õ������������е�С������
        for(int j=0;j<28;j++)
        {
        	double index_j=(j+0.5)*2-0.5;     //�������Ķ���
        	//��ֹԽ��
			if(index_j<0) index_j=0;
			if(index_j>=55) index_j=55;
			//����4*4���ص��У����꣩
			int j1=(int)index_j;                               //����ȡ��
			int j2=(int)(index_j+1);                           //����ȡ��
			double v=index_j-j1;                             //vΪ�õ������������е�С������
			dst[0][i][j]=(1-u)*(1-v)*src[0][i1][j1]+(1-u)*v*src[0][i1][j2]+u*(1-v)*src[0][i2][j1]+u*v*src[0][i2][j2];
			dst[1][i][j]=(1-u)*(1-v)*src[1][i1][j1]+(1-u)*v*src[1][i1][j2]+u*(1-v)*src[1][i2][j1]+u*v*src[1][i2][j2];
			dst[2][i][j]=(1-u)*(1-v)*src[2][i1][j1]+(1-u)*v*src[2][i1][j2]+u*(1-v)*src[2][i2][j1]+u*v*src[2][i2][j2];
        }
		
	}
}


////=====================================================================
////�������ƣ�Is_2mPic
////�������أ��ж����ֵ
////����˵��: TestGram:�����ԻҶ�ͼ�����
////���ܸ�Ҫ���ж�ͼ��ֲ��Ƿ����
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
////�������ƣ�ArrayCopy
////�������أ���
////����˵����dest�����ƺ��ŵ����飻source�������Ƶ����飻len:���Ƶĳ���
////���ܸ�Ҫ����Դ���鸴��ָ�����ȵ����ݵ�Ŀ������
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
////�������ƣ�Pic_Threshold_2m
////�������أ���
////����˵��: image:28*28��С�Ķ�ά�Ҷ�ͼ�����
////			    h:����ͼ��߶�
////				w:����ͼ����
////���ܸ�Ҫ����ͼ��ͨ��˫�巨��ֵ��������ֵ�˲����Ҳ�ͨ�����ڷ���
////=====================================================================
//int  Model_PicThreshold_2m(image_28 img2d,int h,int w)
//{
//	uint16_t i,j;//��ӡͼ����
//	float Gram_2m[256]={0};//�Ҷ�Ƶ������
//	float Gram_2m_1[256]={0};//�Ҷ�Ƶ������1
//	uint16_t arr_temp;
//	uint16_t cnt=0;
//	uint16_t temp_ts=0;
//
//	//(1)ͳ��������������лҶ�ֵ �����һά�����СΪ256
//	//   ��i�������Ա��ֵj�����ŻҶ�Ϊi�����ص���j��
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
//	//(2)�������� �����鲻��ƽ���ɷ�ֵ ����Ϊֻ��������ֵ��ͳ������
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
//			Is_2mPic(Gram_2m);//�ж��Ƿ�ֻ�������ֲ����ֵ
//		}
//
//		if(cnt==1000)
//		{
//			return 0;
//		}
//
//	}
//
//	//(3)�õ������ֲ����ֵ�ĻҶ�ֵȡ���߾�ֵΪ�˲���ֵ
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
//	//(4)����������ֵ��0
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
//��������:otsu_binarization
//��������:��
//����˵��: image:28*28��С�ĻҶ�ͼ��
//���ܸ�Ҫ:��򷨶�ֵ��
//=====================================================================
void otsu_binarization(uint8_t *image, uint8_t height, uint8_t width)
{
	uint8_t _t = 0, i = 0, j = 0;
	//�ֱ���ǰ���ĳ��ȣ�ǰ��������ƽ��ֵ��ǰ���ı����������Ǳ�����
	uint16_t v0_len = 0;
	float v0_mean = 0.0;
	float v0_w = 0.0;
	uint16_t v1_len = 0;
	float v1_mean = 0.0;
	float v1_w = 0.0;
	//max_sigma��max_t�ֱ��������䷽������ֵ
	float max_sigma = 0.0;
	float sigma = 0.0;
	uint8_t max_t = 0;
	for(_t = 1; _t < 255; _t++){
		//������ֵ 
		v0_len = 0, v0_mean = 0.0, v0_w = 0.0;
		v1_len = 0, v1_mean = 0.0, v1_w = 0.0;
		for(i = 0; i < height; i++){
			for(j = 0; j < width; j++){
				//���ص�
				uint8_t image_dot = *((uint8_t*)image+width*i+j);
				if(image_dot < _t){
					//ǰ��
					v0_len++;
					v0_mean+=image_dot;
				}else{
					//����
					v1_len++;
					v1_mean+=image_dot;
				}
			}
		}
		//�õ�ǰ���ı�����ƽ��ֵ
		if(v0_len > 0){
			v0_mean = v0_mean / (float)v0_len;
		}else{
			v0_mean = 0.0;
		}
		v0_w = (float)v0_len / (float)(height * width);
		//�õ������ı�����ƽ��ֵ
		if(v1_len > 0){
			v1_mean = v1_mean / (float)v1_len;
		}else{
			v1_mean = 0.0;
		}
		v1_w = 1 - v0_w;
		//�õ���䷽��
		sigma = v0_w * v1_w * ((v0_mean - v1_mean) * (v0_mean - v1_mean));
		if(sigma > max_sigma){
			//����
			max_sigma = sigma;
			max_t = _t;
		}
//		printf("v0_len=%d, v0_mean=%f, v0_w=%f, v1_len=%d, v1_mean=%f, v1_w=%f\r\n",
//		      v0_len, v0_mean, v0_w, v1_len, v1_mean, v1_w);
	}
//	printf("max_sigma = %d, max_t = %d\r\n", max_sigma, max_t);
	//��ֵ��
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
//��������:Morphology_Dilate
//��������:��
//����˵��: image:28*28��С�ĻҶ�ͼ��
//���ܸ�Ҫ:����ͼ���е�Ŀ�����
//=====================================================================
void Morphology_Dilate(uint8_t* image, uint8_t height, uint8_t width, uint8_t dil_time)
{
	//�������͵ľ���� 
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
				//���о������ 
				kernel_sum = image_pad[i-1][j-1]*kernel[0][0];
				kernel_sum += image_pad[i-1][j]*kernel[0][1];
				kernel_sum += image_pad[i-1][j+1]*kernel[0][2];
				kernel_sum += image_pad[i][j-1]*kernel[1][0];
				kernel_sum += image_pad[i][j]*kernel[1][1];
				kernel_sum += image_pad[i][j+1]*kernel[1][2];
				kernel_sum += image_pad[i+1][j-1]*kernel[2][0];
				kernel_sum += image_pad[i+1][j]*kernel[2][1];
				kernel_sum += image_pad[i+1][j+1]*kernel[2][2];
				//ֻҪ����������255���ͽ���Ӧλ�ø�ֵΪ255
				if(kernel_sum >= 255){
					*(image + (i-1)*width + (j-1)) = 255;
				}	
			}
		}	
	}
	return;
}