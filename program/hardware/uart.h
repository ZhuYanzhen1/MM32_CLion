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

#define UART6_CONFIG_GPS(cmdbuf)    for (unsigned char i = 0; i < (unsigned char) sizeof(cmdbuf); i++)\
                                        uart6_sendbyte(cmdbuf[i]);

void uart1_config(unsigned int baudrate);
void uart1_sendbyte(unsigned char data);

void uart3_config(unsigned int baudrate);
void uart3_sendbyte(unsigned char data);

void uart6_config(unsigned int baudrate);
void uart6_sendbyte(unsigned char data);
void uart6_config_gps(char *cmdbuf);

#endif  // MM32F3277_HARDWARE_UART_H_
