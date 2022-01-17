/**************************************************************************/ /**
    \file     mm32f3x_it.h
    \brief    MM32F3x interrupt handler function header file
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#ifndef MINIFOC_SYSTEM_GD32F1X0_IT_H
#define MINIFOC_SYSTEM_GD32F1X0_IT_H

void SysTick_Handler(void);

#ifdef IS_PROCESS_MCU
void EXTI0_IRQHandler(void);
void UART1_IRQHandler(void);
void UART3_IRQHandler(void);
void DMA1_Channel1_IRQHandler(void);
void deal_dma_gnrmc();
#endif

#endif  // MINIFOC_SYSTEM_GD32F1X0_IT_H
