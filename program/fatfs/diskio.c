#include "ff.h"
#include "diskio.h"
#include "w25q64.h"

#define SPI_FLASH_SIZE_IN_MBYTE     8           // 8MB Flash
#define SPI_FLASH_SECTOR_SIZE       512
#define SPI_FLASH_SECTOR_COUNT      2048 * SPI_FLASH_SIZE_IN_MBYTE
#define SPI_FLASH_BLOCK_SIZE        8

DSTATUS disk_status(
    BYTE pdrv        /* Physical drive nmuber to identify the drive */
) {
    return RES_OK;
}

DSTATUS disk_initialize(
    BYTE pdrv        /* Physical drive nmuber to identify the drive */
) {
    if (pdrv == 0) {
        w25q64_read_uid();
        w25q64_wake_up();
        return RES_OK;
    } else
        return STA_NOINIT;
}

DRESULT disk_read(
    BYTE pdrv,       /* Physical drive nmuber to identify the drive */
    BYTE *buff,      /* Data buffer to store read data */
    LBA_t sector,    /* Start sector in LBA */
    UINT count       /* Number of sectors to read */
) {
    if (!count)
        return RES_PARERR;
    if (pdrv == 0) {
        for (; count > 0; count--) {
            w25q64_read(buff, sector * SPI_FLASH_SECTOR_SIZE, SPI_FLASH_SECTOR_SIZE);
            sector++;
            buff += SPI_FLASH_SECTOR_SIZE;
        }
        return RES_OK;
    } else
        return RES_ERROR;
}

DRESULT disk_write(
    BYTE pdrv,           /* Physical drive nmuber to identify the drive */
    const BYTE *buff,    /* Data to be written */
    LBA_t sector,        /* Start sector in LBA */
    UINT count           /* Number of sectors to write */
) {
    if (!count)
        return RES_PARERR;
    if (pdrv == 0) {
        for (; count > 0; count--) {
            w25q64_write((unsigned char *) buff, sector * SPI_FLASH_SECTOR_SIZE, SPI_FLASH_SECTOR_SIZE);
            sector++;
            buff += SPI_FLASH_SECTOR_SIZE;
        }
        return RES_OK;
    } else
        return RES_ERROR;
}

DRESULT disk_ioctl(
    BYTE pdrv,       /* Physical drive nmuber (0..) */
    BYTE cmd,        /* Control code */
    void *buff       /* Buffer to send/receive control data */
) {
    DRESULT res;
    if (pdrv == 0) {
        switch (cmd) {
            case CTRL_SYNC:res = RES_OK;
                break;
            case GET_SECTOR_SIZE:*(WORD *) buff = SPI_FLASH_SECTOR_SIZE;
                res = RES_OK;
                break;
            case GET_BLOCK_SIZE:*(WORD *) buff = SPI_FLASH_BLOCK_SIZE;
                res = RES_OK;
                break;
            case GET_SECTOR_COUNT:*(DWORD *) buff = SPI_FLASH_SECTOR_COUNT;
                res = RES_OK;
                break;
            default:res = RES_PARERR;
                break;
        }
    } else res = RES_ERROR;
    return res;
}

