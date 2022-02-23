/**************************************************************************/ /**
    \file       dma.h
    \brief      DMA function Header File
    \author     ZGL
    \version    V1.2.2
    \date       19. January 2022
******************************************************************************/

#ifndef MM32F3277_HARDWARE_DMA_H_
#define MM32F3277_HARDWARE_DMA_H_

void uart6_dma_set_transmit_buffer(const unsigned int *data_address, unsigned short data_length);
void uart6_dma_receive_config(const unsigned int *data_address, unsigned short data_length);
void uart6_dma_nvic_config();

void uart1_dma_set_transmit_buffer(const unsigned int *data_address, unsigned short data_length);
void uart1_dma_sent_config(const unsigned int *data_address, unsigned short data_length);
void uart1_dma_nvic_config();

void spi2_dma_set_transmit_buffer(const unsigned int *data_address, unsigned short data_length);
void spi2_dma_sent_config(const unsigned int *data_address, unsigned short data_length);

#endif // MM32F3277_HARDWARE_DMA_H_
