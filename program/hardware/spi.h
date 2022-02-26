/**************************************************************************/ /**
    \file     spi.h
    \brief    SPI function Header File
    \author   Lao·Zhu
    \version  V1.2.2
    \date     19. January 2022
******************************************************************************/

#ifndef MM32F3277_HARDWARE_SPI_H_
#define MM32F3277_HARDWARE_SPI_H_

#define SPI3_MOSI_HIGH  GPIO_SetBits(SPI3_MOSI_PORT, SPI3_MOSI_PIN)
#define SPI3_MOSI_LOW   GPIO_ResetBits(SPI3_MOSI_PORT, SPI3_MOSI_PIN)
#define SPI3_SCK_HIGH   GPIO_SetBits(SPI3_SCK_PORT, SPI3_SCK_PIN)
#define SPI3_SCK_LOW    GPIO_ResetBits(SPI3_SCK_PORT, SPI3_SCK_PIN)
#define SPI3_MISO       GPIO_ReadInputDataBit(SPI3_MISO_PORT, SPI3_MISO_PIN)

/* Not yet determined */
#define SPI_CS_GPIO_Port GPIO_PortSourceGPIOA

/* Not yet determined */
#define SPI_CS_Pin GPIO_Pin_0

void spi2_config(void);
void spi2_write_byte(unsigned int tx_data);
void spi2_dma_sendbuffer(void);
void spi1_config(void);
unsigned int spi1_readwrite_byte(unsigned int tx_data);
void spi3_config(void);
unsigned int spi3_readwrite_byte(unsigned int tx_data);
void spi3_software_init(void);
short spi3_software_mode3(unsigned int write_data);

#endif  // MM32F3277_HARDWARE_SPI_H_
