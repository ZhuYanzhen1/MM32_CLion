/**************************************************************************/ /**
    \file     uart.h
    \brief    UART function Header File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#ifndef MM32F3277_HARDWARE_UART_H_
#define MM32F3277_HARDWARE_UART_H_

void uart1_config(unsigned int baudrate);
void uart1_sendbyte(unsigned char data);

void uart3_config(unsigned int baudrate);
void uart3_sendbyte(unsigned char data);

void uart6_config(unsigned int baud_rate);
void uart6_sendbyte(unsigned char data);

#endif  // MM32F3277_HARDWARE_UART_H_
