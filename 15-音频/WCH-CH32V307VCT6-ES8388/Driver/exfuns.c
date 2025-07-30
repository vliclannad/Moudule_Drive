#include "string.h"
#include "exfuns.h"


#define FILE_MAX_TYPE_NUM       7   //���FILE_MAX_TYPE_NUM������
#define FILE_MAX_SUBT_NUM       4   //���FILE_MAX_SUBT_NUM��С��

 //�ļ������б�
u8*const FILE_TYPE_TBL[FILE_MAX_TYPE_NUM][FILE_MAX_SUBT_NUM]=
{
{"BIN"},            //BIN�ļ�
{"LRC"},            //LRC�ļ�
{"NES"},            //NES�ļ�
{"TXT","C","H"},    //�ı��ļ�
{"WAV","MP3","APE","FLAC"},//֧�ֵ������ļ�
{"BMP","JPG","JPEG","GIF"},//ͼƬ�ļ�
{"AVI"},            //��Ƶ�ļ�
};
//�����ļ�������
//fname:�ļ���
//����ֵ:0XFF,��ʾ�޷�ʶ����ļ����ͱ��.
//       ����,����λ��ʾ��������,����λ��ʾ����С��.

//��Сд��ĸתΪ��д��ĸ,���������,�򱣳ֲ���.
u8 char_upper(u8 c)
{
    if(c<'A')return c;//����,���ֲ���.
    if(c>='a')return c-0x20;//��Ϊ��д.
    else return c;//��д,���ֲ���
}


u8 f_typetell(u8 *fname)
{
    u8 tbuf[5];
    u8 *attr='\0';//��׺��
    u8 i=0,j;
    while(i<250)
    {
        i++;
        if(*fname=='\0')break;//ƫ�Ƶ��������.
        fname++;
    }
    if(i==250)return 0XFF;//������ַ���.
    for(i=0;i<5;i++)//�õ���׺��
    {
        fname--;
        if(*fname=='.')
        {
            fname++;
            attr=fname;
            break;
        }
    }
    strcpy((char *)tbuf,(const char*)attr);//copy
    for(i=0;i<4;i++)tbuf[i]=char_upper(tbuf[i]);//ȫ����Ϊ��д
    for(i=0;i<FILE_MAX_TYPE_NUM;i++)    //����Ա�
    {
        for(j=0;j<FILE_MAX_SUBT_NUM;j++)//����Ա�
        {
            if(*FILE_TYPE_TBL[i][j]==0)break;//�����Ѿ�û�пɶԱȵĳ�Ա��.
            if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//�ҵ���
            {
                return (i<<4)|j;
            }
        }
    }
    return 0XFF;//û�ҵ�
}
