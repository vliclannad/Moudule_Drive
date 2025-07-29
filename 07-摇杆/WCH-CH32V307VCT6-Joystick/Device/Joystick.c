#include "Joystick.h"

void Joystick_Init(void)
{
    
    
    
    //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(JOYSTICK_XY_CLK , ENABLE );
    GPIO_InitStructure.GPIO_Pin = JOYSTICK_X_PIN|JOYSTICK_Y_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //GPIO模式为模拟输入
    GPIO_Init(JOYSTICK_XY_PORT, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(JOYSTICK_RS_CLK , ENABLE );
    GPIO_InitStructure.GPIO_Pin = JOYSTICK_RS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO模式为上拉输入
    GPIO_Init(JOYSTICK_RS_PORT, &GPIO_InitStructure);

    //ADC初始化
    ADC_InitTypeDef  ADC_InitStructure={0};
    RCC_APB2PeriphClockCmd(JOYSTICK_ADC_CLK, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    ADC_DeInit(JOYSTICK_ADC);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //配置ADC为独立模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;        //多通道模式下开启扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //设置开启连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //设置转换不是由外部触发启动，软件触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //设置ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 2;                //规则转换的ADC通道的数目
    ADC_Init(JOYSTICK_ADC, &ADC_InitStructure);                    //根据ADC_InitStructure中指定的参数初始化ADC1寄存器
    ADC_Cmd(JOYSTICK_ADC, ENABLE);      //使能ADC1
    ADC_ResetCalibration(JOYSTICK_ADC); //重置ADC1校准寄存器。
    while(ADC_GetResetCalibrationStatus(JOYSTICK_ADC)); //等待复位校准结束
    ADC_StartCalibration(JOYSTICK_ADC); //开启AD校准
    while(ADC_GetCalibrationStatus(JOYSTICK_ADC));      //等待校准结束

    //DMA初始化
    DMA_InitTypeDef DMA_InitStructure={0};
    RCC_AHBPeriphClockCmd( JOYSTICK_DMA_CLK, ENABLE ); //使能开启DMA时钟
    DMA_DeInit(JOYSTICK_DMA_CHANNEL); //复位DMA控制器
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->RDATAR;  //配置外设地址为ADC数据寄存器地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)gJoystickValue; //配置存储器地址为读取ADC值地址,全局变量
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;              //配置数据源为外设，即DMA传输方式为外设到存储器
    DMA_InitStructure.DMA_BufferSize = 2;                      //设置DMA数据缓冲区大小，此处设置为2
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//设置DMA外设递增模式关闭
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;         //设置DMA存储器递增模式开启
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //设置外设数据大小为半字，即两个字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //设置存储器数据大小为半字，即两个字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     //设置DMA模式为循环传输模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //设置DMA传输通道优先级为高，当使用一 DMA通道时，优先级设置不影响
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;        //因为此DMA传输方式为外设到存储器，因此禁用存储器到存储器传输方式
    DMA_Init( JOYSTICK_DMA_CHANNEL, &DMA_InitStructure );      //初始化DMA
    DMA_Cmd(JOYSTICK_DMA_CHANNEL , ENABLE);  //使能DMA

    //开启DMA传输
    ADC_RegularChannelConfig(JOYSTICK_ADC, JOYSTICK_ADC_XCHANNEL, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(JOYSTICK_ADC, JOYSTICK_ADC_YCHANNEL, 2, ADC_SampleTime_55Cycles5);
    ADC_DMACmd(JOYSTICK_ADC, ENABLE);
    ADC_SoftwareStartConvCmd(JOYSTICK_ADC, ENABLE);//使用软件触发
}
