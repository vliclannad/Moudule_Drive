//===========================================================================
//�ļ����ƣ�flash.c
//���ܸ�Ҫ��Flash�ײ���������Դ�ļ�
//��Ȩ���У�SD-Arm(sumcu.suda.edu.cn)
//�汾���£�20200831-20200904
//оƬ���ͣ�CH32V103R8T6
//===========================================================================
//����ͷ�ļ�
#include "flash.h"
/*==================================�ڲ����ú�������=====================================*/
//======================================================================
//�������ƣ�flash_Best
//�������أ�0-�ɹ� 1-ʧ��
//����˵����sect:��д��������
//            offset:��д������λ�õ�ƫ�Ƶ�ַ
//            N����д�������ֽ���
//            buf:��д�����ݵ��׵�ַ
//���ܸ�Ҫ����λ��ַ�����������µ�����д��
//��̲ο�������
//======================================================================
uint8_t flash_Best(uint16_t sect,uint16_t offset,uint16_t N,uint8_t *buf);
uint8_t flash_test(uint16_t addr, uint16_t offset,uint16_t N, uint8_t* buf);
//======================================================================
//�������ƣ�flash_Program_fast
//����˵����address
//���ܸ�Ҫ����������д�����ʼ��ַ
//=======================================================================
void flash_Program_fast(uint32_t address);

//======================================================================
//�������ƣ�flash_init
//�������أ���
//����˵������
//���ܸ�Ҫ����ʼ��flashģ��
//======================================================================
void flash_init(void)
{
    //������д����־λ
    FLASH->STATR &= 0xFFFFFFFF;

    //����Flash���ƼĴ���
    if((FLASH->CTLR & FLASH_CTLR_LOCK )!= 0u)
    {
        FLASH->KEYR = (uint32_t)FLASH_KEY1;
        FLASH->KEYR = (uint32_t)FLASH_KEY2;

        FLASH->MODEKEYR = FLASH_KEY1;
        FLASH->MODEKEYR = FLASH_KEY2;

    }
    //�ȴ�֮ǰ���һ��flash�������
    while((FLASH->STATR & FLASH_STATR_BSY) != 0U);

    //�����ݻ�����
    FLASH->ACTLR &= ~FLASH_ACTLR_PRFTBE;
    //��flash���ٱ��λ
    FLASH->CTLR &= ~FLASH_CTLR_PAGE_PG;

}

//======================================================================
//�������ƣ�flash_erase
//�������أ�����ִ��ִ��״̬��0=������1=�쳣��
//����˵����sect��Ŀ�������ţ���Χȡ����ʵ��оƬ������ STM32L433:0~127,ÿ����2KB;
//���ܸ�Ҫ������flash�洢����sect����
//======================================================================
uint8_t flash_erase(uint32_t sect)
{
    uint32_t address;

    address = (uint32_t)(sect*FLASH_PAGE_SIZE + FLASH_ADDR_START);
    flash_init();
    //�ȴ�֮ǰ���һ��flash�������
    while((FLASH->STATR & STATR_BSY) != 0U);
    //�����漴ʱ���λ
    FLASH->CTLR &= ~FLASH_CTLR_PG_set;
    //ʹ���������ٲ���
    FLASH->CTLR |= FLASH_CTLR_PAGE_SET;
    //���ò�������
    FLASH->ADDR = address;
    //��ʼ��������
    FLASH->CTLR |= FLASH_CTLR_START;
    //�ȴ��������
    while((FLASH->STATR & STATR_BSY) != 0U);
    //��ֹ��������
    FLASH->CTLR &= ~FLASH_CTLR_PAGE_SET;
    return 0;  //�ɹ�����
}

