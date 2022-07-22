#include "ff.h"
#include "diskio.h"
#include "tfcard.h"
#include "spi.h"

#define SD_CARD_SECTOR_SIZE         512
#define SD_CARD_BLOCK_SIZE          8

DSTATUS disk_status(
    BYTE pdrv        /* Physical drive nmuber to identify the drive */
) {
    return RES_OK;
}

DSTATUS disk_initialize(
    BYTE pdrv        /* Physical drive nmuber to identify the drive */
) {
    if (pdrv == 0) {
        unsigned char result;
        result = sdcard_config();
        if (result) {
            spi3_set_prescaler(SPI_BaudRatePrescaler_256);
            spi3_readwrite_byte(0xff);
            spi3_set_prescaler(SPI_BaudRatePrescaler_8);
            return STA_NOINIT;
        } else
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
        unsigned char result;
        result = sdcard_read_disk(buff, sector, count);
        if (result == 0)
            return RES_OK;
        else
            return RES_ERROR;
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
        unsigned char result;
        result = sdcard_write_disk((uint8_t *) buff, sector, count);
        if (result == 0)
            return RES_OK;
        else
            return RES_ERROR;
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
            case GET_SECTOR_SIZE:*(WORD *) buff = SD_CARD_SECTOR_SIZE;
                res = RES_OK;
                break;
            case GET_BLOCK_SIZE:*(WORD *) buff = SD_CARD_BLOCK_SIZE;
                res = RES_OK;
                break;
            case GET_SECTOR_COUNT:*(DWORD *) buff = sdcard_get_sector_num();
                res = RES_OK;
                break;
            default:res = RES_PARERR;
                break;
        }
    } else res = RES_ERROR;
    return res;
}

