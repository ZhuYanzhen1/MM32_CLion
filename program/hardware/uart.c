/**************************************************************************/ /**
    \file     uart.c
    \brief    UART function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "uart.h"
#include "printf.h"

void uart1_config(unsigned int baudrate) {
    UART_InitTypeDef UART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2ENR_UART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    UART_StructInit(&UART_InitStruct);

    UART_InitStruct.BaudRate = baudrate;
    UART_InitStruct.WordLength = UART_WordLength_8b;
    UART_InitStruct.StopBits = UART_StopBits_1;
    UART_InitStruct.Parity = UART_Parity_No;
    UART_InitStruct.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStruct.Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART1, &UART_InitStruct);
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
    UART_Cmd(UART1, ENABLE);
}

void uart3_config(unsigned int baudrate) {
    UART_InitTypeDef UART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB1ENR_UART3, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOD, ENABLE);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_7);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_7);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = UART3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate = baudrate;
    UART_InitStruct.WordLength = UART_WordLength_8b;
    UART_InitStruct.StopBits = UART_StopBits_1;
    UART_InitStruct.Parity = UART_Parity_No;
    UART_InitStruct.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStruct.Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART3, &UART_InitStruct);
    UART_ITConfig(UART3, UART_IT_RXIEN, ENABLE);
    UART_Cmd(UART3, ENABLE);
}

void uart6_config(unsigned int baudrate) {
    UART_InitTypeDef UART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2ENR_UART6, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_8);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_8);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = UART6_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    UART_StructInit(&UART_InitStruct);

    UART_InitStruct.BaudRate = baudrate;
    UART_InitStruct.WordLength = UART_WordLength_8b;
    UART_InitStruct.StopBits = UART_StopBits_1;
    UART_InitStruct.Parity = UART_Parity_No;
    UART_InitStruct.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStruct.Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART6, &UART_InitStruct);
    UART_ITConfig(UART6, UART_IT_RXIEN, ENABLE);
    UART_Cmd(UART6, ENABLE);
}

void uart6_sendbyte(unsigned char data) {
    UART_SendData(UART6, data);
    while (!UART_GetFlagStatus(UART6, UART_FLAG_TXEPT));
}

void uart3_sendbyte(unsigned char data) {
    UART_SendData(UART3, data);
    while (!UART_GetFlagStatus(UART3, UART_FLAG_TXEPT));
}

void uart1_sendbyte(unsigned char data) {
    UART_SendData(UART1, data);
    while (!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
}

void uart6_dma_init(unsigned int baudrate) {
    UART_InitTypeDef UART_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2ENR_UART6, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_8);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_8);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    UART_StructInit(&UART_InitStruct);

    UART_InitStruct.BaudRate = baudrate;
    UART_InitStruct.WordLength = UART_WordLength_8b;
    UART_InitStruct.StopBits = UART_StopBits_1;
    UART_InitStruct.Parity = UART_Parity_No;
    UART_InitStruct.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStruct.Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART6, &UART_InitStruct);
//    UART_ITConfig(UART6, UART_IT_RXIEN, ENABLE);
    UART_Cmd(UART6, ENABLE);
}

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
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
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
