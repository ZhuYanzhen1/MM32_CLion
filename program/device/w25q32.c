//
// Created by 16625 on 2022-04-21.
//

#include "w25q32.h"
#include "spi.h"

#include "hal_conf.h"
#include "delay.h"

#ifndef IS_PROCESS_MCU
unsigned int w25q32_read_uid(void) {
    static unsigned int uid_1 = 0;
    static unsigned int uid_2 = 0;
    static unsigned int uid_3 = 0;
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(0x9f);
    uid_1 = spi3_readwrite_byte(0x00);
    uid_2 = spi3_readwrite_byte(0x00);
    uid_3 = spi3_readwrite_byte(0x00);
    SPI3_NSS_SET();
    return uid_1;
}

unsigned char w25q32_read_sr(void) {
    unsigned char byte = 0;
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q32_READ_STATUS_REG);
    byte = spi3_readwrite_byte(0Xff);
    SPI3_NSS_SET();
    return byte;
}

void w25q32_write_sr(unsigned char sr) {
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q32_WRITE_STATUS_REG);
    spi3_readwrite_byte(sr);
    SPI3_NSS_SET();
}

void w25q32_write_enable(void) {
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q32_WRITE_ENABLE);
    SPI3_NSS_SET();
}

void w25q32_write_disable(void) {
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q32_WRITE_DISABLE);
    SPI3_NSS_SET();
}

void w25q32_read(unsigned char *p_buffer, unsigned int read_addr, unsigned short num_byte_to_read) {
    unsigned short i;
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q32_READ_DATA);
    spi3_readwrite_byte((unsigned char) ((read_addr) >> 16));
    spi3_readwrite_byte((unsigned char) ((read_addr) >> 8));
    spi3_readwrite_byte((unsigned char) read_addr);
    for (i = 0; i < num_byte_to_read; i++) {
        p_buffer[i] = spi3_readwrite_byte(0XFF);
    }
    SPI3_NSS_SET();
}

void w25q32_write_page(unsigned char *p_buffer, unsigned int write_addr, unsigned short num_byte_to_write) {
    unsigned short i;
    w25q32_write_enable();                    //SET WEL
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q32_PAGE_PROGRAM);
    spi3_readwrite_byte((unsigned char) ((write_addr) >> 16));
    spi3_readwrite_byte((unsigned char) ((write_addr) >> 8));
    spi3_readwrite_byte((unsigned char) write_addr);
    for (i = 0; i < num_byte_to_write; i++)
        spi3_readwrite_byte(p_buffer[i]);
    SPI3_NSS_SET();
    w25q32_wait_busy();
}

void w25q32_write_no_check(unsigned char *p_buffer, unsigned int write_addr, unsigned short num_byte_to_write) {
    unsigned short pageremain;
    pageremain = 256 - write_addr % 256;
    if (num_byte_to_write <= pageremain)pageremain = num_byte_to_write;
    while (1) {
        w25q32_write_page(p_buffer, write_addr, pageremain);
        if (num_byte_to_write == pageremain)break;
        else { //num_byte_to_write>pageremain
            p_buffer += pageremain;
            write_addr += pageremain;
            num_byte_to_write -= pageremain;
            if (num_byte_to_write > 256)pageremain = 256;
            else pageremain = num_byte_to_write;
        }
    };
}

unsigned char w25q32_buffer[4096];
void w25q32_write(unsigned char *p_buffer, unsigned int write_addr, unsigned short num_byte_to_write) {
    unsigned int secpos;
    unsigned short secoff;
    unsigned short secremain;
    unsigned short i;
    unsigned char *W25QXX_BUF;
    W25QXX_BUF = w25q32_buffer;
    secpos = write_addr / 4096;
    secoff = write_addr % 4096;
    secremain = 4096 - secoff;
    if (num_byte_to_write <= secremain)
        secremain = num_byte_to_write;
    while (1) {
        w25q32_read(W25QXX_BUF, secpos * 4096, 4096);
        for (i = 0; i < secremain; i++) {
            if (W25QXX_BUF[secoff + i] != 0XFF)
                break;
        }
        if (i < secremain) {
            w25q32_erase_sector(secpos);
            for (i = 0; i < secremain; i++) {
                W25QXX_BUF[i + secoff] = p_buffer[i];
            }
            w25q32_write_no_check(W25QXX_BUF, secpos * 4096, 4096);
        } else
            w25q32_write_no_check(p_buffer, write_addr, secremain);
        if (num_byte_to_write == secremain)
            break;
        else {
            secpos++;
            secoff = 0;

            p_buffer += secremain;
            write_addr += secremain;
            num_byte_to_write -= secremain;
            if (num_byte_to_write > 4096)
                secremain = 4096;
            else
                secremain = num_byte_to_write;
        }
    }
}

void w25q32_erase_chip(void) {
    w25q32_write_enable();                        //SET WEL
    w25q32_wait_busy();
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q32_CHIP_ERASE);
    SPI3_NSS_SET();
    w25q32_wait_busy();
}

void w25q32_erase_sector(unsigned int dst_addr) {

    dst_addr *= 4096;
    w25q32_write_enable();                    //SET WEL
    w25q32_wait_busy();
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q32_SECTOR_ERASE);
    spi3_readwrite_byte((unsigned char) ((dst_addr) >> 16));
    spi3_readwrite_byte((unsigned char) ((dst_addr) >> 8));
    spi3_readwrite_byte((unsigned char) dst_addr);
    SPI3_NSS_SET();
    w25q32_wait_busy();
}

void w25q32_wait_busy(void) {
    while ((w25q32_read_sr() & 0x01) == 0x01);
}

void w25q32_power_down(void) {
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q32_POWER_DOMN);
    SPI3_NSS_SET();
    delayus(3);
}

void w25q32_wake_up(void) {
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q32_RELEASE_POWER_DOWM);    //  send W25Q32_POWER_DOMN command 0xAB
    SPI3_NSS_SET();
    delayus(3);
}

#endif
