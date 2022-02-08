/**************************************************************************/ /**
    \file     spi.c
    \brief    SPI function Source File
    \author   Lao·Zhu
    \version  V1.2.2
    \date     19. January 2022
******************************************************************************/

#include "spi.h"
#include "hal_conf.h"
#include "hal_gpio.h"
#include "delay.h"

unsigned int spi1_readwrite_byte(unsigned int tx_data) {
    SPI_SendData(SPI1, tx_data);
    while (1) {
        if (SPI_GetFlagStatus(SPI1, SPI_FLAG_RXAVL))
            return SPI_ReceiveData(SPI1);
    }
}

void spi1_config(void) {
    SPI_InitTypeDef SPI_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB2ENR_SPI1, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_5);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    SPI_StructInit(&SPI_InitStruct);
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

    // 120MHz / 64 = 1.875MHz
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI1, &SPI_InitStruct);
    if (SPI_InitStruct.SPI_BaudRatePrescaler <= 8)
        exSPI_DataEdgeAdjust(SPI1, SPI_DataEdgeAdjust_FAST);

    SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Rx);
    SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Tx);
    SPI_Cmd(SPI1, ENABLE);
}

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

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_5);

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

    /* 120MHz / 16 = 7.5MHz */
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI2, &SPI_InitStruct);
    if (SPI_InitStruct.SPI_BaudRatePrescaler <= 8)
        exSPI_DataEdgeAdjust(SPI2, SPI_DataEdgeAdjust_FAST);

    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);
    SPI_Cmd(SPI2, ENABLE);
}

unsigned int spi3_readwrite_byte(unsigned int tx_data) {
    SPI_SendData(SPI3, tx_data);
    while (1) {
        if (SPI_GetFlagStatus(SPI3, SPI_FLAG_RXAVL))
            return SPI_ReceiveData(SPI3);
    }
}

void spi3_config(void) {
    SPI_InitTypeDef SPI_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_SPI3, ENABLE);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_5);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_5);
    // 如果你的CS引脚由软件控制，不要把它复用了

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;      // PD5才是MISO，要配置成上拉输入
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;   // 如果你的CS引脚由软件控制，记得初始化成通用推挽输出
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    SPI_StructInit(&SPI_InitStruct);
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_32b;
    SPI_InitStruct.SPI_DataWidth = SPI_DataWidth_16b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_LSB;
    SPI_Init(SPI3, &SPI_InitStruct);
    if (SPI_InitStruct.SPI_BaudRatePrescaler <= 8)
        exSPI_DataEdgeAdjust(SPI3, SPI_DataEdgeAdjust_FAST);

    SPI_BiDirectionalLineConfig(SPI3, SPI_Direction_Rx);
    SPI_BiDirectionalLineConfig(SPI3, SPI_Direction_Tx);
    SPI_Cmd(SPI3, ENABLE);
}

#define SPI3_SCK_PIN        GPIO_Pin_4
#define SPI3_SCK_GPIO_PORT  GPIOD
#define SPI3_MISO_PIN       GPIO_Pin_5
#define SPI3_MISO_GPIO_PORT GPIOD
#define SPI3_MOSI_PIN       GPIO_Pin_6
#define SPI3_MOSI_GPIO_PORT GPIOD
#define SPI3_NSS_PIN        GPIO_Pin_7
#define SPI3_NSS_GPIO_PORT  GPIOD

#define SPI3_MOSI_HIGH      GPIO_SetBits(SPI3_MOSI_GPIO_PORT, SPI3_MOSI_PIN)
#define SPI3_MOSI_LOW       GPIO_ResetBits(SPI3_MOSI_GPIO_PORT, SPI3_MOSI_PIN)
#define SPI3_SCK_HIGH       GPIO_SetBits(SPI3_SCK_GPIO_PORT, SPI3_SCK_PIN)
#define SPI3_SCK_LOW        GPIO_ResetBits(SPI3_SCK_GPIO_PORT, SPI3_SCK_PIN)
#define SPI3_MISO           GPIO_ReadInputDataBit(SPI3_MISO_GPIO_PORT, SPI3_MISO_PIN)

void software_spi3_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOD, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;      // PD5才是MISO，要配置成上拉输入
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;   // 如果你的CS引脚由软件控制，记得初始化成通用推挽输出
    GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* CPOL = 1, CPHA = 1, MSB first */
unsigned int software_spi3_mode3(unsigned int write_data) {
    unsigned int read_data = 0;
    for (unsigned char i = 0; i < 16; i++) {
        SPI3_SCK_LOW;
        if (write_data & 0x8000)
            SPI3_MOSI_HIGH;
        else
            SPI3_MOSI_LOW;
        write_data <<= 1;
        delayus(1);     // min：25ns
        SPI3_SCK_HIGH;
        read_data <<= 1;
        if (SPI3_MISO)      // GPIO_ReadOutputDataBit(SPI3_MISO_GPIO_PORT, SPI3_MISO_PIN)
            read_data++;    //若从从机接收到高电平，数据自加一
        delayus(1);     // max：12.5ns
    }
    return read_data;
}
