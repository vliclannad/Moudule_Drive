//================================================================
//�ļ����ƣ�mcu.h(mcuͷ�ļ�)
//������λ�����ݴ�ѧǶ��ʽ
//���¼�¼��20200831--20200903
//================================================================

#ifndef __MCU_H         //��ֹ�ظ�����(__MCU_H)��ͷ
#define __MCU_H

//(1)���䶯������оƬͷ�ļ�
#include "ch32v30x.h"

//(2)���̶�������cpuͷ�ļ�
#include "cpu.h"

//��3�����䶯��MCU������Ϣ��غ곣��
//                            "1234567890123456789"
#define MCU_TYPE              "AHL-CH32V307-V1.6 "  //MCU�ͺţ�19�ֽڣ�
//                            "123456789"
#define BIOS_TYPE             "20220126 "       //BIOS�汾�ţ�9�ֽڣ�
#define MCU_SECTORSIZE        256              //������С��B��
#define MCU_SECTOR_NUM        1920             //MCU��������
#define MCU_STACKTOP          0x2001ffff        //ջ����ַ��RAM��ߵ�ַ��
#define MCU_FLASH_ADDR_START  0x08000000        //MCU��FLASH��ʼ��ַ
#define MCU_FLASH_ADDR_LENGTH 0x00078000        //MCU��FLASH���ȣ�480KB��
#define MCU_RAM_ADDR_START    0x20000000        //MCU��RAM��ʼ��ַ
#define MCU_RAM_ADDR_LENGTH   0x00020000        //MCU��RAM���ȣ�128KB��


#endif /* __MCU_H */
