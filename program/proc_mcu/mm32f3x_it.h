/**************************************************************************/ /**
    \file     mm32f3x_it.h
    \brief    MM32F3x interrupt handler function header file
    \author   Lao·Zhu
    \version  V1.3.2
    \date     06. March 2022
 ******************************************************************************/

#ifndef MM32F3277_SYSTEM_MM32F3X_IT_H_
#define MM32F3277_SYSTEM_MM32F3X_IT_H_

void TIM2_IRQHandler(void);
void UART1_IRQHandler(void);
void UART6_IRQHandler(void);
void UART8_IRQHandler(void);
void EXTI4_IRQHandler(void);
void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA2_Channel2_IRQHandler(void);

#endif  // MM32F3277_SYSTEM_MM32F3X_IT_H_
