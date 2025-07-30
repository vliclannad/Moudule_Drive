//======================================================================
//�ļ����ƣ�adc.h
//����ṩ��SD-Arm(sumcu.suda.edu.cn)
//�汾���£�20210309-20211111
//����������CH32V103R8T6оƬADת��ͷ�ļ�
//�ɼ�����12λ
//======================================================================

#ifndef __ADC_H_   //��ֹ�ظ����壨��ͷ��
#define __ADC_H_

#include "mcu.h"
#include "gpio.h"

//ͨ���ź궨��
#define ADC_CHANNEL_0 0           //ͨ��0
#define ADC_CHANNEL_1 1           //ͨ��1
#define ADC_CHANNEL_2 2           //ͨ��2
#define ADC_CHANNEL_3 3           //ͨ��3
#define ADC_CHANNEL_4 4           //ͨ��4
#define ADC_CHANNEL_5 5           //ͨ��5
#define ADC_CHANNEL_6 6           //ͨ��6
#define ADC_CHANNEL_7 7           //ͨ��7
#define ADC_CHANNEL_8 8           //ͨ��8
#define ADC_CHANNEL_9 9           //ͨ��9
#define ADC_CHANNEL_10 10         //ͨ��10
#define ADC_CHANNEL_11 11         //ͨ��11
#define ADC_CHANNEL_12 12         //ͨ��12
#define ADC_CHANNEL_13 13         //ͨ��13
#define ADC_CHANNEL_14 14         //ͨ��14  (PTC_NUM|4)
#define ADC_CHANNEL_15 15         //ͨ��15
#define ADC_CHANNEL_TEMPSENSOR 16 //�ڲ��¶ȼ�� ��Ҫʹ��TEMPSENSOR
#define ADC_CHANNEL_VREFINT 17     //�ڲ��ο���ѹ��� ��Ҫʹ��VREFINT����

//�¶Ȳɼ�����AD_CAL2��AD_CAL1
#define AD_CAL2 (*(uint16_t*) 0x1FFF75CA)
#define AD_CAL1 (*(uint16_t*) 0x1FFF75A8)



//======================================================================
//�������ƣ�adc_init
//���ܸ�Ҫ����ʼ��һ��ADͨ����
//����˵����Channel��ͨ���š���ѡ��Χ��ADC_CHANNEL_x(0=<x<=15)��
//                              ADC_CHANNEL_TEMPSENSOR(16)��
//                              ADC_CHANNEL_VREFINT(17)
//         NC��������δʹ�ã�Ϊ��ǿ��������ֲ��
//======================================================================
void adc_init(uint16_t Channel, uint16_t Nc);



//======================================================================
//�������ƣ�adc_read
//���ܸ�Ҫ����ģ����ת������������������
//����˵����Channel��ͨ���š���ѡ��Χ��ADC_CHANNEL_x(0=<x<=15)��
//                              ADC_CHANNEL_TEMPSENSOR(16)��
//                              ADC_CHANNEL_VREFINT(17)
//======================================================================
uint16_t adc_read(uint16_t Channel);


//======================================================================
//�� �� ��:adc_mid                                                  
//��    ��:��ȡͨ��channel��ֵ�˲����A/Dת�����                    
//��    ��:channel = ͨ����                                           
//��    ��:��ͨ����ֵ�˲����A/Dת�����                         
//�ڲ�����:adc_read                                               
//======================================================================
uint16_t adc_mid(uint16_t Channel);

//======================================================================
//�� �� ��:adc_ave                                                    
//��    ��:1·A/Dת������(��ֵ�˲�),ͨ��channel����n����ֵ�˲�,�������  
//          ��ֵ,�ó���ֵ�˲����                                        
//��    ��:channel = ͨ����,n = ��ֵ�˲�����                               
//��    ��:��ͨ����ֵ�˲����A/Dת�����                                   
//�ڲ�����:adc_mid                                                          
//======================================================================
uint16_t adc_ave(uint16_t Channel,uint8_t n); 


#endif    
