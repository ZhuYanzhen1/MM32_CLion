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

// 3us
void TIM2_IRQHandler(void) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    debugger_scan_variable(global_time_stamp);
}

/* LQR控制 */

void TIM4_IRQHandler(void) {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

}

void UART1_IRQHandler(void) {
    if (UART_GetITStatus(UART1, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART1);
        mdtp_receive_handler(recvbyte);
        UART_ClearITPendingBit(UART1, UART_ISR_RX);
    }
}

extern float actual_speed;
void UART7_IRQHandler(void) {
    if (UART_GetITStatus(UART7, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART7);
        UART_ClearITPendingBit(UART7, UART_ISR_RX);
        sdtp_receive_handler(recvbyte);
    }
}

typedef enum { buffer_no_1 = 1, buffer_no_2 = 2 } buffer_no;
static buffer_no uart6_free_buffer_no = buffer_no_1;
unsigned int uart6_dma_buffer_1[CTRL_MCU_RECEIVE_AMOUNT];
unsigned int uart6_dma_buffer_2[CTRL_MCU_RECEIVE_AMOUNT];
void DMA1_Channel1_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC1)) {
        /* Clear all interrupt flags */
        DMA_ClearITPendingBit(DMA1_IT_TC1);
        /* Double ping pong buffer */
        if (uart6_free_buffer_no == buffer_no_1) {
            uart6_dma_set_transmit_buffer(uart6_dma_buffer_2, CTRL_MCU_RECEIVE_AMOUNT);
            uart6_free_buffer_no = buffer_no_2;
            deal_uart6_dma_proc(uart6_dma_buffer_1);
        } else {
            uart6_dma_set_transmit_buffer(uart6_dma_buffer_1, CTRL_MCU_RECEIVE_AMOUNT);
            uart6_free_buffer_no = buffer_no_1;
            deal_uart6_dma_proc(uart6_dma_buffer_2);  // 解包，从proc传过来的，双乒乓缓冲区有时会导致乱
        }
    }
}

extern unsigned int printf_mdtp_dma_buffer[16][12];
extern unsigned char printf_dma_counter;
unsigned char printf_sending_flag = 0;
void DMA1_Channel4_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC4)) {
        DMA_ClearITPendingBit(DMA1_IT_TC4);
        if (printf_sending_flag == 1 && printf_dma_counter == 0) {
            printf_sending_flag = 0;
            printf_dma_counter = 0;
        }
        if (printf_dma_counter != 0) {
            uart1_dma_set_transmit_buffer(printf_mdtp_dma_buffer[0], printf_dma_counter * 12);
            printf_dma_counter = 0;
            printf_sending_flag = 1;
        }
    }
}

void EXTI9_5_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line6)) {
        EXTI_ClearFlag(EXTI_Line6);
//        ch372_interrupt_callback();
    }
}
