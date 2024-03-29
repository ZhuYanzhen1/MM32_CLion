/**************************************************************************/ /**
    \file       dma.h
    \brief      DMA function Header File
    \author     ZGL
    \version    V1.6.1
    \date       24. July 2022
******************************************************************************/

#ifndef MM32F3277_HARDWARE_DMA_H_
#define MM32F3277_HARDWARE_DMA_H_

void uart7_dma_set_send_buffer(const unsigned int *data_address, unsigned short data_length);
void uart7_dma_sent_config(const unsigned int *data_address, unsigned short data_length);

void uart6_dma_set_transmit_buffer(const unsigned int *data_address, unsigned short data_length);
void uart6_dma_receive_config(const unsigned int *data_address, unsigned short data_length);
void uart6_dma_nvic_config();

void uart3_dma_set_transmit_buffer(const unsigned int *data_address, unsigned short data_length);
void uart3_dma_receive_config(const unsigned int *data_address, unsigned short data_length);
void uart3_dma_nvic_config();

void uart2_dma_set_transmit_buffer(const unsigned int *data_address, unsigned short data_length);
void uart2_dma_receive_config(const unsigned int *data_address, unsigned short data_length);
void uart2_dma_nvic_config();

void uart1_dma_set_transmit_buffer(const unsigned int *data_address, unsigned short data_length);
void uart1_dma_sent_config(const unsigned int *data_address, unsigned short data_length);
void uart1_dma_nvic_config();

void spi3_dma_set_transmit_buffer(const unsigned int *data_address, unsigned short data_length);
void spi3_dma_sent_config(const unsigned int *data_address, unsigned short data_length);
void spi3_dma_nvic_config();

#endif // MM32F3277_HARDWARE_DMA_H_
