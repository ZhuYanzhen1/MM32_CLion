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

static unsigned char uart6_byte_counter = 0;
volatile unsigned char uart6_buffer[128] = {0x00};
volatile unsigned char uart6_rc_flag = 0;
static unsigned char uart6_fsm_status = 0;

void UART6_IRQHandler(void) {
    if (UART_GetITStatus(UART6, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART6);
        switch (uart6_fsm_status) {
            case 0:
                if (recvbyte == '$') {
                    uart6_fsm_status = 1;
                    uart6_byte_counter = 1;
                    for (unsigned char counter = 0; counter < (unsigned char) sizeof(uart6_buffer); ++counter)
                        uart6_buffer[counter] = 0x00;
                    uart6_buffer[0] = '$';
                } else
                    uart6_fsm_status = 0;
                break;
            case 1:
                if (recvbyte == '\r') {
                    uart6_fsm_status = 2;
                    uart6_rc_flag = 1;
                } else {
                    uart6_buffer[uart6_byte_counter] = recvbyte;
                    uart6_byte_counter++;
                }
                break;
            default:
            case 2:uart6_fsm_status = 0;
                break;
        }
        UART_ClearITPendingBit(UART6, UART_ISR_RX);
    }
}

typedef enum { buffer_no_1 = 0, buffer_no_2 = 1 } buffer_no;
buffer_no free_buffer_no;
unsigned char usart6_dma_buffer_1[74];
unsigned char usart6_dma_buffer_2[74];
extern DMA_InitTypeDef DMA_InitStructure;
void DMA1_Channel1_IRQHandler(void) {
    if (DMA_GetITStatus(DMA1_IT_TC1)) //通道1传输完成中断TC 还有传输 过半中断HT 错误中断TE 全局中断GL
    {
//        unsigned int data_counter = DMA_GetCurrDataCounter(DMA1_Channel1);//获取剩余长度,一般都为0,调试用
        DMA_ClearITPendingBit(DMA1_IT_GL1);    //清除全部中断标志
        //转换可操作BUF
        if (free_buffer_no == buffer_no_1) {
            dma_receive_config(usart6_dma_buffer_1, 74);
            free_buffer_no = buffer_no_2;
        } else {
            dma_receive_config(usart6_dma_buffer_2, 74);
            DMA_Init(DMA1_Channel5, &DMA_InitStructure);
            free_buffer_no = buffer_no_1;
        }
//        buffer_ok = TRUE; //有准备好的数据了
    }
}

