//
// Created by 16625 on 2022-04-21.
//

#ifndef MAIN_C_DEVICE_W25Q32_H_
#define MAIN_C_DEVICE_W25Q32_H_

#ifndef IS_PROCESS_MCU

#define W25Q32_WRITE_ENABLE         0x06
#define W25Q32_WRITE_DISABLE        0x04
#define W25Q32_READ_STATUS_REG      0x05
#define W25Q32_WRITE_STATUS_REG     0x01
#define W25Q32_READ_DATA            0x03
#define W25Q32_FAST_READ_DATA       0x0B
#define W25Q32_FAST_READ_DUAL       0x3B
#define W25Q32_PAGE_PROGRAM         0x02
#define W25Q32_BLOCK_ERASE          0xD8
#define W25Q32_SECTOR_ERASE         0x20
#define W25Q32_CHIP_ERASE           0xC7
#define W25Q32_POWER_DOMN           0xB9
#define W25Q32_RELEASE_POWER_DOWM   0xAB
#define W25Q32_DEVICE_ID            0xAB
#define W25Q32_MANUFACT_DEVICE_ID   0x90
#define W25Q32_JEDEC_DEVICE_ID      0x9F

unsigned int w25q32_read_uid(void);
void w25q32_wait_busy(void);
unsigned char w25q32_read_sr(void);
void w25q32_write_sr(unsigned char sr);
void w25q32_write_enable(void);
void w25q32_write_disable(void);
void w25q32_read(unsigned char *p_buffer, unsigned int read_addr, unsigned short num_byte_to_read);
void w25q32_write_page(unsigned char *p_buffer, unsigned int write_addr, unsigned short num_byte_to_write);
void w25q32_write_no_check(unsigned char *p_buffer, unsigned int write_addr, unsigned short num_byte_to_write);
void w25q32_write(unsigned char *p_buffer, unsigned int write_addr, unsigned short num_byte_to_write);
void w25q32_erase_chip(void);
void w25q32_erase_sector(unsigned int dst_addr);
#endif

#endif //MAIN_C_DEVICE_W25Q32_H_