//======================================================================
//�������ƣ�flash_erase_fast
//�������أ�����ִ��ִ��״̬��0=������1=�쳣��
//����˵����sect��Ŀ�������ţ���Χȡ����ʵ��оƬ������ STM32L433:0~127,ÿ����2KB;
//���ܸ�Ҫ������flash�洢����sect����
//======================================================================
uint8_t flash_erase_fast(uint32_t sect)
{
    uint32_t address;
    flash_init();
    address = (uint32_t)(sect*FLASH_PAGE_SIZE + FLASH_ADDR_START);
    //�ȴ�֮ǰ���һ��flash�������
    while((FLASH->STATR & STATR_BSY) != 0U);
    //�����漴ʱ���λ
    FLASH->CTLR &= ~FLASH_CTLR_PG_set;
    //ʹ���������ٲ���
    FLASH->CTLR |= FLASH_CTLR_PAGE_SET;
    //���ò�������
    FLASH->ADDR = address;
    //��ʼ��������
    FLASH->CTLR |= FLASH_CTLR_START;
    //�ȴ��������
    while((FLASH->STATR & STATR_BSY) != 0U);
    //��ֹ��������
    FLASH->CTLR &= ~FLASH_CTLR_PAGE_ER;
    return 0;  //�ɹ�����
}

//======================================================================
//�������ƣ�flash_write
//�������أ�����ִ��״̬��0=������1=�쳣��
//����˵����sect�������ţ���Χȡ����ʵ��оƬ������ STM32L433:0~127,ÿ����2KB��
//        offset:д�������ڲ�ƫ�Ƶ�ַ��0~2044��Ҫ��Ϊ0,4,8,12��......��
//        N��д���ֽ���Ŀ��4~2048,Ҫ��Ϊ4,8,12,......��
//        buf��Դ���ݻ������׵�ַ
//���ܸ�Ҫ����buf��ʼ��N�ֽ�д�뵽flash�洢����sect������ offset��
//=======================================================================
uint8_t flash_write(uint16_t sect, uint16_t offset, uint16_t N,uint8_t *buf)
{
    //��20200124-�޸ġ�����д�뷽ʽΪ����д��
    uint8_t i;

    //д���ֽ�����������
    if(offset+N>MCU_SECTORSIZE)
    {
        //��д���һ������
        flash_write(sect,offset,MCU_SECTORSIZE-offset,buf);
        //��д��ڶ�������
        flash_write(sect+1,0,N-(MCU_SECTORSIZE-offset),buf+(MCU_SECTORSIZE-offset));
    }
    //д���ֽ������������
    else
    {
        //���ǰ���ַ����32λ���룬ֱ��д��
        if(offset%4==0&&N%4==0)
        {
            flash_Best(sect,offset,N,buf);
        }
        //ǰ���ַֻҪ��һ��������
        else
        {
            uint8_t data[MCU_SECTORSIZE]; //�洢��ǰ������ȫ��ֵ
            flash_read_logic(data,sect,0,MCU_SECTORSIZE); //����ǰ������ȫ��ֵ����������
            //��Ҫд����������ն�Ӧλ��д��������
            for(i = 0;i<N;i++)
            {
                data[offset+i] = buf[i];
            }
            //��������
            flash_erase(sect);
            //������д������
            flash_Best(sect,0,MCU_SECTORSIZE,data);
        }
    }
    return 0;  //�ɹ�ִ��
}


uint8_t flash_test(uint16_t addr, uint16_t offset,uint16_t N, uint8_t* buf)
{
    uint8_t i = 0;
    uint32_t address;

    address = (uint32_t)(addr*128+0x08000000 + offset);

    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;

        while(i < N)
       {
            while((FLASH->STATR & FLASH_STATR_BSY) != 0U);
            FLASH->CTLR |= FLASH_CTLR_PG_set;
           *(__IO uint16_t*)address = (buf[i]|(buf[i+1]<<8));
           while((FLASH->STATR & FLASH_STATR_BSY) != 0U);
           FLASH->CTLR &= ((uint32_t)0x00001FFE) ;
           *(__IO uint32_t*)0x40022034 = *(__IO uint32_t*)((address & 0xFFFFFFFC) ^ 0x00001000);
          i=i+2;
          address = address + 2;
       }

    FLASH->CTLR |= (uint32_t)FLASH_CTLR_LOCK;

    return 0;
}

