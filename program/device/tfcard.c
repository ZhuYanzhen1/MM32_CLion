/**************************************************************************/ /**
    \file       tfcard.c
    \brief      tfcard function Source File
    \author     LaoZhu
    \version    V1.3.2
    \date       21. April 2022
******************************************************************************/

#include "tfcard.h"
#include "spi.h"
#include "hal_conf.h"
#include "delay.h"

#define CMD0    0
#define CMD1    1
#define CMD8    8
#define CMD9    9
#define CMD12   12
#define CMD16   16
#define CMD17   17
#define CMD18   18
#define CMD23   23
#define CMD24   24
#define CMD25   25
#define CMD41   41
#define CMD55   55
#define CMD58   58

#ifndef IS_PROCESS_MCU

#pragma GCC push_options
#pragma GCC optimize("O0")

unsigned char SD_TYPE = 0x00;

void sdcard_sync(void) {
    GPIO_ResetBits(GPIOD, GPIO_Pin_15);
    do {
        delayms(20);
    } while (spi3_readwrite_byte(0xFF) != 0xFF);
    GPIO_SetBits(GPIOD, GPIO_Pin_15);
}

int sdcard_send_cmd(unsigned char cmd, unsigned int arg, unsigned char crc) {
    unsigned char result;
    unsigned char retry;
    GPIO_SetBits(GPIOD, GPIO_Pin_15);
    delayms(20);
    GPIO_ResetBits(GPIOD, GPIO_Pin_15);
    do {
        retry = spi3_readwrite_byte(0xFF);
    } while (retry != 0xFF);
    spi3_readwrite_byte(cmd | 0x40);
    spi3_readwrite_byte(arg >> 24);
    spi3_readwrite_byte(arg >> 16);
    spi3_readwrite_byte(arg >> 8);
    spi3_readwrite_byte(arg);
    spi3_readwrite_byte(crc);
    if (cmd == CMD12)
        spi3_readwrite_byte(0xFF);
    do {
        result = spi3_readwrite_byte(0xFF);
    } while (result & 0X80);
    return result;
}

unsigned char sdcard_config(void) {
    unsigned char result;
    unsigned char buffer[6] = {0};
    unsigned short retry;
    unsigned char counter;

    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOD, ENABLE);
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    GPIO_SetBits(GPIOD, GPIO_Pin_10);

    spi3_set_prescaler(SPI_BaudRatePrescaler_256);
    GPIO_SetBits(GPIOD, GPIO_Pin_15);
    for (retry = 0; retry < 10; retry++)
        spi3_readwrite_byte(0xFF);

    do {
        result = sdcard_send_cmd(CMD0, 0, 0x95);
    } while (result != 0x01);

    SD_TYPE = 0;
    result = sdcard_send_cmd(CMD8, 0x1AA, 0x87);
    if (result == 0x01) {
        for (counter = 0; counter < 4; counter++)buffer[counter] = spi3_readwrite_byte(0xFF);
        if (buffer[2] == 0X01 && buffer[3] == 0XAA) {
            retry = 0XFFFE;
            do {
                sdcard_send_cmd(CMD55, 0, 0X01);
                result = sdcard_send_cmd(CMD41, 0x40000000, 0X01);
            } while (result && retry--);
            if (retry && sdcard_send_cmd(CMD58, 0, 0X01) == 0) {
                for (counter = 0; counter < 4; counter++)buffer[counter] = spi3_readwrite_byte(0XFF);
                if (buffer[0] & 0x40)
                    SD_TYPE = SD_TYPE_V2HC;
                else
                    SD_TYPE = SD_TYPE_V2;
            }
        } else {
            sdcard_send_cmd(CMD55, 0, 0X01);
            result = sdcard_send_cmd(CMD41, 0, 0X01);
            if (result <= 1) {
                SD_TYPE = SD_TYPE_V1;
                retry = 0XFFFE;
                do {
                    sdcard_send_cmd(CMD55, 0, 0X01);
                    result = sdcard_send_cmd(CMD41, 0, 0X01);
                } while (result && retry--);
            } else {
                SD_TYPE = SD_TYPE_MMC;
                retry = 0XFFFE;
                do {
                    result = sdcard_send_cmd(CMD1, 0, 0X01);
                } while (result && retry--);
            }
            if (retry == 0 || sdcard_send_cmd(CMD16, 512, 0X01) != 0)
                SD_TYPE = SD_TYPE_ERR;
        }
    }
    GPIO_SetBits(GPIOD, GPIO_Pin_15);
    spi3_set_prescaler(SPI_BaudRatePrescaler_2);
    if (SD_TYPE)
        return 0;
    else
        return 1;
}

