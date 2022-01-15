/**************************************************************************/ /**
    \file     mm32f3x_it.c
    \brief    MM32F3x interrupt handler function source file
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "mm32f3x_it.h"
#include "main.h"

unsigned char uart6_buffer[256] = {' '};
static unsigned char uart6_counter = 0;
static unsigned char flag = 0;
/*!
    \brief  this function handles SysTick exception
    \retval none
*/
void SysTick_Handler(void) {
    /* update millisecond delay counter */
    delay_decrease();
}

void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0)) {
        EXTI_ClearFlag(EXTI_Line0);
    }
}

void EXTI15_10_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line14)) {
        LED2_TOGGLE();
        EXTI_ClearFlag(EXTI_Line14);
    }
}

void UART1_IRQHandler(void) {
    if (UART_GetITStatus(UART1, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART1);
        mdtp_receive_handler(recvbyte);
        UART_ClearITPendingBit(UART1, UART_ISR_RX);
    }
}

void UART3_IRQHandler(void) {
    if (UART_GetITStatus(UART3, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART3);
        sdtp_receive_handler(recvbyte);
        UART_ClearITPendingBit(UART3, UART_ISR_RX);
    }
}

void UART6_IRQHandler(void) {
    if (UART_GetITStatus(UART6, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART6);
        if ('$' == recvbyte) {
            flag = 1;
        } else if (10 == recvbyte)
            flag = 0;
        if (1 == flag) {
            uart6_buffer[uart6_counter++] = ((recvbyte == 13) ? ' ' : recvbyte);
//            uart6_counter = (uart6_counter + 1) % 0xff;
        } else
            uart6_counter = 0;
        UART_ClearITPendingBit(UART6, UART_ISR_RX);
    }
}
