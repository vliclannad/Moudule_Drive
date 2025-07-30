#include "wavplay.h"
#include "ff.h"
#include "stdlib.h"
#include "ES8388.h"
#include "I2S.h"







__wavctrl wavctrl;      //WAV控制结构体
vu8 wavwitchbuf=0;      //i2sbufx指示标志
//音乐播放控制器
__audiodev audiodev;
//WAV解析初始化
//fname:文件路径+文件名
//wavx:wav 信息存放结构体指针
//返回值:0,成功;1,打开文件失败;2,非WAV文件;3,DATA区域未找到.
u8 wav_decode_init(u8* fname,__wavctrl* wavx)
{
    FIL*ftemp;
    u8 *buf;
    u32 br=0;
    u8 res=0;
    ChunkRIFF *riff;
    ChunkFMT *fmt;
    ChunkFACT *fact;
    ChunkDATA *data;
    ftemp=(FIL*)malloc(sizeof(FIL));
    buf=malloc(512);
    if(ftemp&&buf)  //内存申请成功
    {
        res=f_open(ftemp,(TCHAR*)fname,FA_READ);//打开文件
        if(res==FR_OK)
        {
            f_read(ftemp,buf,512,&br);  //读取512字节在数据
            riff=(ChunkRIFF *)buf;      //获取RIFF块
            if(riff->Format==0X45564157)//是WAV文件
            {
                printf("判断是wav文件\r\n");
                fmt=(ChunkFMT *)(buf+12);   //获取FMT块
                fact=(ChunkFACT *)(buf+12+8+fmt->ChunkSize);//读取FACT块
                if(fact->ChunkID==0X74636166||fact->ChunkID==0X5453494C)wavx->datastart=12+8+fmt->ChunkSize+8+fact->ChunkSize;//具有fact/LIST块的时候(未测试)
                else wavx->datastart=12+8+fmt->ChunkSize;
                data=(ChunkDATA *)(buf+wavx->datastart);    //读取DATA块
                if(data->ChunkID==0X61746164)//解析成功!
                {
                    printf("ChunkID解析成功\r\n");
                    wavx->audioformat=fmt->AudioFormat;     //音频格式
                    wavx->nchannels=fmt->NumOfChannels;     //通道数
                    wavx->samplerate=fmt->SampleRate;       //采样率
                    wavx->bitrate=fmt->ByteRate*8;          //得到位速
                    wavx->blockalign=fmt->BlockAlign;       //块对齐
                    wavx->bps=fmt->BitsPerSample;           //位数,16/24/32位

                    wavx->datasize=data->ChunkSize;         //数据块大小
                    wavx->datastart=wavx->datastart+8;      //数据流开始的地方.

                    printf("wavx->audioformat:%d\r\n",wavx->audioformat);
                    printf("wavx->nchannels:%d\r\n",wavx->nchannels);
                    printf("wavx->samplerate:%d\r\n",wavx->samplerate);
                    printf("wavx->bitrate:%d\r\n",wavx->bitrate);
                    printf("wavx->blockalign:%d\r\n",wavx->blockalign);
                    printf("wavx->bps:%d\r\n",wavx->bps);
                    printf("wavx->datasize:%d\r\n",wavx->datasize);
                    printf("wavx->datastart:%d\r\n",wavx->datastart);
                }else res=3;//data区域未找到.
            }else res=2;//非wav文件

        }else res=1;//打开文件错误*/
    }
    f_close(ftemp);
    free(ftemp);//释放内存
    return 0;
}

//填充buf
//buf:数据区
//size:填充数据量
//bits:位数(16/24)
//返回值:读到的数据个数
u32 wav_buffill(u8 *buf,u16 size,u8 bits)
{
    u16 readlen=0;
    u32 bread;
    u16 i;
    u8 *p;
    u8 res;
    //printf("开始填充\r\n");
    if(bits==24)//24bit音频,需要处理一下
    {
        readlen=(size/4)*3;                         //此次要读取的字节数
        f_read(audiodev.file,audiodev.tbuf,readlen,(UINT*)&bread);  //读取数据
        
        p=audiodev.tbuf;
        for(i=0;i<size;)
        {
            buf[i++]=p[1];
            buf[i]=p[2];
            i+=2;
            buf[i++]=p[0];
            p+=3;
        }
        bread=(bread*4)/3;      //填充后的大小.
    }else
    {
        f_read(audiodev.file,buf,size,(UINT*)&bread);//16bit音频,直接读取数据
        //printf("实际读取数据量为 = %d\r\n",bread);
        if(bread<size)//不够数据了,补充0
        {
            for(i=bread;i<size-bread;i++)buf[i]=0;
        }
    }
    return bread;
}

//播放某个WAV文件
//fname:wav文件路径.
//返回值:
u8 wav_play_song(u8* fname)
{
    u8 key;
    u8 count = 0;
    u8 t=0;
    u8 res;
    u32 fillnum;
    u16 i;
    audiodev.file=(FIL*)malloc(sizeof(FIL));
    audiodev.i2sbuf=malloc(WAV_I2S_TX_DMA_BUFSIZE);
    audiodev.tbuf=malloc(WAV_I2S_TX_DMA_BUFSIZE);
    if(audiodev.file&&audiodev.i2sbuf&&audiodev.tbuf)
    {
        res=wav_decode_init(fname,&wavctrl);//得到文件的信息
        if(res==0)//解析文件成功
        {
            printf("解析文件成功\r\n");
            if(wavctrl.bps==16)
            {
                //WM8978_CfgAudioIF(I2S_Standard_Phillips,I2S_DataFormat_16b);    //飞利浦标准,16位数据长度
                USER_I2S_Init(I2S_Standard_Phillips,I2S_Mode_MasterTx,I2S_CPOL_Low,I2S_DataFormat_16b,I2S_AudioFreq_44k);     //飞利浦标准,主机发送,时钟低电平有效,16位扩展帧长度
            }else if(wavctrl.bps==24)
            {
                //WM8978_CfgAudioIF(I2S_Standard_Phillips,I2S_DataFormat_24b);    //飞利浦标准,24位数据长度
                USER_I2S_Init(I2S_Standard_Phillips,I2S_Mode_MasterTx,I2S_CPOL_Low,I2S_DataFormat_24b,I2S_AudioFreq_44k);     //飞利浦标准,主机发送,时钟低电平有效,24位扩展帧长度
            }
            res=f_open(audiodev.file,(TCHAR*)fname,FA_READ);    //打开文件
            if(res==0)
            {
                printf("打开文件成功\r\n");
                f_lseek(audiodev.file, wavctrl.datastart);      //跳过文件头
                printf("开始播放\r\n");
                while(res==0)
                {
                   fillnum=wav_buffill(audiodev.i2sbuf,WAV_I2S_TX_DMA_BUFSIZE,wavctrl.bps);//填充播放数据
                   for(i=0;i<WAV_I2S_TX_DMA_BUFSIZE/2;i++)
                   {
                    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE)==RESET);
                    SPI_I2S_SendData(SPI3,((u16)audiodev.i2sbuf[i*2+1]<<8)|audiodev.i2sbuf[i*2]);//WAV文件数据低端在前
                   }
                   
                    if(fillnum!=WAV_I2S_TX_DMA_BUFSIZE)//播放结束?
                    {
                        printf("播放结束\r\n");
                        res=1;
                        break;
                    }
                }
            }else res=0XFF;
        }else res=0XFF;
    }else res=0XFF;
    free(audiodev.tbuf);   //释放内存
    free(audiodev.i2sbuf);//释放内存
    free(audiodev.file);   //释放内存
    return res;
}