//==========================================================================
//�������ƣ�flash_write_physical
//�������أ�����ִ��״̬��0=��������0=�쳣��
//����˵���� addr��Ŀ���ַ��Ҫ��Ϊ4�ı����Ҵ���Flash�׵�ַ
//              �����磺0x08000004��Flash�׵�ַΪ0x08000000��
//       N��д���ֽ���Ŀ��8~512��
//       buf��Դ���ݻ������׵�ַ
//���ܸ�Ҫ��flashд�����
//==========================================================================
uint8_t flash_write_physical(uint32_t addr,uint16_t N,uint8_t buf[])
{
    //��1�����������sect-�����ţ�offset-������ַ
    uint16_t sect;   //������
    uint16_t offset;    // ƫ�Ƶ�ַ

    //��2��������ֵ���������ַת��Ϊ�߼���ַ��������ƫ������
    sect = (addr-MCU_FLASH_ADDR_START)/MCU_SECTORSIZE;
    offset = addr-(sect*MCU_SECTORSIZE)-MCU_FLASH_ADDR_START;
    //��3������д�뺯��д������
    flash_write(sect,offset,N,buf);
    //��4���ȴ�д��������
    while( (FLASH->STATR & STATR_BSY) != 0U);
    return 0;  //�ɹ�ִ��
}

//======================================================================
//�������ƣ�flash_read_logic
//�������أ���
//����˵����dest���������ݴ�Ŵ�������ַ��Ŀ���Ǵ����������ݣ�RAM����
//       sect�������ţ���Χȡ����ʵ��оƬ������ CH32V103R8T6:0~512,ÿ����128B��
//       offset:�����ڲ�ƫ�Ƶ�ַ��0~2024��Ҫ��Ϊ0,4,8,12��......��
//       N�����ֽ���Ŀ��4~2048,Ҫ��Ϊ4,8,12,......��//
//���ܸ�Ҫ����ȡflash�洢����sect������ offset����ʼ��N�ֽڣ���RAM��dest��
//=======================================================================
void flash_read_logic(uint8_t *dest,uint16_t sect,uint16_t offset,uint16_t N)
{
    uint8_t *src;
    src = (uint8_t *)(FLASH_BASE + ((uint32_t)(sect*FLASH_PAGE_SIZE)) + offset);
    memcpy(dest,src,N);
}

//======================================================================
//�������ƣ�flash_read
//�������أ���
//����˵����dest���������ݴ�Ŵ�������ַ��Ŀ���Ǵ����������ݣ�RAM����
//       sect�������ţ���Χȡ����ʵ��оƬ������ CH32V103R8T6:0~512,ÿ����128B��
//       offset:�����ڲ�ƫ�Ƶ�ַ��0~2024��Ҫ��Ϊ0,4,8,12��......��
//       N�����ֽ���Ŀ��4~2048,Ҫ��Ϊ4,8,12,......��//
//���ܸ�Ҫ����ȡflash�洢����sect������ offset����ʼ��N�ֽڣ���RAM��dest��
//=======================================================================
void flash_read(uint8_t* Buffer,uint16_t sect,uint16_t offset,uint32_t N)
{
    uint32_t i, Length = ( N + 3 ) >> 2;
    uint32_t* pCode = ( uint32_t* ) (sect * FLASH_PAGE_SIZE + offset + FLASH_BASE);
    uint32_t* pBuf = ( uint32_t* ) Buffer;

    for ( i = 0; i < Length; i++ )
    {
      *pBuf++ = *pCode++;
    }
}

