#include "Joystick.h"

void Joystick_Init(void)
{
    
    
    
    //GPIO��ʼ��
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(JOYSTICK_XY_CLK , ENABLE );
    GPIO_InitStructure.GPIO_Pin = JOYSTICK_X_PIN|JOYSTICK_Y_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //GPIOģʽΪģ������
    GPIO_Init(JOYSTICK_XY_PORT, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(JOYSTICK_RS_CLK , ENABLE );
    GPIO_InitStructure.GPIO_Pin = JOYSTICK_RS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIOģʽΪ��������
    GPIO_Init(JOYSTICK_RS_PORT, &GPIO_InitStructure);

    //ADC��ʼ��
    ADC_InitTypeDef  ADC_InitStructure={0};
    RCC_APB2PeriphClockCmd(JOYSTICK_ADC_CLK, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    ADC_DeInit(JOYSTICK_ADC);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //����ADCΪ����ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;        //��ͨ��ģʽ�¿���ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //���ÿ�������ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //����ת���������ⲿ���������������������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //����ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 2;                //����ת����ADCͨ������Ŀ
    ADC_Init(JOYSTICK_ADC, &ADC_InitStructure);                    //����ADC_InitStructure��ָ���Ĳ�����ʼ��ADC1�Ĵ���
    ADC_Cmd(JOYSTICK_ADC, ENABLE);      //ʹ��ADC1
    ADC_ResetCalibration(JOYSTICK_ADC); //����ADC1У׼�Ĵ�����
    while(ADC_GetResetCalibrationStatus(JOYSTICK_ADC)); //�ȴ���λУ׼����
    ADC_StartCalibration(JOYSTICK_ADC); //����ADУ׼
    while(ADC_GetCalibrationStatus(JOYSTICK_ADC));      //�ȴ�У׼����

    //DMA��ʼ��
    DMA_InitTypeDef DMA_InitStructure={0};
    RCC_AHBPeriphClockCmd( JOYSTICK_DMA_CLK, ENABLE ); //ʹ�ܿ���DMAʱ��
    DMA_DeInit(JOYSTICK_DMA_CHANNEL); //��λDMA������
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->RDATAR;  //���������ַΪADC���ݼĴ�����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)gJoystickValue; //���ô洢����ַΪ��ȡADCֵ��ַ,ȫ�ֱ���
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;              //��������ԴΪ���裬��DMA���䷽ʽΪ���赽�洢��
    DMA_InitStructure.DMA_BufferSize = 2;                      //����DMA���ݻ�������С���˴�����Ϊ2
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����DMA�������ģʽ�ر�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;         //����DMA�洢������ģʽ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�����������ݴ�СΪ���֣��������ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //���ô洢�����ݴ�СΪ���֣��������ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     //����DMAģʽΪѭ������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //����DMA����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ DMAͨ��ʱ�����ȼ����ò�Ӱ��
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;        //��Ϊ��DMA���䷽ʽΪ���赽�洢������˽��ô洢�����洢�����䷽ʽ
    DMA_Init( JOYSTICK_DMA_CHANNEL, &DMA_InitStructure );      //��ʼ��DMA
    DMA_Cmd(JOYSTICK_DMA_CHANNEL , ENABLE);  //ʹ��DMA

    //����DMA����
    ADC_RegularChannelConfig(JOYSTICK_ADC, JOYSTICK_ADC_XCHANNEL, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(JOYSTICK_ADC, JOYSTICK_ADC_YCHANNEL, 2, ADC_SampleTime_55Cycles5);
    ADC_DMACmd(JOYSTICK_ADC, ENABLE);
    ADC_SoftwareStartConvCmd(JOYSTICK_ADC, ENABLE);//ʹ���������
}
