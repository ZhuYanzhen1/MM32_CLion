/**************************************************************************/ /**
    \file     spi.h
    \brief    SPI function Header File
    \author   Lao·Zhu
    \version  V1.2.2
    \date     19. January 2022
******************************************************************************/

#ifndef MM32F3277_HARDWARE_SPI_H_
#define MM32F3277_HARDWARE_SPI_H_

/* Not yet determined */
#define SPI_CS_GPIO_Port GPIO_PortSourceGPIOA

/* Not yet determined */
#define SPI_CS_Pin GPIO_Pin_0

void spi2_config(void);
unsigned int spi2_readwrite_byte(unsigned int tx_data);
void spi1_config(void);
unsigned int spi1_readwrite_byte(unsigned int tx_data);
void spi3_config(void);
unsigned int spi3_readwrite_byte(unsigned int tx_data);
void software_spi3_init(void);
unsigned int software_spi3_mode3(unsigned int write_data);

#endif  // MM32F3277_HARDWARE_SPI_H_
