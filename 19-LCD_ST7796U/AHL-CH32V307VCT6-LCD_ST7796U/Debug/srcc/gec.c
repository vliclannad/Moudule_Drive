//=====================================================================
//�ļ����ƣ�gec.c�ļ�
//������λ��SD-Arm(sumcu.suda.edu.cn)
//���¼�¼��20181201-20200627
//��ֲ���򣺡��̶���
//=====================================================================
#include "gec.h"
//======================================================================
//�������ƣ�Vectors_Init
//�������أ���
//����˵������
//���ܸ�Ҫ��User��BIOS�ж�������Ĳ��ּ̳�,�����⺯��ָ���ʼ��
//�޸���Ϣ��WYH��20200805���淶
//======================================================================
void  Vectors_Init()
{
    //���û����򣬱��뱾�δ���
    #if (GEC_USER_SECTOR_START!=0)
    //��1����Flash����1������ǰ24�ֽ�Ϊ�գ���д���豸���кż�����汾�ų�ֵ
    if(flash_isempty(MCU_SECTOR_NUM-1,24))
    {
        flash_write_physical((MCU_SECTOR_NUM-1)*MCU_SECTORSIZE+
        MCU_FLASH_ADDR_START,24,(uint8_t *)"0123456789ABCDEF20200716");
    }

    //��2���̳�BIOS���ж�
    void ** bios = (void *)MCU_FLASH_ADDR_START;    //BIOS��������ʼ��ַ
    uint32_t user[MCU_SECTORSIZE];                 //User�������������
    //��2.1����ȡUser������ж���������жϴ�������׵�ַ������user����
    flash_read_physical((uint8_t *)user,GEC_USER_SECTOR_START*
               MCU_SECTORSIZE+MCU_FLASH_ADDR_START,MCU_SECTORSIZE*2);
 	//��2.2����ȡUser������ж���������жϴ�������׵�ַ������user����
    if (user[BIOS_UPDATE_IRQn] != (uint32_t)bios[BIOS_UPDATE_IRQn])
    {
        printf("  ��User��ʾ�����жϼ̳�\r\n");
        //��2.2.1���ı�User�ж���������BIOS_UART�ж�����
        flash_erase(GEC_USER_SECTOR_START);
        flash_erase(GEC_USER_SECTOR_START+1);
//        flash_erase(GEC_USER_SECTOR_START+2);
//        flash_erase(GEC_USER_SECTOR_START+3);
        //��2.2.2���޸�USER�ж���������BIOS���´��ڵ��жϴ�������ַ
        user[BIOS_UPDATE_IRQn]=(uint32_t)bios[BIOS_UPDATE_IRQn];
//        //����  ����פ������ϵͳʱʹ��
//        //��2.2.3���޸�USER�ж���������SYSTICK��SW�жϴ�������ַ
//        user[BIOS_SYSTICK_IRQn] = (uint32_t)bios[BIOS_SYSTICK_IRQn];
//        user[BIOS_SW_IRQn]=(uint32_t)bios[BIOS_SW_IRQn];

        //��2.2.3�����޸ĺ��user����д��User�ж�������
        flash_write_physical(GEC_USER_SECTOR_START*MCU_SECTORSIZE+
               MCU_FLASH_ADDR_START,MCU_SECTORSIZE*2,(uint8_t *)user);
    }

    //��3����component_fun��ֵ��SYSTEM_FUNCTION������
    component_fun=(void **)(MCU_FLASH_ADDR_START+GEC_COMPONENT_LST_START*MCU_SECTORSIZE);

    //��4��printf��ʾ
    printf("  ��User��ʾ����������User��main����ִ��...\r\n\n");

    #endif
}
