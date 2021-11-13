#include "uart.h"

typedef struct UART_SendItStruct {
    u32 UART_SendBufferAddress;
    u32 UART_SendLen;
    u32 UART_SendRealCnt;
    bool UART_SendComplete;
} UART_SendIt_Typedef;
typedef struct UART_RecvItStruct {
    u32 UART_RecvBufferAddress;
    u32 UART_RecvLen;
    u32 UART_RecvRealCnt;
    bool UART_RecvComplete;
} UART_RecvIt_Typedef;
UART_SendIt_Typedef send_struct;
UART_RecvIt_Typedef recv_struct;

#define RECVBUFLENGTH 100
__IO u8 sSendBuf[RECVBUFLENGTH] = "0123456789ABCDEFGHIJKLMN";
__IO u8 sRecvBuf[RECVBUFLENGTH];

__IO u16 RecvLen;
__IO u16 SendLen;
////////////////////////////////////////////////////////////////////////////////
/// @brief  TIM3 Pin Config
/// @param  None
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);

    //UART1_TX   GPIOA.9
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //UART1_RX    GPIOA.10
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Serial port initialization configuration
/// @note    It must be careful of the Chip Version.
/// @param  baudrate: Baud rate
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_NVIC_Init(u32 baudrate) {
    UART_InitTypeDef UART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2ENR_UART1, ENABLE);

    //UART1 NVIC
    NVIC_InitStruct.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    //Baud rate
    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate = baudrate;
    //The word length is in 8-bit data format.
    UART_InitStruct.WordLength = UART_WordLength_8b;
    UART_InitStruct.StopBits = UART_StopBits_1;
    //No even check bit.
    UART_InitStruct.Parity = UART_Parity_No;
    //No hardware data flow control.
    UART_InitStruct.HWFlowControl = UART_HWFlowControl_None;
    UART_InitStruct.Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART1, &UART_InitStruct);
