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

void TIM2_IRQHandler(void) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    debugger_scan_variable(global_time_stamp);
}

void UART1_IRQHandler(void) {
    if (UART_GetITStatus(UART1, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART1);
        mdtp_receive_handler(recvbyte);
        UART_ClearITPendingBit(UART1, UART_ISR_RX);
    }
}

#ifdef IS_PROCESS_MCU

void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0)) {
        EXTI_ClearFlag(EXTI_Line0);
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
buffer_no uart6_free_buffer_no = buffer_no_1;
unsigned int usart6_dma_buffer_1[74];
unsigned int usart6_dma_buffer_2[74];

void DMA1_Channel1_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC1)) {
        /* Clear all interrupt flags */
        DMA_ClearITPendingBit(DMA1_IT_GL1);

        /* Double ping pong buffer */
        if (uart6_free_buffer_no == buffer_no_1) {
            uart6_dma_receive_config(usart6_dma_buffer_2, 74);
            DMA_Cmd(DMA1_Channel1, ENABLE);
            uart6_free_buffer_no = buffer_no_2;
            deal_dma_gnrmc(usart6_dma_buffer_1);
        } else {
            uart6_dma_receive_config(usart6_dma_buffer_1, 74);
            DMA_Cmd(DMA1_Channel1, ENABLE);
            uart6_free_buffer_no = buffer_no_1;
            deal_dma_gnrmc(usart6_dma_buffer_2);
        }
    }
}

// 双乒乓缓冲：
// 先是选好两个buffer，
// 然后怎么拿给那边用呢
buffer_no uart1_free_buffer_no = buffer_no_1;
unsigned char dma1_ch4_flag = 0;
unsigned int usart1_dma_buffer_1[12];
unsigned int usart1_dma_buffer_2[12];
void DMA1_Channel4_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC4)) {
        DMA_ClearITPendingBit(DMA1_IT_TC4);
        dma1_ch4_flag = 0;
        /* Double ping pong buffer */
        if (uart1_free_buffer_no == buffer_no_1)
            uart1_free_buffer_no = buffer_no_2;
        else
            uart1_free_buffer_no = buffer_no_1;
    }
}

unsigned int *choose_uart1_buffer() {
    if (uart1_free_buffer_no == buffer_no_1)
        return usart1_dma_buffer_1;
    else if (uart1_free_buffer_no == buffer_no_2)
        return usart1_dma_buffer_2;
    else
        return 0;
}

#endif  // IS_PROCESS_MCU
