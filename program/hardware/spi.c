/**************************************************************************/ /**
    \file     spi.c
    \brief    SPI function Source File
    \author   Lao·Zhu
    \version  V1.3.2
    \date     06. March 2022
******************************************************************************/

#include "spi.h"
#include "hal_conf.h"
#include "hal_gpio.h"

#ifdef IS_PROCESS_MCU
unsigned int spi2_readwrite_byte(unsigned int tx_data) {
    SPI_SendData(SPI2, tx_data);
    while (1) {
        if (SPI_GetFlagStatus(SPI2, SPI_FLAG_RXAVL))
            return SPI_ReceiveData(SPI2);
    }
}

void spi2_config(void) {
    SPI_InitTypeDef SPI_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOE, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_SPI2, ENABLE);

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_5);    // CLK
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_5);    // MISO
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_5);    // MOSI

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOE, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOE, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIO_InitStruct);

    SPI_StructInit(&SPI_InitStruct);
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

    /* 120MHz / 128 = 0.94MHz */
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI2, &SPI_InitStruct);
    if (SPI_InitStruct.SPI_BaudRatePrescaler <= 8)
        exSPI_DataEdgeAdjust(SPI2, SPI_DataEdgeAdjust_FAST);
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
    SPI_Cmd(SPI2, ENABLE);
}

void spi3_readwrite_byte(unsigned int tx_data) {
    SPI_SendData(SPI3, tx_data);
    while (1) {
        if (SPI_GetFlagStatus(SPI3, SPI_FLAG_TXEPT))
            break;
    }
}
void spi3_config(void) {
    SPI_InitTypeDef SPI_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_SPI3, ENABLE);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_5);    // CLK
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_5);    // MOSI
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_5);    // NSS

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    SPI_StructInit(&SPI_InitStruct);
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Hard;

    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI3, &SPI_InitStruct);
    if (SPI_InitStruct.SPI_BaudRatePrescaler <= 8)
        exSPI_DataEdgeAdjust(SPI3, SPI_DataEdgeAdjust_FAST);

    SPI_BiDirectionalLineConfig(SPI3, SPI_Direction_Tx);
    SPI_Cmd(SPI3, ENABLE);
}
#else

void spi3_set_prescaler(SPI_BaudRatePrescaler_TypeDef SPI_BaudRatePrescaler) {
    MODIFY_REG(SPI3->BRR, BRR_Mask, SPI_BaudRatePrescaler);
}

unsigned char spi3_readwrite_byte(unsigned char tx_data) {
    __asm volatile("cpsid i");
    WRITE_REG(SPI3->TDR, tx_data);
    __asm volatile("cpsie i");
    while (1) {
        if ((SPI3->SR & SPI_FLAG_RXAVL) != 0) {
            unsigned char result;
            __asm volatile("cpsid i");
            result = READ_REG(SPI3->RDR);
            __asm volatile("cpsie i");
            return result;
        }
    }
}

void spi3_config(void) {
    SPI_InitTypeDef SPI_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_SPI3, ENABLE);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_6);   // CLK
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_6);   // MISO
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_6);   // MOSI

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14;    // SCLK
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;    // NSS
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    GPIO_SetBits(GPIOD, GPIO_Pin_15);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;    // MISO
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    SPI_StructInit(&SPI_InitStruct);
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI3, &SPI_InitStruct);
    if (SPI_InitStruct.SPI_BaudRatePrescaler <= 8)
        exSPI_DataEdgeAdjust(SPI3, SPI_DataEdgeAdjust_FAST);
    SPI_BiDirectionalLineConfig(SPI3, SPI_Direction_Tx);
    SPI_BiDirectionalLineConfig(SPI3, SPI_Direction_Rx);
    SPI_Cmd(SPI3, ENABLE);
}
#endif  // IS_PROCESS_MCU