unsigned char sdcard_receive_data(unsigned char *data, unsigned short len) {
    unsigned char result;
    GPIO_ResetBits(GPIOD, GPIO_Pin_15);
    do {
        result = spi3_readwrite_byte(0xFF);
        delayms(100);
    } while (result != 0xFE);
    while (len--) {
        *data = spi3_readwrite_byte(0xFF);
        data++;
    }
    spi3_readwrite_byte(0xFF);
    spi3_readwrite_byte(0xFF);
    return 0;
}

unsigned char sdcard_write_block(unsigned char *buf, unsigned char cmd) {
    unsigned short counter;
    unsigned char result;
    do {
        result = spi3_readwrite_byte(0xFF);
    } while (result != 0xFF);

    spi3_readwrite_byte(cmd);
    if (cmd != 0XFD) {
        for (counter = 0; counter < 512; counter++)
            spi3_readwrite_byte(buf[counter]);
        spi3_readwrite_byte(0xFF);
        spi3_readwrite_byte(0xFF);
        counter = spi3_readwrite_byte(0xFF);
        if ((counter & 0x1F) != 0x05)
            return 2;
    }
    return 0;
}

unsigned char sdcard_get_csd(unsigned char *csd_data) {
    unsigned char result;
    result = sdcard_send_cmd(CMD9, 0, 0x01);
    if (result == 0)
        result = sdcard_receive_data(csd_data, 16);
    GPIO_SetBits(GPIOD, GPIO_Pin_15);
    if (result)
        return 1;
    else
        return 0;
}

unsigned int sdcard_get_sector_num(void) {
    unsigned char csd[16];
    unsigned int capacity;
    unsigned char number;
    unsigned short csize;
    if (sdcard_get_csd(csd) != 0) return 0;
    if ((csd[0] & 0xC0) == 0x40) {
        csize = csd[9] + ((unsigned short) csd[8] << 8) + 1;
        capacity = (unsigned int) csize << 10;
    } else {
        number = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((unsigned short) csd[7] << 2) + ((unsigned short) (csd[6] & 3) << 10) + 1;
        capacity = (unsigned int) csize << (number - 9);
    }
    return capacity;
}

unsigned char sdcard_write_disk(unsigned char *buf, unsigned int sector, unsigned char cnt) {
    unsigned char result;
    if (SD_TYPE != SD_TYPE_V2HC)
        sector *= 512;
    if (cnt == 1) {
        result = sdcard_send_cmd(CMD24, sector, 0X01);
        if (result == 0)
            result = sdcard_write_block(buf, 0xFE);
    } else {
        if (SD_TYPE != SD_TYPE_MMC) {
            sdcard_send_cmd(CMD55, 0, 0X01);
            sdcard_send_cmd(CMD23, cnt, 0X01);
        }
        result = sdcard_send_cmd(CMD25, sector, 0X01);
        if (result == 0) {
            do {
                result = sdcard_write_block(buf, 0xFC);
                buf += 512;
            } while (--cnt && result == 0);
            result = sdcard_write_block(0, 0xFD);
        }
    }
    GPIO_SetBits(GPIOD, GPIO_Pin_15);
    return result;
}

unsigned char sdcard_read_disk(unsigned char *buf, unsigned int sector, unsigned char cnt) {
    unsigned char result;
    if (SD_TYPE != SD_TYPE_V2HC)
        sector <<= 9;
    if (cnt == 1) {
        result = sdcard_send_cmd(CMD17, sector, 0X01);
        if (result == 0) {
            result = sdcard_receive_data(buf, 512);
        }
    } else {
        result = sdcard_send_cmd(CMD18, sector, 0X01);
        do {
            result = sdcard_receive_data(buf, 512);
            buf += 512;
        } while (--cnt && result == 0);
        sdcard_send_cmd(CMD12, 0, 0X01);
    }
    GPIO_SetBits(GPIOD, GPIO_Pin_15);
    return result;
}

#pragma GCC pop_options

#endif
