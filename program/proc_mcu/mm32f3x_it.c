/**************************************************************************/ /**
    \file     mm32f3x_it.c
    \brief    MM32F3x interrupt handler function source file
    \author   Lao·Zhu & ZGL
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

#include "mm32f3x_it.h"
#include "main.h"

extern EventGroupHandle_t touch_event;

/* \brief Motor temperature */
unsigned char temperature = 0;

unsigned char battery_voltage = 0x00;
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

void TIM2_IRQHandler(void) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    debugger_scan_variable(global_time_stamp);
}

/*!
    \brief  Touch Screen Related
    \retval None
*/
void EXTI4_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line4)) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        EXTI_ClearFlag(EXTI_Line4);
        if (!GPIO_ReadInputDataBit(TOUCH_PEN_PORT, TOUCH_PEN_PIN) && touch_event != NULL)
            xEventGroupSetBitsFromISR(touch_event, 0x00000001,
                                      &xHigherPriorityTaskWoken);
    }
}

/*!
    \brief  Receive data sent from the PC side
    \retval None
*/
void UART1_IRQHandler(void) {
    if (UART_GetITStatus(UART1, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART1);
        mdtp_receive_handler(recvbyte);
        UART_ClearITPendingBit(UART1, UART_ISR_RX);
    }
}

/*!
    \brief  Receive the motor temperature sent by ctrl_mcu
    \retval None
*/
void UART4_IRQHandler(void) {
    if (UART_GetITStatus(UART4, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART4);
        UART_ClearITPendingBit(UART4, UART_ISR_RX);
        temperature = recvbyte;
    }
}

/*!
    \brief  Receive information from the power board
    \retval None
*/
void UART6_IRQHandler(void) {
    if (UART_GetITStatus(UART6, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART6);
        UART_ClearITPendingBit(UART6, UART_ISR_RX);
        battery_voltage = (recvbyte >> 3) + 140;
        battery_current = recvbyte & 0x07;
    }
}

/*!
    \brief  Receive information from GPS
    \retval None
*/
void DMA1_Channel3_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC3)) {
        /* Clear all interrupt flags */
        DMA_ClearITPendingBit(DMA1_IT_GL3);

        /* Double ping pong buffer */
        if (uart3_free_buffer_no == buffer_no_1) {
            uart3_dma_set_transmit_buffer(usart3_dma_buffer_2, 74);
            uart3_free_buffer_no = buffer_no_2;
            deal_dma_gnrmc(usart3_dma_buffer_1);
        } else {
            uart3_dma_set_transmit_buffer(usart3_dma_buffer_1, 74);
            uart3_free_buffer_no = buffer_no_1;
            deal_dma_gnrmc(usart3_dma_buffer_2);
        }
    }
}

/*!
    \brief
    \retval None
*/
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

/*!
    \brief  Receive messages sent by read_mcu
    \retval None
*/
void DMA1_Channel6_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC6)) {
        DMA_ClearITPendingBit(DMA1_IT_TC6);

        if (uart2_free_buffer_no == buffer_no_1) {
            uart2_dma_set_transmit_buffer((unsigned int *) packages_to_be_unpacked_2, uart2_dma_buffer_size);
            uart2_free_buffer_no = buffer_no_2;
            deal_dma_read_mcu((unsigned int *) packages_to_be_unpacked_1);
        } else {
            uart2_dma_set_transmit_buffer((unsigned int *) packages_to_be_unpacked_1, uart2_dma_buffer_size);
            uart2_free_buffer_no = buffer_no_1;
            deal_dma_read_mcu((unsigned int *) packages_to_be_unpacked_2);
        }
    }
}

/*!
    \brief  Refreshing the interface of the display
    \retval None
*/
void DMA2_Channel2_IRQHandler(void) {
    if (DMA_GetITStatus(DMA2_IT_TC2)) {
        DMA_ClearITPendingBit(DMA2_IT_TC2);
        lcd_set_address(0, 0, 127, 159);
        spi3_dma_set_transmit_buffer((unsigned int *) lcd_buffer, 128 * 160 * 2);
    }
}
