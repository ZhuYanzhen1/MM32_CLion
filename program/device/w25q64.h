/**************************************************************************/ /**
    \file       w25q64.H
    \brief      w25q64 function Header File
    \author     ZGL
    \version    V1.3.2
    \date       21. April 2022
******************************************************************************/

#ifndef MM32F3277_DEVICE_W25Q64_H_
#define MM32F3277_DEVICE_W25Q64_H_

#ifndef IS_PROCESS_MCU

#define W25Q64_WRITE_ENABLE         0x06
#define W25Q64_WRITE_DISABLE        0x04
#define W25Q64_READ_STATUS_REG      0x05
#define W25Q64_WRITE_STATUS_REG     0x01
#define W25Q64_READ_DATA            0x03
#define W25Q64_FAST_READ_DATA       0x0B
#define W25Q64_FAST_READ_DUAL       0x3B
#define W25Q64_PAGE_PROGRAM         0x02
#define W25Q64_BLOCK_ERASE          0xD8
#define w25q64_SECTOR_ERASE         0x20
#define W25Q64_CHIP_ERASE           0xC7
#define W25Q64_POWER_DOMN           0xB9
#define W25Q64_RELEASE_POWER_DOWM   0xAB
#define W25Q64_DEVICE_ID            0xAB
#define W25Q64_MANUFACT_DEVICE_ID   0x90
#define W25Q64_JEDEC_DEVICE_ID      0x9F

void w25q64_read_uid(void);
void w25q64_wait_busy(void);
unsigned char w25q64_read_sr(void);
void w25q64_write_sr(unsigned char sr);
void w25q64_write_enable(void);
void w25q64_write_disable(void);
void w25q64_read(unsigned char *p_buffer, unsigned int read_addr, unsigned short num_byte_to_read);
void w25q64_write(unsigned char *p_buffer, unsigned int write_addr, unsigned short num_byte_to_write);
void w25q64_erase_chip(void);
void w25q64_erase_sector(unsigned int dst_addr);
void w25q64_wake_up(void);
unsigned char w25q64_test_flash(void);
#endif

#endif // MM32F3277_DEVICE_W25Q64_H_
