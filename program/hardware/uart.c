/**************************************************************************/ /**
    \file     uart.c
    \brief    UART function Source File
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

#include "uart.h"
#include "hal_conf.h"
#ifdef IS_CONTROL_MCU
#include "../ctrl_mcu/config.h"
#else
#include "../proc_mcu/config.h"
#endif  // IS_CONTROL_MCU
#include "dma.h"

void uart1_config() {
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
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = UART1_PRIORITY;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    UART_StructInit(&UART_InitStruct);

    UART_InitStruct.BaudRate = UART1_BAUDRATE;
    UART_InitStruct.WordLength = UART_WordLength_8b;
    UART_InitStruct.StopBits = UART_StopBits_1;
    UART_InitStruct.Parity = UART_Parity_No;
    UART_InitStruct.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStruct.Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART1, &UART_InitStruct);
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
    UART_Cmd(UART1, ENABLE);

    uart1_dma_nvic_config();
}

#ifdef IS_PROCESS_MCU
void uart3_config() {
    UART_InitTypeDef UART_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART3, ENABLE);
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

    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate = UART3_BAUDRATE;
    UART_InitStruct.WordLength = UART_WordLength_8b;
    UART_InitStruct.StopBits = UART_StopBits_1;
    UART_InitStruct.Parity = UART_Parity_No;
    UART_InitStruct.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStruct.Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART3, &UART_InitStruct);
    UART_Cmd(UART3, ENABLE);
}

void uart4_config() {
    UART_InitTypeDef UART_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART4, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_8);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_8);

    NVIC_InitStruct.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = UART4_PRIORITY;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;    // 模式？
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate = UART4_BAUDRATE;
    UART_InitStruct.WordLength = UART_WordLength_8b;
    UART_InitStruct.StopBits = UART_StopBits_1;
    UART_InitStruct.Parity = UART_Parity_No;
    UART_InitStruct.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStruct.Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART4, &UART_InitStruct);
    UART_ITConfig(UART4, UART_IT_RXIEN, ENABLE);
    UART_Cmd(UART4, ENABLE);
}
#endif // IS_PROCESS_MCU

#ifdef IS_CONTROL_MCU
void uart6_config() {
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
    UART_InitStruct.BaudRate = UART6_BAUDRATE;
    UART_InitStruct.WordLength = UART_WordLength_8b;
    UART_InitStruct.StopBits = UART_StopBits_1;
    UART_InitStruct.Parity = UART_Parity_No;
    UART_InitStruct.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStruct.Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART6, &UART_InitStruct);
    UART_Cmd(UART6, ENABLE);
}

void uart7_config() {
    UART_InitTypeDef UART_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART7, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_8);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_8);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;  // Tx
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;  //  Rx
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = UART7_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = UART7_PRIORITY;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate = UART7_BAUDRATE;
    UART_InitStruct.WordLength = UART_WordLength_8b;
    UART_InitStruct.StopBits = UART_StopBits_1;
    UART_InitStruct.Parity = UART_Parity_No;
    UART_InitStruct.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStruct.Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART7, &UART_InitStruct);
    UART_ITConfig(UART7, UART_IT_RXIEN, ENABLE);
    UART_Cmd(UART7, ENABLE);
}
#else
void uart6_config() {
    UART_InitTypeDef UART_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2ENR_UART6, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_8);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_8);

    NVIC_InitStruct.NVIC_IRQChannel = UART6_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = UART6_PRIORITY;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate = UART6_BAUDRATE;
    UART_InitStruct.WordLength = UART_WordLength_8b;
    UART_InitStruct.StopBits = UART_StopBits_1;
    UART_InitStruct.Parity = UART_Parity_No;
    UART_InitStruct.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStruct.Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART6, &UART_InitStruct);
    UART_ITConfig(UART6, UART_IT_RXIEN, ENABLE);
    UART_Cmd(UART6, ENABLE);
}
#endif  // IS_CONTROL_MCU

#ifdef IS_PROCESS_MCU
void uart2_config() {
    UART_InitTypeDef UART_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART2, ENABLE); //
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;              // Tx  PA2
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;              // Rx  PA3
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate = UART2_BAUDRATE;
    UART_InitStruct.WordLength = UART_WordLength_8b;
    UART_InitStruct.StopBits = UART_StopBits_1;
    UART_InitStruct.Parity = UART_Parity_No;
    UART_InitStruct.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStruct.Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART2, &UART_InitStruct);
    UART_Cmd(UART2, ENABLE);
}

void uart2_sendbyte(unsigned char data) {
    UART_SendData(UART2, data);
    while (!UART_GetFlagStatus(UART2, UART_FLAG_TXEPT));
}
#endif // IS_PROCESS_MCU

void uart6_sendbyte(unsigned char data) {
    UART_SendData(UART6, data);
    while (!UART_GetFlagStatus(UART6, UART_FLAG_TXEPT));
}

#ifndef IS_PROCESS_MCU
void uart7_sendbyte(unsigned char data) {
    UART_SendData(UART7, data);
    while (!UART_GetFlagStatus(UART7, UART_FLAG_TXEPT));
}

#else
void uart4_sendbyte(unsigned char data) {
    UART_SendData(UART4, data);
    while (!UART_GetFlagStatus(UART4, UART_FLAG_TXEPT));
}

void uart3_sendbyte(unsigned char data) {
    UART_SendData(UART3, data);
    while (!UART_GetFlagStatus(UART3, UART_FLAG_TXEPT));
}
#endif // IS_PROCESS_MCU

#if DEBUG_USE_PROTOCOL == 0
void _putchar(char character) {
    UART_SendData(UART1, character);
    while (!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
}
#endif  // DEBUG_USE_PROTOCOL