//======================================================================
//�������ƣ�flash_read_physical
//�������أ���
//����˵����dest���������ݴ�Ŵ�������ַ��Ŀ���Ǵ����������ݣ�RAM����
//       addr��Ŀ���ַ��Ҫ��Ϊ4�ı��������磺0x00000004��
//       N�����ֽ���Ŀ��0~1020,Ҫ��Ϊ4��8,12,......��
//���ܸ�Ҫ����ȡflashָ����ַ������
//======================================================================
void flash_read_physical(uint8_t *dest,uint32_t addr,uint16_t N)
{

    uint8_t *src;
    src=(uint8_t *)addr;
    memcpy(dest,src,N);
}

//======================================================================
//�������ƣ�flash_protect
//�������أ���
//����˵����M��(0---511)��������������������ֵ��ʵ�ʱ�����������
//���ܸ�Ҫ��flash��������
//======================================================================
uint8_t flash_protect(void)
{
    //��Ϊ�����üĴ�����LOCKλΪд1��ʹFPEC �� FLASH_CTLR ����ס����д������Ҫ���н����Կlock
    if((FLASH->CTLR | FLASH_CTLR_OPTWRE) !=0U)
    {
        //����ѡ����Կ�Ĵ���
        FLASH->OBKEYR = FLASH_KEY1;
        FLASH->OBKEYR = FLASH_KEY2;

    //�ȴ��������
    while( (FLASH->STATR & STATR_BSY) != 0U);

    FLASH->CTLR |= FLASH_CTLR_OBPG;
    //������ؼĴ���
      OB->WRPR0 = 0x007F;
    }

    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;

    if((FLASH->WPR) == FLASH_WPR_WRP)
        return 1;
    return 0;
}



//======================================================================
//�������ƣ�flash_unprotect
//�������أ���
//����˵����M����������������������ֵ��ʵ�ʱ�����������
//���ܸ�Ҫ��flash�Ᵽ������
//˵   ����
//======================================================================
void flash_unprotect(uint32_t M)
{
    uint32_t temp;

    temp = FLASH_WPR_WRP;

    //���ѡ������������ɣ������ѡ����Կ�Ĵ�������
    if((FLASH->CTLR & FLASH_CTLR_LOCK) != 0U)
    {
        //����ѡ����Կ�Ĵ���
        FLASH->OBKEYR = FLASH_KEY1;
        FLASH->OBKEYR = FLASH_KEY2;
    }
    while( (FLASH->STATR & FLASH_STATR_BSY) != 0U);
    //������ؼĴ���
    FLASH->WPR &= 0x0;
    FLASH->WPR |= temp;
    FLASH->CTLR |= FLASH_CTLR_OBPG;
    while( (FLASH->STATR & FLASH_STATR_BSY) != 0U);
    FLASH->CTLR &= ~FLASH_CTLR_OBPG;
}

//========================================================================
//�������ƣ�flashCtl_isSectorProtected
//�������أ�1=������������0=����δ������
//����˵������Ҫ��������
//���ܸ�Ҫ���ж�flash�����Ƿ񱻱���
//=========================================================================
uint8_t flash_isSectorProtected(void)
{

    uint32_t temp;
    temp = (uint32_t)(FLASH->WPR);
    if(temp != FLASH_WPR_WRP)
    {
        return 1;
    }
    else
        return 0;
}

//========================================================================
//�������ƣ�flash_isempty
//�������أ�1=Ŀ������Ϊ�գ�0=Ŀ������ǿա�
//����˵������Ҫ̽���flash���������ż��ֽ���
//���ܸ�Ҫ��flash�пղ���
//========================================================================
uint8_t flash_isempty(uint16_t sect, uint16_t N)
{
    uint16_t i,flag;
    uint32_t* buff;

    flag = 1;
    for(i = 0; i<N; i++)   //�����������ֽ�
    {
        buff=(uint32_t*)(sect*FLASH_PAGE_SIZE + FLASH_ADDR_START );

        if(buff[i]!=0xFFFFFFFF)   //�ǿ�
        {
            return flag=0;
            break;
        }
    }
    return flag;
}

