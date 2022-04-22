/**************************************************************************/ /**
    \file       w25q64.c
    \brief      w25q64 function Source File
    \author     ZGL
    \version    V1.3.2
    \date       21. April 2022
******************************************************************************/

#include "w25q64.h"
#include "spi.h"
#include "hal_conf.h"
#include "delay.h"

#ifndef IS_PROCESS_MCU

#pragma GCC push_options
#pragma GCC optimize("O0")

#define SPI3_NSS_RESET()    GPIOD->BRR = GPIO_Pin_15
#define SPI3_NSS_SET()      GPIOD->BSRR = GPIO_Pin_15

static volatile unsigned int w25q_uid = 0;

void w25q64_read_uid(void) {
    unsigned char uid_1, uid_2, uid_3;
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(0x9f);
    uid_1 = spi3_readwrite_byte(0x00);
    uid_2 = spi3_readwrite_byte(0x00);
    uid_3 = spi3_readwrite_byte(0x00);
    SPI3_NSS_SET();
    w25q_uid = uid_1 << 16 | uid_2 << 8 | uid_3;
    if (w25q_uid != 0x00EF4017)
        while (1);

    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(0x66);
    SPI3_NSS_SET();

    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(0x99);
    SPI3_NSS_SET();
    delayus(35);
}

unsigned char w25q64_read_sr(void) {
    static volatile unsigned char byte = 0;
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q64_READ_STATUS_REG);
    byte = spi3_readwrite_byte(0xff);
    SPI3_NSS_SET();
    return byte;
}

void w25q64_write_sr(unsigned char sr) {
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q64_WRITE_STATUS_REG);
    spi3_readwrite_byte(sr);
    SPI3_NSS_SET();
}

void w25q64_write_enable(void) {
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q64_WRITE_ENABLE);
    SPI3_NSS_SET();
}

void w25q64_write_disable(void) {
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q64_WRITE_DISABLE);
    SPI3_NSS_SET();
}

void w25q64_read(unsigned char *p_buffer, unsigned int read_addr, unsigned short num_byte_to_read) {
    unsigned short i;
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q64_READ_DATA);
    spi3_readwrite_byte((unsigned char) ((read_addr) >> 16));
    spi3_readwrite_byte((unsigned char) ((read_addr) >> 8));
    spi3_readwrite_byte((unsigned char) read_addr);
    for (i = 0; i < num_byte_to_read; i++) {
        p_buffer[i] = spi3_readwrite_byte(0XFF);
    }
    SPI3_NSS_SET();
}

static void w25q64_write_page(unsigned char *p_buffer,
                              unsigned int write_addr,
                              unsigned short num_byte_to_write) {
    static unsigned short i = 0;
    w25q64_write_enable();                    //SET WEL
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q64_PAGE_PROGRAM);
    spi3_readwrite_byte((unsigned char) ((write_addr >> 16) & 0x000000FFUL));
    spi3_readwrite_byte((unsigned char) ((write_addr >> 8) & 0x000000FFUL));
    spi3_readwrite_byte((unsigned char) (write_addr & 0x000000FFUL));
    for (i = 0; i < num_byte_to_write; i++)
        spi3_readwrite_byte(p_buffer[i]);
    SPI3_NSS_SET();
    w25q64_wait_busy();
}

static void w25q64_write_no_check(unsigned char *p_buffer,
                                  unsigned int write_addr,
                                  unsigned short num_byte_to_write) {
    static unsigned short pageremain = 0;
    pageremain = 256 - write_addr % 256;
    if (num_byte_to_write <= pageremain)
        pageremain = num_byte_to_write;
    while (1) {
        w25q64_write_page(p_buffer, write_addr, pageremain);
        if (num_byte_to_write == pageremain)
            break;
        else { //num_byte_to_write>pageremain
            p_buffer += pageremain;
            write_addr += pageremain;
            num_byte_to_write -= pageremain;
            if (num_byte_to_write > 256)
                pageremain = 256;
            else
                pageremain = num_byte_to_write;
        }
    };
}

static unsigned char w25q64_buffer[4096] = {0};
void w25q64_write(unsigned char *p_buffer, unsigned int write_addr, unsigned short num_byte_to_write) {
    static volatile unsigned int secpos = 0, i = 0;
    static volatile unsigned short secoff = 0, secremain = 0;
    secpos = write_addr / 4096;
    secoff = write_addr % 4096;
    secremain = 4096 - secoff;
    if (num_byte_to_write <= secremain)
        secremain = num_byte_to_write;
    while (1) {
        w25q64_read((unsigned char *) w25q64_buffer, secpos * 4096, 4096);
        for (i = 0; i < secremain; i++) {
            if (w25q64_buffer[secoff + i] != 0xFF)
                break;
        }
        if (i < secremain) {
            w25q64_erase_sector(secpos);
            for (i = 0; i < secremain; i++)
                w25q64_buffer[i + secoff] = p_buffer[i];
            w25q64_write_no_check(w25q64_buffer, secpos * 4096, 4096);
        } else
            w25q64_write_no_check(p_buffer, write_addr, secremain);
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

void w25q64_erase_chip(void) {
    w25q64_write_enable();                        //SET WEL
    w25q64_wait_busy();
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q64_CHIP_ERASE);
    SPI3_NSS_SET();
    w25q64_wait_busy();
}

void w25q64_erase_sector(unsigned int dst_addr) {
    dst_addr *= 4096;
    w25q64_write_enable();                    //SET WEL
    w25q64_wait_busy();
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(w25q64_SECTOR_ERASE);
    spi3_readwrite_byte((unsigned char) ((dst_addr) >> 16));
    spi3_readwrite_byte((unsigned char) ((dst_addr) >> 8));
    spi3_readwrite_byte((unsigned char) dst_addr);
    SPI3_NSS_SET();
    w25q64_wait_busy();
}

void w25q64_wait_busy(void) {
    while ((w25q64_read_sr() & 0x01) == 0x01)
        delayms(1);
}

void w25q64_power_down(void) {
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q64_POWER_DOMN);
    SPI3_NSS_SET();
    delayus(3);
}

void w25q64_wake_up(void) {
    SPI3_NSS_RESET();
    delayus(1);
    spi3_readwrite_byte(W25Q64_RELEASE_POWER_DOWM);    //  send w25q64_POWER_DOMN command 0xAB
    SPI3_NSS_SET();
    delayus(3);
}

unsigned char w25q64_test_flash(void) {
    static unsigned char write_data[128] = {0};
    static unsigned char read_data[128] = {0};
    w25q64_read_uid();
    w25q64_wake_up();
    for (int counter = 0; counter < 1024; ++counter) {
        w25q64_write(write_data, counter * 1024 + 512, sizeof(write_data));
        w25q64_read(read_data, counter * 1024 + 512, sizeof(write_data));
        for (unsigned int i = 0; i < sizeof(write_data); ++i) {
            if (write_data[i] != read_data[i])
                return 1;
            else
                write_data[i] = (global_time_stamp + i) % 0xff;
        }
    }
    return 0;
}
#pragma GCC pop_options

#endif
