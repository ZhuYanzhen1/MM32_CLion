/**************************************************************************/ /**
    \file     spi.h
    \brief    SPI function Header File
    \author   Lao·Zhu
    \version  V1.3.2
    \date     06. March 2022
******************************************************************************/

#ifndef MM32F3277_HARDWARE_SPI_H_
#define MM32F3277_HARDWARE_SPI_H_

#ifdef IS_PROCESS_MCU
void spi2_config(void);
unsigned int spi2_readwrite_byte(unsigned int tx_data);
void spi3_config(void);
void spi3_readwrite_byte(unsigned int tx_data);
#else
void spi3_config(void);
unsigned char spi3_readwrite_byte(unsigned char tx_data);
#endif

#endif  // MM32F3277_HARDWARE_SPI_H_
