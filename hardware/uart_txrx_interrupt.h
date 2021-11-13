////////////////////////////////////////////////////////////////////////////////
/// @file    uart_txrx_interrupt.h
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FIRMWARE FUNCTIONS.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////


// Define to prevent recursive inclusion
#ifndef __UART_TXRX_INTERRUPT_H
#define __UART_TXRX_INTERRUPT_H
// Files includes
#include "hal_conf.h"
#include  "stdio.h"
#include "string.h"


#define UART_REC_LEN            200
#define EN_UART1_RX             1
extern u8 gUartRxBuf[UART_REC_LEN];
extern u16 gUartRxSta;
void UART1_GPIO_Init(void);
void UART1_NVIC_Init(u32 baudrate);
void UART1_Send_Byte(u8 dat);
void UART1_Send_Group(u8* buf, u16 len);
void UART1_RxTx_Transceiving(void);
/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
