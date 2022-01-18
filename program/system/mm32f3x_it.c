/**************************************************************************/ /**
    \file     mm32f3x_it.c
    \brief    MM32F3x interrupt handler function source file
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "mm32f3x_it.h"
#include "main.h"

/*!
    \brief  this function handles SysTick exception
    \retval none
*/
void SysTick_Handler(void) {
    /* update millisecond delay counter */
    delay_decrease();
}

#ifdef IS_PROCESS_MCU

void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0)) {
        EXTI_ClearFlag(EXTI_Line0);
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

typedef enum { buffer_ok = 0, buffer_no_1 = 1, buffer_no_2 = 2 } buffer_no;
buffer_no free_buffer_no = buffer_no_1;
unsigned int usart6_dma_buffer_1[74];
unsigned int usart6_dma_buffer_2[74];

void DMA1_Channel1_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC1)) {
        /* Clear all interrupt flags */
        DMA_ClearITPendingBit(DMA1_IT_GL1);

        /* Double ping pong buffer */
        if (free_buffer_no == buffer_no_1) {
            dma_receive_config(usart6_dma_buffer_2, 74);
            DMA_Cmd(DMA1_Channel1, ENABLE);
            free_buffer_no = buffer_no_2;
            deal_dma_gnrmc(usart6_dma_buffer_1);
        } else {
            dma_receive_config(usart6_dma_buffer_1, 74);
            DMA_Cmd(DMA1_Channel1, ENABLE);
            free_buffer_no = buffer_no_1;
            deal_dma_gnrmc(usart6_dma_buffer_2);
        }
    }
}

#endif