//==========================================================================
//�������ƣ�flash_write_physical_fast
//�������أ�����ִ��״̬��0=��������0=�쳣��
//����˵���� addr��Ŀ���ַ��Ҫ��Ϊ4�ı����Ҵ���Flash�׵�ַ
//              �����磺0x08000004��Flash�׵�ַΪ0x08000000��
//       N��д���ֽ���Ŀ��8~512��
//       buf��Դ���ݻ������׵�ַ
//���ܸ�Ҫ��flashд�����
//==========================================================================
uint8_t flash_write_physical_fast(uint32_t Addr,uint16_t N, uint8_t *buf)
{
    uint8_t i, flag=0;
    uint32_t address, addr;
    uint32_t data_0,data_1,data_2,data_3;

    address=Addr;

    flash_init();
    /* ���滺�������� */
    FLASH_bufreset();
    addr = address;

    for(i=0;i<N;i+=16)
    {
        data_0  = (buf[i]    | buf[i+1]<<8  | buf[i+2]<<16  | buf[i+3] << 24);
        data_1 = (buf[i+4]  | buf[i+5]<<8  | buf[i+6]<<16  | buf[i+7] << 24);
        data_2  = (buf[i+8]  | buf[i+9]<<8  | buf[i+10]<<16 | buf[i+11] << 24);
        data_3 = (buf[i+12] | buf[i+13]<<8 | buf[i+14]<<16 | buf[i+15] << 24);
        flash_write_Word(addr + (uint32_t)i,data_0,data_1,data_2,data_3);
        addr= addr+0x10;
    }
    flash_Program_fast(address);

    FLASH->CTLR |= FLASH_CTLR_LOCK_set;

    for(i=0;i<N;i++)
    {
        if(buf[i] == *(uint32_t*)(address + i))
        {
            flag = 0;
        }
        else
            flag = 1;
            break;
    }
    return flag;

}

//==========================================================================
//�������ƣ�flash_write_physical_halbit
//�������أ�����ִ��״̬��0=��������0=�쳣��
//����˵���� Addr��Ŀ���ַ��Ҫ��Ϊ4�ı����Ҵ���Flash�׵�ַ
//              �����磺0x08000004��Flash�׵�ַΪ0x08000000��
//       N��д���ֽ���Ŀ��8~512��
//       buf��Դ���ݻ������׵�ַ
//���ܸ�Ҫ��flashд�����
//==========================================================================
uint8_t flash_write_physical_halbit(uint32_t Addr,uint16_t N, uint8_t buf[])
{
    uint8_t i, flag=0;
    uint32_t address;
    uint8_t * data;

    data=(uint8_t*)buf;

    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;

    address = Addr;

    if((address>=FLASH_ADDR_START)&&(address<FLASH_ADDR_END))
    {
        for(i=0;i<N;i++)
        {
            FLASH->CTLR |= FLASH_CTLR_PG_set;
            *(__IO uint16_t*)address = (uint16_t)(data[i]);
            FLASH->CTLR &= 0x00001FFE;
            *(__IO uint32_t*)0x40022034 = *(__IO uint32_t*)((address & 0xFFFFFFFC) ^ 0x00001000);
            address = address + 0x02;
        }
    }
    for(i=0;i<N;i++)
    {
        if((*(__IO uint16_t*) Addr) != data[i])
        {
            flag = 1;
            break;
        }
        Addr += 2;
    }
    return flag;
}

