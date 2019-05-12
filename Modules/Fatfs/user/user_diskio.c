
#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/* 
 * Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
 * To be suppressed in the future. 
 * Kept to ensure backward compatibility with previous CubeMx versions when 
 * migrating projects. 
 * User code previously added there should be copied in the new user sections before 
 * the section contents can be deleted.
 */

#endif

/* USER CODE BEGIN DECL */
#include <string.h>
#include "ff_gen_drv.h"
#include "w25q64_app.h"
#include "windebug.h"

/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;


DSTATUS USER_initialize (BYTE pdrv);
DSTATUS USER_status (BYTE pdrv);
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);  
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  USER_Driver =
{
  USER_initialize,
  USER_status,
  USER_read, 
#if  _USE_WRITE
  USER_write,
#endif  /* _USE_WRITE == 1 */  
#if  _USE_IOCTL == 1
  USER_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_initialize (
	BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
    W25Q64_AppInit();
    Stat = RES_OK;
    return Stat;
}
 
/**
  * @brief  Gets Disk Status 
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_status (
	BYTE pdrv       /* Physical drive number to identify the drive */
)
{
    Stat = RES_OK;
    return Stat;
}

/**
  * @brief  Reads Sector(s) 
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USER_read (
	BYTE pdrv,      /* Physical drive nmuber to identify the drive */
	BYTE *buff,     /* Data buffer to store read data */
	DWORD sector,   /* Sector address in LBA */
	UINT count      /* Number of sectors to read */
)
{
    s16 ret;
    wLog("%u,%u", sector, count);
    //printf("USER_read:%u,%u\r\n", sector, count);
    ret = W25Q64_ReadData(&g_w25q64_handle, sector*SECTOR_SZ, buff, count*SECTOR_SZ);
    WDebugHex(0, (u32)buff, 100, 8, "rd");
    return ((ret==0)?RES_OK: RES_ERROR);
}

/**
  * @brief  Writes Sector(s)  
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT USER_write (
	BYTE pdrv,          /* Physical drive nmuber to identify the drive */
	const BYTE *buff,   /* Data to be written */
	DWORD sector,       /* Sector address in LBA */
	UINT count          /* Number of sectors to write */
)
{ 
    u32 addr, sect_idx;
    const u8* ptr= buff;
    s16 ret;
static u8 testbuf[50];
    sect_idx = sector;

    wLog("%u,%u", sector, count);
    //printf("USER_write:%u,%u\r\n", sector, count);
    WDebugHex(0, (u32)buff, 50, 8, "toWr");

#if 1
    ret = W25Q64_ReadData(&g_w25q64_handle, sector*SECTOR_SZ, (u8*)testbuf, 50);
    if(0 == ret)
    {
        WDebugHex(0, (u32)testbuf, 50, 8, "prRd");
    }
#endif

        ret = W25Q64_WriteDataCheck(&g_w25q64_handle, sector*SECTOR_SZ, (u8*)buff, count*SECTOR_SZ);
        if(ret)
        {
            return RES_ERROR;
        }


    ret = W25Q64_ReadData(&g_w25q64_handle, sector*SECTOR_SZ, (u8*)buff, 100);
    if(0 == ret)
    {
        WDebugHex(0, (u32)buff, 50, 8, "imrd");
    }

    return RES_OK;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation  
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT USER_ioctl (
	BYTE pdrv,      /* Physical drive nmuber (0..) */
	BYTE cmd,       /* Control code */
	void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res = RES_ERROR;

    wLog("cmd:%u", cmd);

    switch(cmd)
    {
        case CTRL_SYNC:
            res = RES_OK;
            break;

        case GET_SECTOR_COUNT:
            *(u32*)buff = 512;
            res = RES_OK;
            break;

        case GET_SECTOR_SIZE:
            //*(u16*)buff = 4096;
            res = RES_OK;
            break;

        case GET_BLOCK_SIZE:
            *(u32*)buff = 4096*16;
            res = RES_OK;
            break;

        case CTRL_TRIM:
            {
                u32 start_sect, end_sect, sect;
                s16 ret;

                WDebugHex(0, (u32)buff, 8, 8, "CTRL_TRIM");
                //printf("CTRL_TRIM %u:%u\r\n", *(u32*)buff, *((u32*)buff + 1));
                
                start_sect = (*(u32*)buff)*SECTOR_SZ;
                end_sect = (*((u32*)buff + 1))*SECTOR_SZ;
                for(sect = start_sect; sect < end_sect; sect+=FF_MIN_SS)
                {
                    ret = 0;
                    //ret = W25Q64_EraseSector(&g_w25q64_handle, sect);
                    if(ret)
                    {
                        return RES_ERROR;
                    }
                }
            }
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
            break;
    }
    
    return res;
}
#endif /* _USE_IOCTL == 1 */

