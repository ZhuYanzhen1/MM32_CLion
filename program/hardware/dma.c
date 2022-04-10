/**************************************************************************/ /**
    \file       dma.c
    \brief      DMA function Source File
    \author     ZGL
    \version    V1.3.2
    \date       19. March 2022
******************************************************************************/

#include "dma.h"
#include "hal_conf.h"
#ifdef IS_CONTROL_MCU
#include "../ctrl_mcu/config.h"
#else
#include "../proc_mcu/config.h"
#endif  // IS_CONTROL_MCU

#ifdef IS_CONTROL_MCU
void uart7_dma_set_send_buffer(const unsigned int *data_address, unsigned short data_length) {
    DMA2_Channel2->CNDTR = (volatile unsigned int) data_length;
    DMA2_Channel2->CMAR = (volatile unsigned int) data_address;
    MODIFY_REG(DMA2_Channel2->CCR, DMA_CCR_EN, 1 << DMA_CCR_EN_Pos);
}

void uart7_dma_sent_config(const unsigned int *data_address, unsigned short data_length) {
    DMA_InitTypeDef DMA_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_DMA2, ENABLE);

    DMA_DeInit(DMA2_Channel2);
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_PeripheralBaseAddr = (unsigned int) &UART7->TDR;
    DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned int) data_address;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_BufferSize = data_length;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStruct.DMA_Auto_reload = DMA_Auto_Reload_Disable;
    DMA_Init(DMA2_Channel2, &DMA_InitStruct);

    UART_DMACmd(UART7, UART_GCR_DMA, ENABLE);
    DMA_Cmd(DMA2_Channel2, DISABLE);
}

void uart6_dma_set_transmit_buffer(const unsigned int *data_address, unsigned short data_length) {
    DMA1_Channel1->CNDTR = (volatile unsigned int) data_length;
    DMA1_Channel1->CMAR = (volatile unsigned int) data_address;
    MODIFY_REG(DMA1_Channel1->CCR, DMA_CCR_EN, 1 << DMA_CCR_EN_Pos);
}

void uart6_dma_receive_config(const unsigned int *data_address, unsigned short data_length) {
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
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStruct.DMA_Auto_reload = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStruct);

    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    UART_DMACmd(UART6, UART_GCR_DMA, ENABLE);
    DMA_Cmd(DMA1_Channel1, DISABLE);
}

void uart6_dma_nvic_config() {
    exNVIC_Init_TypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = UART6_DMA_PRIORITY;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    exNVIC_Init(&NVIC_InitStruct);
}
#endif

#ifdef IS_PROCESS_MCU
void uart3_dma_set_transmit_buffer(const unsigned int *data_address, unsigned short data_length) {
    DMA1_Channel3->CNDTR = (volatile unsigned int) data_length;
    DMA1_Channel3->CMAR = (volatile unsigned int) data_address;
    MODIFY_REG(DMA1_Channel3->CCR, DMA_CCR_EN, 1 << DMA_CCR_EN_Pos);
}

void uart3_dma_receive_config(const unsigned int *data_address, unsigned short data_length) {
    DMA_InitTypeDef DMA_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_DMA1, ENABLE);

    DMA_DeInit(DMA1_Channel3);
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_PeripheralBaseAddr = (unsigned int) &UART3->RDR;
    DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned int) data_address;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_BufferSize = data_length;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStruct.DMA_Auto_reload = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStruct);

    DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
    UART_DMACmd(UART3, UART_GCR_DMA, ENABLE);
    DMA_Cmd(DMA1_Channel3, DISABLE);
}

void uart3_dma_nvic_config() {
    exNVIC_Init_TypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = UART3_DMA_PRIORITY;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    exNVIC_Init(&NVIC_InitStruct);
}
#endif  // IS_PROCESS_MCU

void uart1_dma_set_transmit_buffer(const unsigned int *data_address, unsigned short data_length) {
    DMA1_Channel4->CNDTR = (volatile unsigned int) data_length;
    DMA1_Channel4->CMAR = (volatile unsigned int) data_address;
    MODIFY_REG(DMA1_Channel4->CCR, DMA_CCR_EN, 1 << DMA_CCR_EN_Pos);
}

void uart1_dma_sent_config(const unsigned int *data_address, unsigned short data_length) {
    DMA_InitTypeDef DMA_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_DMA1, ENABLE);

    DMA_DeInit(DMA1_Channel4);
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_PeripheralBaseAddr = (unsigned int) &UART1->TDR;
    DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned int) data_address;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_BufferSize = data_length;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStruct.DMA_Auto_reload = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStruct);

    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
    UART_DMACmd(UART1, UART_GCR_DMA, ENABLE);
    DMA_Cmd(DMA1_Channel4, DISABLE);
}

void uart1_dma_nvic_config() {
    exNVIC_Init_TypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = UART1_DMA_PRIORITY;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    exNVIC_Init(&NVIC_InitStruct);
}

#ifdef IS_PROCESS_MCU
void spi3_dma_set_transmit_buffer(const unsigned int *data_address, unsigned short data_length) {
    DMA2_Channel2->CNDTR = (volatile unsigned int) data_length;
    DMA2_Channel2->CMAR = (volatile unsigned int) data_address;
    MODIFY_REG(DMA2_Channel2->CCR, DMA_CCR_EN, 1 << DMA_CCR_EN_Pos);
}

void spi3_dma_sent_config(const unsigned int *data_address, unsigned short data_length) {
    DMA_InitTypeDef DMA_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_DMA2, ENABLE);

    DMA_DeInit(DMA2_Channel2);
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_PeripheralBaseAddr = (unsigned int) &(SPI3->TDR);
    DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned int) data_address;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_BufferSize = data_length;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStruct.DMA_Auto_reload = DMA_Auto_Reload_Disable;
    DMA_Init(DMA2_Channel2, &DMA_InitStruct);

    DMA_ITConfig(DMA2_Channel2, DMA_IT_TC, ENABLE);
    SPI_DMACmd(SPI3, ENABLE);
    DMA_Cmd(DMA2_Channel2, DISABLE);
}

void spi3_dma_nvic_config() {
    exNVIC_Init_TypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = DMA2_Channel2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = SPI3_DMA_PRIORITY;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    exNVIC_Init(&NVIC_InitStruct);
}
#endif
