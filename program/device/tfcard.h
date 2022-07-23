/**************************************************************************/ /**
    \file       tfcard.h
    \brief      tfcard function Header File
    \author     LaoZhu
    \version    V1.3.2
    \date       21. April 2022
******************************************************************************/

#ifndef MM32F3277_DEVICE_TFCARD_H_
#define MM32F3277_DEVICE_TFCARD_H_

#ifndef IS_PROCESS_MCU

#define SD_TYPE_ERR         0x00
#define SD_TYPE_MMC         0x01
#define SD_TYPE_V1          0x02
#define SD_TYPE_V2          0x04
#define SD_TYPE_V2HC        0x06

void sdcard_switch_device(unsigned char index);
unsigned char sdcard_config(void);
unsigned int sdcard_get_sector_num(void);
unsigned char sdcard_read_disk(unsigned char *buf, unsigned int sector, unsigned char cnt);
unsigned char sdcard_write_disk(unsigned char *buf, unsigned int sector, unsigned char cnt);

#endif

#endif // MM32F3277_DEVICE_TFCARD_H_
