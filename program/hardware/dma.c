//
// Created by Lao·Zhu on 2022/1/18.
//

#include "dma.h"
#include "hal_conf.h"

void dma_receive_config(const unsigned char *data_address, unsigned short data_length) {
    DMA_InitTypeDef DMA_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_DMA1, ENABLE);

    DMA_DeInit(DMA1_Channel1);
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_PeripheralBaseAddr = (unsigned int) &UART6->RDR;
    DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned int) data_address;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_BufferSize = data_length;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStruct.DMA_Auto_reload = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStruct);

    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    UART_DMACmd(UART6, UART_GCR_DMA, ENABLE);
}

void dma_nvic_config(unsigned char priority, unsigned char sub_priority) {
    exNVIC_Init_TypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = priority;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = sub_priority;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    exNVIC_Init(&NVIC_InitStruct);
}
