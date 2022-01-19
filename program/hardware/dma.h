/**************************************************************************/ /**
    \file       gps.c
    \brief      DMA function Header File
    \author     ZGL
    \version    V1.0.1
    \date       19. January 2022
******************************************************************************/

#ifndef MM32F3277_HARDWARE_DMA_H_
#define MM32F3277_HARDWARE_DMA_H_

void uart6_dma_receive_config(const unsigned int *data_address, unsigned short data_length);
void uart6_dma_nvic_config();

void uart1_dma_sent_config(const unsigned int *data_address, unsigned short data_length);
void uart1_dma_nvic_config();

#endif // MM32F3277_HARDWARE_DMA_H_
