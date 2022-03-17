/**************************************************************************/ /**
    \file     spi.h
    \brief    SPI function Header File
    \author   Lao·Zhu
    \version  V1.3.2
    \date     06. March 2022
******************************************************************************/

#ifndef MM32F3277_HARDWARE_SPI_H_
#define MM32F3277_HARDWARE_SPI_H_

#define SPI3_MOSI_HIGH  GPIO_SetBits(SPI3_MOSI_PORT, SPI3_MOSI_PIN)
#define SPI3_MOSI_LOW   GPIO_ResetBits(SPI3_MOSI_PORT, SPI3_MOSI_PIN)
#define SPI3_SCK_HIGH   GPIO_SetBits(SPI3_SCK_PORT, SPI3_SCK_PIN)
#define SPI3_SCK_LOW    GPIO_ResetBits(SPI3_SCK_PORT, SPI3_SCK_PIN)
#define SPI3_MISO       GPIO_ReadInputDataBit(SPI3_MISO_PORT, SPI3_MISO_PIN)

void spi2_config(void);
unsigned int spi2_readwrite_byte(unsigned int tx_data);

void spi3_config(void);
void spi3_readwrite_byte(unsigned int tx_data);

void spi3_software_init(void);
short spi3_software_mode3(unsigned int write_data);

#endif  // MM32F3277_HARDWARE_SPI_H_
