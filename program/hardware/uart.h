/**************************************************************************/ /**
    \file     uart.h
    \brief    UART function Header File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#ifndef MM32F3277_HARDWARE_UART_H_
#define MM32F3277_HARDWARE_UART_H_

#include "hal_conf.h"

void uart1_config(unsigned int baudrate);
void uart1_sendbyte(unsigned char data);

void uart3_config(unsigned int baudrate);
void uart3_sendbyte(unsigned char data);

void uart6_config(unsigned int baudrate);
void uart6_sendbyte(unsigned char data);

void uart6_dma_init(unsigned int baud_rate);
void dma_receive_config(const unsigned char *data_address, unsigned short data_length);
void dma_nvic_config(unsigned char priority, unsigned char sub_priority);

#endif  // MM32F3277_HARDWARE_UART_H_
