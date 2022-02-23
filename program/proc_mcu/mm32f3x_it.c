/**************************************************************************/ /**
    \file     mm32f3x_it.c
    \brief    MM32F3x interrupt handler function source file
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "mm32f3x_it.h"
#include "main.h"

//
//TODO 每一次在中断内写函数时，留意一下是否会发生重入，然后到Trello里面评论记录
//

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
    LED1_ON();
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    debugger_scan_variable(global_time_stamp);
    LED1_OFF();
}

void UART1_IRQHandler(void) {
    if (UART_GetITStatus(UART1, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART1);
//        uart3_sendbyte(recvbyte);
        mdtp_receive_handler(recvbyte);
        UART_ClearITPendingBit(UART1, UART_ISR_RX);
    }
}

void EXTI1_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line1)) {
        EXTI_ClearFlag(EXTI_Line1);
    }
}

void UART3_IRQHandler(void) {
    if (UART_GetITStatus(UART3, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART3);
//        uart1_sendbyte(recvbyte);
        sdtp_receive_handler(recvbyte);
        UART_ClearITPendingBit(UART3, UART_ISR_RX);
    }
}

typedef enum { buffer_ok = 0, buffer_no_1 = 1, buffer_no_2 = 2 } buffer_no;
buffer_no uart6_free_buffer_no = buffer_no_1;
unsigned int usart6_dma_buffer_1[74];
unsigned int usart6_dma_buffer_2[74];

// 70.6us ~ 68.1us in V status
// 63.5us in A status
void DMA1_Channel1_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC1)) {
        /* Clear all interrupt flags */
        DMA_ClearITPendingBit(DMA1_IT_GL1);

        /* Double ping pong buffer */
        if (uart6_free_buffer_no == buffer_no_1) {
            uart6_dma_set_transmit_buffer(usart6_dma_buffer_2, 74);
            uart6_free_buffer_no = buffer_no_2;
            deal_dma_gnrmc(usart6_dma_buffer_1);
        } else {
            uart6_dma_set_transmit_buffer(usart6_dma_buffer_1, 74);
            uart6_free_buffer_no = buffer_no_1;
            deal_dma_gnrmc(usart6_dma_buffer_2);
        }
    }
}

extern unsigned int printf_mdtp_dma_buffer[16][12];
extern unsigned char printf_dma_counter;
unsigned char printf_sending_flag = 0;
void DMA1_Channel4_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC4)) {
        DMA_ClearITPendingBit(DMA1_IT_TC4);
        if (printf_sending_flag == 1) {
            printf_sending_flag = 0;
            printf_dma_counter = 0;
        }
        if (printf_dma_counter != 0) {
            uart1_dma_set_transmit_buffer(printf_mdtp_dma_buffer[0], printf_dma_counter * 12);
            printf_sending_flag = 1;
        }
    }
}

extern volatile unsigned char lcd_buffer[128 * 160 * 2];
void DMA1_Channel5_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC5)) {
        DMA_ClearITPendingBit(DMA1_IT_TC5);
        lcd_set_address(0, 0, 127, 159);
        spi2_dma_set_transmit_buffer((unsigned int *) lcd_buffer, 128 * 160 * 2);
    }
}
