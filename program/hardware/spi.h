/*****************************************************************************
    \file     spi.h
    \brief    spi function Header File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     24. November 2021
******************************************************************************/

#ifndef MM32F3277_HARDWARE_SPI_H_
#define MM32F3277_HARDWARE_SPI_H_

#define SPI_CS_GPIO_Port GPIO_PortSourceGPIOA /* Not yet determined */
#define SPI_CS_Pin GPIO_Pin_0                 /* Not yet determined */

void spi2_config(void);
unsigned int spi2_readwrite_byte(unsigned int tx_data);

#endif  // MM32F3277_HARDWARE_SPI_H_
