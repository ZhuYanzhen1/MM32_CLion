/**************************************************************************/ /**
    \file     mm32f3x_it.h
    \brief    MM32F3x interrupt handler function header file
    \author   Lao·Zhu & ZGL
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

#ifndef MM32F3277_SYSTEM_MM32F3X_IT_H_
#define MM32F3277_SYSTEM_MM32F3X_IT_H_

void TIM2_IRQHandler(void);
void EXTI4_IRQHandler(void);
void UART1_IRQHandler(void);
void UART4_IRQHandler(void);
void UART6_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA2_Channel2_IRQHandler(void);

#endif  // MM32F3277_SYSTEM_MM32F3X_IT_H_
