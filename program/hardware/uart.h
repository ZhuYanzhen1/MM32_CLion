/**************************************************************************//**
  \file     uart.h
  \brief    uart function Header File
  \author   Lao·Zhu
  \version  V1.0.1
  \date     14. November 2021
 ******************************************************************************/

#ifndef MM32F3277_HARDWARE_UART_H_
#define MM32F3277_HARDWARE_UART_H_

#include "hal_conf.h"

void uart1_config(unsigned int baudrate);
void uart1_sendbyte(unsigned char data);

#endif  // MM32F3277_HARDWARE_UART_H_
