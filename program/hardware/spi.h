/*****************************************************************************
    \file     spi.h
    \brief    spi function Header File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     24. November 2021
******************************************************************************/

#ifndef MM32F3277_HARDWARE_SPI_H_
#define MM32F3277_HARDWARE_SPI_H_

void spi2_config(void);
unsigned int spi2_readwrite_byte(unsigned int tx_data);

#endif  // MM32F3277_HARDWARE_SPI_H_
