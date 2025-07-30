/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/


#include "diskio.h"		/* Declarations of disk functions */

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/
//��ȡ�����豸״̬���ߺ�SD����ʼ��������ⲽ
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	switch (pdrv) {
	case DEV_RAM :
	    break;
	case DEV_MMC :
		stat = 0;
		return stat;
	case DEV_USB :
	    break;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
//���̳�ʼ��
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM :
	    break;
	case DEV_MMC :
		stat = SD_Init();
		return stat;

	case DEV_USB :
	    break;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
//���̶�ȡ����
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
	    break;
	case DEV_MMC :
		res = SD_ReadDisk(buff, sector, count);
		return res;

	case DEV_USB :
	    break;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0
//����д�뺯��
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
	    break;
	case DEV_MMC :
		res = SD_WriteDisk(buff, sector, count);
		return res;
	case DEV_USB :
	    break;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
//��ȡ������Ϣ
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;
	switch (pdrv) {
	case DEV_RAM :
	    break;
	case DEV_MMC :
	    switch(cmd)
	    {
	    case GET_SECTOR_SIZE://��ȡ����������С
	        *(WORD*)buff = SDCardInfo.CardBlockSize;
	        break;
	    case GET_BLOCK_SIZE://ָ����������С��λ
	        *(WORD*)buff = SDCardInfo.CardBlockSize;
	        break;
	    case GET_SECTOR_COUNT://��ȡ��������
	        *(WORD*)buff = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;
	        break;
	    case CTRL_SYNC:
	        break;
	    }
		return res;
	case DEV_USB :
	    break;
	}
	return RES_PARERR;
}
//��ȡ�ļ�ʱ�䣬û��ʵ������
DWORD get_fattime(void)
{
    return 0;
}

