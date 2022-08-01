/**************************************************************************/ /**
    \file     mm32f3x_it.c
    \brief    MM32F3x interrupt handler function source file
    \author   Lao·Zhu & ZGL
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

#include "mm32f3x_it.h"
#include "main.h"

/* \brief The event group when the touch event occurs */
extern EventGroupHandle_t touch_event;

/* \brief Motor temperature form control MCU */
unsigned char temperature = 0;

/* \brief Battery voltage from power board */
unsigned char battery_voltage = 0x00;

/* \brief Battery output current from power board */
unsigned char battery_current = 0x00;

/*! \brief  Buffer enumeration value currently used by UART2 & UART3 */
typedef enum { buffer_no_1 = 1, buffer_no_2 = 2 } buffer_no;

/*! \brief  Buffer flag bit currently used by UART3 */
static buffer_no uart3_free_buffer_no = buffer_no_1;

/*! \brief  UART3 receive buffer 1 */
unsigned int usart3_dma_buffer_1[74];

/*! \brief  UART3 receive buffer 2 */
unsigned int usart3_dma_buffer_2[74];

/*! \brief  Buffer flag bit currently used by UART2 */
static buffer_no uart2_free_buffer_no = buffer_no_1;

/*! \brief  UART2 receive buffer 1 */
volatile unsigned int packages_to_be_unpacked_1[READ_MCU_AMOUNT] = {0};

/*! \brief  UART2 receive buffer 2 */
volatile unsigned int packages_to_be_unpacked_2[READ_MCU_AMOUNT] = {0};

/*! \brief Data buffer used by the printf() function */
extern unsigned int printf_mdtp_dma_buffer[16][12];

/*! \brief The number of buffers used by the printf() function counter */
extern unsigned char printf_dma_counter;

/*! \brief Flag bit for printf content being sent */
unsigned char printf_sending_flag = 0;

/*! \brief  Lcd buffer */
extern volatile unsigned char lcd_buffer[128 * 160 * 2];

/*!
    \brief Timer2 interrupt callback function, scan debug variable
*/
void TIM2_IRQHandler(void) {
    /* Clear TIM2 interrupt bit */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    debugger_scan_variable(global_time_stamp);
}

/*!
    \brief  Interrupt callback function when touch screen event is triggered
*/
void EXTI4_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line4)) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        /* Clear EXTI4 interrupt bit */
        EXTI_ClearFlag(EXTI_Line4);

        /* Determine whether to touch and send touch events through the operating system */
        if (!GPIO_ReadInputDataBit(TOUCH_PEN_PORT, TOUCH_PEN_PIN) && touch_event != NULL)
            xEventGroupSetBitsFromISR(touch_event, 0x00000001,
                                      &xHigherPriorityTaskWoken);
    }
}

/*!
    \brief  Receive data sent from the PC side
*/
void UART1_IRQHandler(void) {
    if (UART_GetITStatus(UART1, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART1);

        /* Clear UART1 interrupt bit */
        UART_ClearITPendingBit(UART1, UART_ISR_RX);

        /* Handles single byte UART data */
        mdtp_receive_handler(recvbyte);
    }
}

/*!
    \brief  Receive the motor temperature sent from ctrl_mcu
*/
void UART4_IRQHandler(void) {
    if (UART_GetITStatus(UART4, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART4);

        /* Clear UART4 interrupt bit */
        UART_ClearITPendingBit(UART4, UART_ISR_RX);

        /* Store the received temperature into variable */
        temperature = recvbyte;
    }
}

/*!
    \brief  Receive information from the power board
*/
void UART6_IRQHandler(void) {
    if (UART_GetITStatus(UART6, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART6);

        /* Clear UART6 interrupt bit */
        UART_ClearITPendingBit(UART6, UART_ISR_RX);

        /* Store the received battery info into variable */
        battery_voltage = (recvbyte >> 3) + 140;
        battery_current = recvbyte & 0x07;
    }
}

/*!
    \brief  Receive information from GPS
*/
void DMA1_Channel3_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC3)) {
        /* Clear DMA1 channel4 interrupt flags */
        DMA_ClearITPendingBit(DMA1_IT_GL3);

        /* Ping-pong buffer switching logic */
        if (uart3_free_buffer_no == buffer_no_1) {
            /* Switching DMA1 receive memory to buffer2 */
            uart3_dma_set_transmit_buffer(usart3_dma_buffer_2, 74);
            uart3_free_buffer_no = buffer_no_2;

            /* Process the data in buffer1 */
            deal_dma_gnrmc(usart3_dma_buffer_1);
        } else {
            /* Switching DMA1 receive memory to buffer1 */
            uart3_dma_set_transmit_buffer(usart3_dma_buffer_1, 74);
            uart3_free_buffer_no = buffer_no_1;

            /* Process the data in buffer2 */
            deal_dma_gnrmc(usart3_dma_buffer_2);
        }
    }
}

/*!
    \brief Interrupt functions related to printf
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

/*!
    \brief  Receive messages sent by read_mcu
*/
void DMA1_Channel6_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC6)) {
        /* Clear DMA1 channel6 interrupt flags */
        DMA_ClearITPendingBit(DMA1_IT_TC6);

        /* Ping-pong buffer switching logic */
        if (uart2_free_buffer_no == buffer_no_1) {
            /* Switching DMA1 receive memory to buffer2 */
            uart2_dma_set_transmit_buffer((unsigned int *) packages_to_be_unpacked_2,
                                          uart2_dma_buffer_size);
            uart2_free_buffer_no = buffer_no_2;

            /* Process the data in buffer1 */
            deal_dma_read_mcu((unsigned int *) packages_to_be_unpacked_1);
        } else {
            /* Switching DMA1 receive memory to buffer1 */
            uart2_dma_set_transmit_buffer((unsigned int *) packages_to_be_unpacked_1,
                                          uart2_dma_buffer_size);
            uart2_free_buffer_no = buffer_no_1;

            /* Process the data in buffer2 */
            deal_dma_read_mcu((unsigned int *) packages_to_be_unpacked_2);
        }
    }
}

/*!
    \brief  Refreshing LCD data using DMA
*/
void DMA2_Channel2_IRQHandler(void) {
    if (DMA_GetITStatus(DMA2_IT_TC2)) {
        /* Clear DMA2 channel2 interrupt flags */
        DMA_ClearITPendingBit(DMA2_IT_TC2);

        /* Set the display refresh area to the entire screen */
        lcd_set_address(0, 0, 127, 159);

        /* Configure DMA2 to start transferring LCD screen color data */
        spi3_dma_set_transmit_buffer((unsigned int *) lcd_buffer, 128 * 160 * 2);
    }
}
