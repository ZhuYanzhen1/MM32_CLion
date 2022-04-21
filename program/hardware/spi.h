/**************************************************************************/ /**
    \file     spi.h
    \brief    SPI function Header File
    \author   Lao·Zhu
    \version  V1.3.2
    \date     06. March 2022
******************************************************************************/

#ifndef MM32F3277_HARDWARE_SPI_H_
#define MM32F3277_HARDWARE_SPI_H_

#define SPI3_NSS_RESET()    GPIOD->BRR = GPIO_Pin_15;
#define SPI3_NSS_SET()      GPIOD->BSRR = GPIO_Pin_15;

void spi2_config(void);
unsigned int spi2_readwrite_byte(unsigned int tx_data);

void spi3_config(void);
#ifdef IS_PROCESS_MCU
void spi3_readwrite_byte(unsigned int tx_data);
#else
unsigned int spi3_readwrite_byte(unsigned int tx_data);
#endif

#endif  // MM32F3277_HARDWARE_SPI_H_