/* ***********************************�ڲ�����************************************** */
//======================================================================
//�������ƣ�flash_Best
//�������أ�0-�ɹ� 1-ʧ��
//����˵����sect:��д��������
//            offset:��д������λ�õ�ƫ�Ƶ�ַ
//            N����д�������ֽ���
//            buf:��д�����ݵ��׵�ַ
//���ܸ�Ҫ����λ��ַ�����������µ�����д��
//��̲ο�������
//======================================================================
uint8_t flash_Best(uint16_t sect,uint16_t offset,uint16_t N,uint8_t *buf)
{
    uint32_t temp;
    uint32_t addr;
    uint16_t i;
    addr = (uint32_t)(sect * FLASH_PAGE_SIZE + FLASH_ADDR_START + offset);

    FLASH->CTLR |= FLASH_CTLR_FTPG_PG;
    while(FLASH->STATR & STATR_BSY);
    while(FLASH->STATR & STATR_SW_BSY);

    for(i = 0; i < N; i+=4)
    {
      //��λ�ָ�ֵ
      temp = (uint32_t)((buf[i])|(buf[i+1]<<8)|(buf[i+2]<<16)|(buf[i+3]<<24));
      //�ھ��Ե�ַ(addr+i)��ʵ��˫��д��
      *(vuint32_t*)addr = temp;
      addr += 4;
      while (FLASH->STATR & STATR_SW_BSY);
    }

    FLASH->CTLR |= FLASH_CTLR_PG_STRT;
    while(FLASH->STATR & STATR_BSY);
    FLASH->CTLR &= ~FLASH_CTLR_FTPG_PG;

    return 0;
}

//======================================================================
//�������ƣ�flash_write_Word
//�������أ�0-�ɹ� 1-ʧ��
//����˵����addr��Ŀ���ַ��Ҫ��Ϊ4�ı����Ҵ���Flash�׵�ַ
//              �����磺0x08000004��Flash�׵�ַΪ0x08000000��
//       data��д���˫��
//���ܸ�Ҫ��Flash˫��д�������STM32L433ÿ��ֻ��ʵ��˫��д�룬��д��λ�֣���д��λ�֣�
//======================================================================
void flash_write_Word(uint32_t addr, uint32_t data_0, uint32_t data_1,uint32_t data_2,uint32_t data_3)
{

    //�����ݻ�����
    if((FLASH->ACTLR & FLASH_ACTLR_PRFTBE) != 0U)
    {
        FLASH->ACTLR &= ~FLASH_ACTLR_PRFTBE;
    }
    //ʹ��Flash��ʱ���
    FLASH->CTLR |= FLASH_CTLR_FTPG_PG;

    //д������
    *(__IO uint32_t*)(addr+0x00) = data_0;
    *(__IO uint32_t*)(addr+0x04) = data_1;
    *(__IO uint32_t*)(addr+0x08) = data_2;
    *(__IO uint32_t*)(addr+0x0C) = data_3;

    //ִ�����ݼ��ص��ڲ�������
    FLASH->CTLR |= FLASH_CTLR_BUFLOAD;

    while( (FLASH->STATR & STATR_BSY) != 0U);       //------------20210617 �������￨��

    //��ֹFlash��ʱ���
    FLASH->CTLR &= ~FLASH_CTLR_FTPG_PG;

}


//======================================================================
//�������ƣ�flash_write_one
//�������أ�0-�ɹ� 1-ʧ��
//����˵����addr��Ŀ���ַ��Ҫ��Ϊ4�ı����Ҵ���Flash�׵�ַ
//              �����磺0x08000004��Flash�׵�ַΪ0x08000000��
//       data��д���1��
//���ܸ�Ҫ��Flash˫��д�������STM32L433ÿ��ֻ��ʵ��˫��д�룬��д��λ�֣���д��λ�֣�
//======================================================================
uint8_t flash_write_one(uint32_t addr,uint32_t data_l)
{
    //�����ݻ�����
    if((FLASH->ACTLR & FLASH_ACTLR_PRFTBE) != 0U)
    {
        FLASH->ACTLR &= ~FLASH_ACTLR_PRFTBE;
    }

    //ʹ��Flash��ʱ���
    FLASH->CTLR |= FLASH_CTLR_PG_set;
    //��д���λ��
    *(__IO uint16_t*)addr = data_l;

    //ִ�����ݼ��ص��ڲ�������
    FLASH->CTLR |= FLASH_CTLR_BUFLOAD;

    while( (FLASH->STATR & STATR_BSY) != 0U);

    //��ֹFlash��ʱ���
    FLASH->CTLR &= ((uint32_t)0x00001FFE);

    return 0;    //���سɹ�
}

