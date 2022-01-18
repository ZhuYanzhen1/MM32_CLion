//
// Created by Lao·Zhu on 2022/1/18.
//

#ifndef MM32F3277_HARDWARE_DMA_H_
#define MM32F3277_HARDWARE_DMA_H_

void dma_receive_config(const unsigned int *data_address, unsigned short data_length);
void dma_nvic_config(unsigned char priority, unsigned char sub_priority);

#endif // MM32F3277_HARDWARE_DMA_H_
