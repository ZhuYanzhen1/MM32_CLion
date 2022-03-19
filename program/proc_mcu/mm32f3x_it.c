/**************************************************************************/ /**
    \file     mm32f3x_it.c
    \brief    MM32F3x interrupt handler function source file
    \author   Lao·Zhu
    \version  V1.3.2
    \date     06. March 2022
 ******************************************************************************/

#include "mm32f3x_it.h"
#include "main.h"

#define SHOW_FIX
//#define SHOW_DEBUG

//TODO 每一次在中断内写函数时，留意一下是否会发生重入，然后到Trello里面评论记录

// 3us
void TIM2_IRQHandler(void) {
    rt_interrupt_enter();
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    debugger_scan_variable(global_time_stamp);
    rt_interrupt_leave();
}

void UART1_IRQHandler(void) {
    rt_interrupt_enter();
    if (UART_GetITStatus(UART1, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART1);
        mdtp_receive_handler(recvbyte);
        UART_ClearITPendingBit(UART1, UART_ISR_RX);
    }
    rt_interrupt_leave();
}

void UART6_IRQHandler(void) {
    rt_interrupt_enter();
    if (UART_GetITStatus(UART6, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART6);
        UART_ClearITPendingBit(UART6, UART_ISR_RX);
    }
    rt_interrupt_leave();
}

unsigned char uart8_counter;
unsigned char packages_to_be_unpacked_fix[12];
unsigned char packages_to_be_unpacked_variable[DEBUG_BYTE];
void UART8_IRQHandler(void) {
    rt_interrupt_enter();
    if (UART_GetITStatus(UART8, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART8);
#ifdef SHOW_FIX
        packages_to_be_unpacked_fix[uart8_counter++] = recvbyte;
        uart8_counter %= 12;
#endif

#ifdef SHOW_DEBUG
        packages_to_be_unpacked_variable[uart8_counter++] = recvbyte;
        uart8_counter %= DEBUG_BYTE;
#endif
        UART_ClearITPendingBit(UART8, UART_ISR_RX);
    }
    rt_interrupt_leave();
}

typedef enum { buffer_no_1 = 1, buffer_no_2 = 2 } buffer_no;
buffer_no uart6_free_buffer_no = buffer_no_1;
unsigned int usart3_dma_buffer_1[74];
unsigned int usart3_dma_buffer_2[74];

// 70.6us ~ 68.1us in V status
// 63.5us in A status
void DMA1_Channel3_IRQHandler(void) {
    rt_interrupt_enter();
    if (DMA_GetITStatus(DMA1_IT_TC3)) {
        /* Clear all interrupt flags */
        DMA_ClearITPendingBit(DMA1_IT_GL3);

        /* Double ping pong buffer */
        if (uart6_free_buffer_no == buffer_no_1) {
            uart3_dma_set_transmit_buffer(usart3_dma_buffer_2, 74);
            uart6_free_buffer_no = buffer_no_2;
            deal_dma_gnrmc(usart3_dma_buffer_1);
        } else {
            uart3_dma_set_transmit_buffer(usart3_dma_buffer_1, 74);
            uart6_free_buffer_no = buffer_no_1;
            deal_dma_gnrmc(usart3_dma_buffer_2);
        }
    }
    rt_interrupt_leave();
}

extern unsigned int printf_mdtp_dma_buffer[16][12];
extern unsigned char printf_dma_counter;
unsigned char printf_sending_flag = 0;
void DMA1_Channel4_IRQHandler(void) {
    rt_interrupt_enter();
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
    rt_interrupt_leave();
}

extern volatile unsigned char lcd_buffer[128 * 160 * 2];
void DMA2_Channel2_IRQHandler(void) {
    rt_interrupt_enter();
    if (DMA_GetITStatus(DMA2_IT_TC2)) {
        DMA_ClearITPendingBit(DMA2_IT_TC2);
        lcd_set_address(0, 0, 127, 159);
        spi3_dma_set_transmit_buffer((unsigned int *) lcd_buffer, 128 * 160 * 2);
    }
    rt_interrupt_leave();
}
