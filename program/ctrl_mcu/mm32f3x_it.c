/**************************************************************************/ /**
    \file     mm32f3x_it.c
    \brief    MM32F3x interrupt handler function source file
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

#include "mm32f3x_it.h"
#include "main.h"

/*! \brief Motor temperature information from the driver */
unsigned char temperature = 0;

/*! \brief Buffer enumeration value currently used by UART6 */
typedef enum { buffer_no_1 = 1, buffer_no_2 = 2 } buffer_no;

/*! \brief Buffer flag bit currently used by UART6 */
static buffer_no uart6_free_buffer_no = buffer_no_1;

/*! \brief UART6 receive buffer 1 */
unsigned int uart6_dma_buffer_1[CTRL_MCU_RECEIVE_AMOUNT];

/*! \brief UART6 receive buffer 2 */
unsigned int uart6_dma_buffer_2[CTRL_MCU_RECEIVE_AMOUNT];

/*! \brief Data buffer used by the printf() function */
extern unsigned int printf_mdtp_dma_buffer[16][12];

/*! \brief The number of buffers used by the printf() function counter */
extern unsigned char printf_dma_counter;

/*! \brief Flag bit for printf content being sent */
unsigned char printf_sending_flag = 0;

/*!
    \brief This function handles SysTick exception
*/
void SysTick_Handler(void) {
    /* update millisecond delay counter */
    delay_decrease();
}

/*!
    \brief Timer2 interrupt callback function, scan debug variable
*/
void TIM2_IRQHandler(void) {
    /* Clear TIM2 interrupt bit */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    debugger_scan_variable(global_time_stamp);
}

/*!
    \brief Receive data sent from the PC side
*/
void UART1_IRQHandler(void) {
    if (UART_GetITStatus(UART1, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART1);

        /* Handles single byte UART data */
        mdtp_receive_handler(recvbyte);

        /* Clear UART1 interrupt bit */
        UART_ClearITPendingBit(UART1, UART_ISR_RX);
    }
}

/*!
    \brief Receive data from the brushless driver board
*/
void UART7_IRQHandler(void) {
    if (UART_GetITStatus(UART7, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART7);

        /* Clear UART7 interrupt bit */
        UART_ClearITPendingBit(UART7, UART_ISR_RX);

        /* Store the received temperature into variable */
        temperature = recvbyte;
    }
}

/*!
    \brief Receive data sent by proc_mcu using a double ping-pong buffer
*/
void DMA1_Channel1_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC1)) {
        /* Clear DMA1 channel1 interrupt flags */
        DMA_ClearITPendingBit(DMA1_IT_TC1);

        /* Ping-pong buffer switching logic */
        if (uart6_free_buffer_no == buffer_no_1) {
            /* Switching DMA1 receive memory to buffer2 */
            uart6_dma_set_transmit_buffer(uart6_dma_buffer_2, CTRL_MCU_RECEIVE_AMOUNT);
            uart6_free_buffer_no = buffer_no_2;

            /* Process the data in buffer1 */
            deal_uart6_dma_proc(uart6_dma_buffer_1);
        } else {
            /* Switching DMA1 receive memory to buffer1 */
            uart6_dma_set_transmit_buffer(uart6_dma_buffer_1, CTRL_MCU_RECEIVE_AMOUNT);
            uart6_free_buffer_no = buffer_no_1;

            /* Process the data in buffer2 */
            deal_uart6_dma_proc(uart6_dma_buffer_2);
        }
    }
}

/*!
    \brief Transmit printf() information to PC using DMA1
*/
void DMA1_Channel4_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC4)) {
        /* Clear DMA1 channel4 interrupt flags */
        DMA_ClearITPendingBit(DMA1_IT_TC4);

        /* Determine if printing of all printf() function data is complete */
        if (printf_sending_flag == 1 && printf_dma_counter == 0) {
            printf_sending_flag = 0;
            printf_dma_counter = 0;
        }

        /* Determine if the printf() function is used to print data */
        if (printf_dma_counter != 0) {
            uart1_dma_set_transmit_buffer(printf_mdtp_dma_buffer[0], printf_dma_counter * 12);
            printf_dma_counter = 0;
            printf_sending_flag = 1;
        }
    }
}
