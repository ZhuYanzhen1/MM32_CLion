/**************************************************************************//**
  \file     mm32f3x_it.c
  \brief    mm32f3x interrupt handler function source file
  \author   Lao·Zhu
  \version  V1.0.1
  \date     14. November 2021
 ******************************************************************************/

#include "mm32f3x_it.h"
#include "main.h"

/*!
    \brief  systick interrupt count variable is used to
            realize the encoder update function.
*/
static unsigned char systick_counter = 0;

/*!
    \brief  this function handles NMI exception
    \retval none
*/
void NMI_Handler(void) {
}

/*!
    \brief  this function handles HardFault exception
    \retval none
*/
void HardFault_Handler(void) {
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief  this function handles MemManage exception
    \retval none
*/
void MemManage_Handler(void) {
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief  this function handles BusFault exception
    \retval none
*/
void BusFault_Handler(void) {
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief  this function handles UsageFault exception
    \retval none
*/
void UsageFault_Handler(void) {
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief  this function handles SVC exception
    \retval none
*/
void SVC_Handler(void) {
}

/*!
    \brief  this function handles DebugMon exception
    \retval none
*/
void DebugMon_Handler(void) {
}

/*!
    \brief  this function handles PendSV exception
    \retval none
*/
void PendSV_Handler(void) {
}

/*!
    \brief  this function handles SysTick exception
    \retval none
*/
void SysTick_Handler(void) {
    /* update millisecond delay counter */
    delay_decrease();
}

void UART1_IRQHandler(void) {
    if (UART_GetITStatus(UART1, UART_ISR_RX) != RESET) {
        unsigned char recvbyte = UART_ReceiveData(UART1);
        uart1_sendbyte(recvbyte);
        UART_ClearITPendingBit(UART1, UART_ISR_RX);
    }
}