//    UART_ITConfig(UART1, UART_IT_TXIEN|UART_IT_RXIEN, ENABLE);
    UART_Cmd(UART1, ENABLE);

    UART1_GPIO_Init();
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Serial port Interrupt Handler
/// @note
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_IRQHandler(void) {
    u8 recvbyte;
    UART_TypeDef *UARTx;
    UART_SendIt_Typedef *pUARTxSendStruct;
    UART_RecvIt_Typedef *pUARTxRecvStruct;

    UARTx = UART1;
    pUARTxSendStruct = &send_struct;
    pUARTxRecvStruct = &recv_struct;
    // Send packet
    if (UART_GetITStatus(UARTx, UART_IT_TXIEN) != RESET) {
        UART_ClearITPendingBit(UARTx, UART_IT_TXIEN);
        if (pUARTxSendStruct->UART_SendComplete == false) {
            if (pUARTxSendStruct->UART_SendLen <= pUARTxSendStruct->UART_SendRealCnt) {
                pUARTxSendStruct->UART_SendLen = 0;
                UART_ITConfig(UARTx, UART_IT_TXIEN, DISABLE);
                while (!UART_GetFlagStatus(UARTx, UART_CSR_TXC));
                pUARTxSendStruct->UART_SendComplete = true;
            } else {
                if (pUARTxSendStruct->UART_SendRealCnt < RECVBUFLENGTH) {
                    UART_SendData(UARTx,
                                  *(vu8 *) (pUARTxSendStruct->UART_SendBufferAddress
                                      + pUARTxSendStruct->UART_SendRealCnt));
                    pUARTxSendStruct->UART_SendRealCnt++;
                }
            }
        }
    }
    // Recv packet
    if (UART_GetITStatus(UARTx, UART_ISR_RX) != RESET) {
        recvbyte = UART_ReceiveData(UARTx);
        UART_ClearITPendingBit(UARTx, UART_ISR_RX);
        if (pUARTxRecvStruct->UART_RecvComplete == false) {
            if (pUARTxRecvStruct->UART_RecvRealCnt < RECVBUFLENGTH) {
                *(vu8 *) (pUARTxRecvStruct->UART_RecvBufferAddress + pUARTxRecvStruct->UART_RecvRealCnt) = recvbyte;
                pUARTxRecvStruct->UART_RecvRealCnt++;
                if (pUARTxRecvStruct->UART_RecvLen == pUARTxRecvStruct->UART_RecvRealCnt) {
                    pUARTxRecvStruct->UART_RecvLen = 0;
                    pUARTxRecvStruct->UART_RecvComplete = true;
                }
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  UART send byte.
/// @note   None.
/// @param  buf:buffer address.
/// @param  len:data length.
/// @retval s32  -1 = Failure, 0 successful.
////////////////////////////////////////////////////////////////////////////////
s32 UART1_Send_Packet_Interrupt(u8 *buf, u16 len) {
    UART_TypeDef *UARTx;
    UART_SendIt_Typedef *pUARTxSendStruct;
    UARTx = UART1;
    pUARTxSendStruct = &send_struct;
    UART_ITConfig(UARTx, UART_IT_TXIEN, DISABLE);
    if (len == 0) {
        return -1;
    } else if (len == 1) {
        UART_SendData(UARTx, *((u8 *) (buf)));
    } else {
        pUARTxSendStruct->UART_SendComplete = false;
        pUARTxSendStruct->UART_SendBufferAddress = (u32) (buf);
        pUARTxSendStruct->UART_SendLen = (u32) (len);
        pUARTxSendStruct->UART_SendRealCnt = 0;
        UART_ITConfig(UARTx, UART_IT_TXIEN, ENABLE);
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Serial port Interrupt checking Sending Finish
/// @note
/// @param  None
/// @retval s32  -1 = Failure, 0 successful..
////////////////////////////////////////////////////////////////////////////////
s32 UART1_Check_Send_Finish(void) {
    s32 temp;
    if (send_struct.UART_SendComplete == true) {
        send_struct.UART_SendComplete = false;
        temp = send_struct.UART_SendRealCnt;
        send_struct.UART_SendRealCnt = 0;
        return temp;
    } else {
        return -1;
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  UART Recv byte.
/// @note   None.
/// @param  buf:buffer address.
/// @param  len:data length.
/// @retval s32  -1 = Failure, 0 successful.
////////////////////////////////////////////////////////////////////////////////
s32 UART1_Recv_Packet_Interrupt(u8 *buf, u16 len) {
    UART_RecvIt_Typedef *pUARTxRecvStruct;

    pUARTxRecvStruct = &recv_struct;

    if (len == 0) {
        return -1;
    } else {
        pUARTxRecvStruct->UART_RecvComplete = false;
        pUARTxRecvStruct->UART_RecvBufferAddress = (u32) (buf);
        pUARTxRecvStruct->UART_RecvLen = (u32) (len);
        pUARTxRecvStruct->UART_RecvRealCnt = 0;
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Serial port Interrupt checking Recving Finish
/// @note
/// @param  None
/// @retval s32  -1 = Failure, 0 successful..
////////////////////////////////////////////////////////////////////////////////
s32 UART1_Check_Receive_Finish(void) {
    s32 temp;
    if (recv_struct.UART_RecvComplete == 1) {
        recv_struct.UART_RecvComplete = 0;
        temp = recv_struct.UART_RecvRealCnt;
        recv_struct.UART_RecvRealCnt = 0;
        return temp;
    } else {
        return -1;
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Run Tx Rx interrupt test
/// @note
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_RxTx_Transceiving(void) {
    s32 result;
    u16 realrecvLen = 0;
    RecvLen = 10;
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
    while (1) {
        UART1_Recv_Packet_Interrupt((u8 *) &sRecvBuf[0], RecvLen);
        while (1) {
            result = UART1_Check_Receive_Finish();
            if (result > 0) {
                realrecvLen = (u16) (result & 0xFFFF);
                break;
            }
        }
        SendLen = realrecvLen;
        memcpy((u8 *) &sSendBuf[0], (u8 *) &sRecvBuf[0], SendLen);
        UART1_Send_Packet_Interrupt((u8 *) &sSendBuf[0], SendLen);
        while (1) {
            if ((SendLen) == UART1_Check_Send_Finish()) {
                break;
            }
        }

    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  UART send byte.
/// @note   None.
/// @param  dat(A byte data).
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_Send_Byte(u8 dat) {
    UART_SendData(UART1, dat);
    while (!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  UART send byte.
/// @note   None.
/// @param  buf:buffer address.
/// @param  len:data length.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void UART1_Send_Group(u8 *buf, u16 len) {
    while (len--)
        UART1_Send_Byte(*buf++);
}

/// @}


/// @}

/// @}

