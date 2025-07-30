#include "wavplay.h"
#include "ff.h"
#include "stdlib.h"
#include "ES8388.h"
#include "I2S.h"







__wavctrl wavctrl;      //WAV���ƽṹ��
vu8 wavwitchbuf=0;      //i2sbufxָʾ��־
//���ֲ��ſ�����
__audiodev audiodev;
//WAV������ʼ��
//fname:�ļ�·��+�ļ���
//wavx:wav ��Ϣ��Žṹ��ָ��
//����ֵ:0,�ɹ�;1,���ļ�ʧ��;2,��WAV�ļ�;3,DATA����δ�ҵ�.
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
    if(ftemp&&buf)  //�ڴ�����ɹ�
    {
        res=f_open(ftemp,(TCHAR*)fname,FA_READ);//���ļ�
        if(res==FR_OK)
        {
            f_read(ftemp,buf,512,&br);  //��ȡ512�ֽ�������
            riff=(ChunkRIFF *)buf;      //��ȡRIFF��
            if(riff->Format==0X45564157)//��WAV�ļ�
            {
                printf("�ж���wav�ļ�\r\n");
                fmt=(ChunkFMT *)(buf+12);   //��ȡFMT��
                fact=(ChunkFACT *)(buf+12+8+fmt->ChunkSize);//��ȡFACT��
                if(fact->ChunkID==0X74636166||fact->ChunkID==0X5453494C)wavx->datastart=12+8+fmt->ChunkSize+8+fact->ChunkSize;//����fact/LIST���ʱ��(δ����)
                else wavx->datastart=12+8+fmt->ChunkSize;
                data=(ChunkDATA *)(buf+wavx->datastart);    //��ȡDATA��
                if(data->ChunkID==0X61746164)//�����ɹ�!
                {
                    printf("ChunkID�����ɹ�\r\n");
                    wavx->audioformat=fmt->AudioFormat;     //��Ƶ��ʽ
                    wavx->nchannels=fmt->NumOfChannels;     //ͨ����
                    wavx->samplerate=fmt->SampleRate;       //������
                    wavx->bitrate=fmt->ByteRate*8;          //�õ�λ��
                    wavx->blockalign=fmt->BlockAlign;       //�����
                    wavx->bps=fmt->BitsPerSample;           //λ��,16/24/32λ

                    wavx->datasize=data->ChunkSize;         //���ݿ��С
                    wavx->datastart=wavx->datastart+8;      //��������ʼ�ĵط�.

                    printf("wavx->audioformat:%d\r\n",wavx->audioformat);
                    printf("wavx->nchannels:%d\r\n",wavx->nchannels);
                    printf("wavx->samplerate:%d\r\n",wavx->samplerate);
                    printf("wavx->bitrate:%d\r\n",wavx->bitrate);
                    printf("wavx->blockalign:%d\r\n",wavx->blockalign);
                    printf("wavx->bps:%d\r\n",wavx->bps);
                    printf("wavx->datasize:%d\r\n",wavx->datasize);
                    printf("wavx->datastart:%d\r\n",wavx->datastart);
                }else res=3;//data����δ�ҵ�.
            }else res=2;//��wav�ļ�

        }else res=1;//���ļ�����*/
    }
    f_close(ftemp);
    free(ftemp);//�ͷ��ڴ�
    return 0;
}

//���buf
//buf:������
//size:���������
//bits:λ��(16/24)
//����ֵ:���������ݸ���
u32 wav_buffill(u8 *buf,u16 size,u8 bits)
{
    u16 readlen=0;
    u32 bread;
    u16 i;
    u8 *p;
    u8 res;
    //printf("��ʼ���\r\n");
    if(bits==24)//24bit��Ƶ,��Ҫ����һ��
    {
        readlen=(size/4)*3;                         //�˴�Ҫ��ȡ���ֽ���
        f_read(audiodev.file,audiodev.tbuf,readlen,(UINT*)&bread);  //��ȡ����
        
        p=audiodev.tbuf;
        for(i=0;i<size;)
        {
            buf[i++]=p[1];
            buf[i]=p[2];
            i+=2;
            buf[i++]=p[0];
            p+=3;
        }
        bread=(bread*4)/3;      //����Ĵ�С.
    }else
    {
        f_read(audiodev.file,buf,size,(UINT*)&bread);//16bit��Ƶ,ֱ�Ӷ�ȡ����
        //printf("ʵ�ʶ�ȡ������Ϊ = %d\r\n",bread);
        if(bread<size)//����������,����0
        {
            for(i=bread;i<size-bread;i++)buf[i]=0;
        }
    }
    return bread;
}

//����ĳ��WAV�ļ�
//fname:wav�ļ�·��.
//����ֵ:
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
        res=wav_decode_init(fname,&wavctrl);//�õ��ļ�����Ϣ
        if(res==0)//�����ļ��ɹ�
        {
            printf("�����ļ��ɹ�\r\n");
            if(wavctrl.bps==16)
            {
                //WM8978_CfgAudioIF(I2S_Standard_Phillips,I2S_DataFormat_16b);    //�����ֱ�׼,16λ���ݳ���
                USER_I2S_Init(I2S_Standard_Phillips,I2S_Mode_MasterTx,I2S_CPOL_Low,I2S_DataFormat_16b,I2S_AudioFreq_44k);     //�����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,16λ��չ֡����
            }else if(wavctrl.bps==24)
            {
                //WM8978_CfgAudioIF(I2S_Standard_Phillips,I2S_DataFormat_24b);    //�����ֱ�׼,24λ���ݳ���
                USER_I2S_Init(I2S_Standard_Phillips,I2S_Mode_MasterTx,I2S_CPOL_Low,I2S_DataFormat_24b,I2S_AudioFreq_44k);     //�����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,24λ��չ֡����
            }
            res=f_open(audiodev.file,(TCHAR*)fname,FA_READ);    //���ļ�
            if(res==0)
            {
                printf("���ļ��ɹ�\r\n");
                f_lseek(audiodev.file, wavctrl.datastart);      //�����ļ�ͷ
                printf("��ʼ����\r\n");
                while(res==0)
                {
                   fillnum=wav_buffill(audiodev.i2sbuf,WAV_I2S_TX_DMA_BUFSIZE,wavctrl.bps);//��䲥������
                   for(i=0;i<WAV_I2S_TX_DMA_BUFSIZE/2;i++)
                   {
                    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE)==RESET);
                    SPI_I2S_SendData(SPI3,((u16)audiodev.i2sbuf[i*2+1]<<8)|audiodev.i2sbuf[i*2]);//WAV�ļ����ݵͶ���ǰ
                   }
                   
                    if(fillnum!=WAV_I2S_TX_DMA_BUFSIZE)//���Ž���?
                    {
                        printf("���Ž���\r\n");
                        res=1;
                        break;
                    }
                }
            }else res=0XFF;
        }else res=0XFF;
    }else res=0XFF;
    free(audiodev.tbuf);   //�ͷ��ڴ�
    free(audiodev.i2sbuf);//�ͷ��ڴ�
    free(audiodev.file);   //�ͷ��ڴ�
    return res;
}