//======================================================================
//�������ƣ�FLASH_bufreset
//����˵�����Կ���д��ʱִ������ڲ� 128 �ֽڻ���������
//���ܸ�Ҫ��Flash����16�ֽ�д��ʱ��ִ������ڲ� 128 �ֽڻ���������
//======================================================================
void FLASH_bufreset(void)
{
    FLASH->CTLR |= FLASH_CTLR_FTPG_PG;
    FLASH->CTLR |= FLASH_CTLR_BUF_rst;
    while( (FLASH->STATR & STATR_BSY) != 0U);
    FLASH->CTLR &= ~FLASH_CTLR_FTPG_PG;
}

//======================================================================
//�������ƣ�flash_write_fast
//�������أ�����ִ��״̬��0=������1=�쳣��
//����˵����sect�������ţ���Χȡ����ʵ��оƬ������ STM32L433:0~127,ÿ����2KB��
//        offset:д�������ڲ�ƫ�Ƶ�ַ��0~2044��Ҫ��Ϊ0,4,8,12��......��
//        N��д���ֽ���Ŀ��4~2048,Ҫ��Ϊ4,8,12,......��
//        buf��Դ���ݻ������׵�ַ
//���ܸ�Ҫ����buf��ʼ��N�ֽ�д�뵽flash�洢����sect������ offset��
//=======================================================================
uint8_t flash_write_fast(uint32_t sect,uint32_t N, uint8_t *buf)
{
    uint8_t i, flag=0;
    uint32_t address, addr;
    uint32_t data_L;
    uint32_t data_LL,data_H,data_HH;
    address = (uint32_t)(sect * FLASH_PAGE_SIZE + FLASH_ADDR_START);

    /* ���滺�������� */
    flash_init();
    FLASH_bufreset();
    addr = address;

    for(i=0;i<N;i+=16)
    {
        data_L  = (buf[i]    | buf[i+1]<<8  | buf[i+2]<<16  | buf[i+3] << 24);
        data_LL = (buf[i+4]  | buf[i+5]<<8  | buf[i+6]<<16  | buf[i+7] << 24);
        data_H  = (buf[i+8]  | buf[i+9]<<8  | buf[i+10]<<16 | buf[i+11] << 24);
        data_HH = (buf[i+12] | buf[i+13]<<8 | buf[i+14]<<16 | buf[i+15] << 24);
        flash_write_Word(addr + (uint32_t)i,data_L,data_LL,data_H,data_HH);
    }

    flash_Program_fast(address);

    FLASH->CTLR |= FLASH_CTLR_LOCK_set;

    for(i=0;i<N;i++)
    {
        if(buf[i] == *(uint32_t*)(address + i))
        {
            flag = 0;
        }
        else
            flag = 1;
            break;
    }
    return flag;

}

//======================================================================
//�������ƣ�flash_Program_fast
//����˵����address
//���ܸ�Ҫ����������д�����ʼ��ַ
//=======================================================================
void flash_Program_fast(uint32_t address)
{
    if((address>=FLASH_ADDR_START) && (address<FLASH_ADDR_END))
    {
        FLASH->CTLR |= FLASH_CTLR_FTPG_PG;
        FLASH->ADDR = address;
        FLASH->CTLR |= FLASH_CTLR_START;
        while((FLASH->STATR & STATR_BSY) != 0U);
        FLASH->CTLR &= ~FLASH_CTLR_FTPG_PG;
    }
}