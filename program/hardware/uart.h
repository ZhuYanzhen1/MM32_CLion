/**************************************************************************/ /**
    \file     uart.h
    \brief    UART function Header File
    \author   Lao·Zhu
    \version  V1.3.2
    \date     06. March 2021
 ******************************************************************************/

#ifndef MM32F3277_HARDWARE_UART_H_
#define MM32F3277_HARDWARE_UART_H_

#define UART7_DMA_SEND_BUFFER       4

void uart1_config();

void uart3_config();
void uart3_sendbyte(unsigned char data);

void uart4_sendbyte(unsigned char data);
void uart4_config();

void uart6_config();
void uart6_sendbyte(unsigned char data);

void uart7_config();
void uart7_sendbyte(unsigned char data);

void uart8_config();
void uart8_sendbyte(unsigned char data);

#endif  // MM32F3277_HARDWARE_UART_H_
