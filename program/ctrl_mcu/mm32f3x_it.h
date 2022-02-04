/**************************************************************************/ /**
    \file     mm32f3x_it.h
    \brief    MM32F3x interrupt handler function header file
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#ifndef MM32F3277_SYSTEM_MM32F3X_IT_H_
#define MM32F3277_SYSTEM_MM32F3X_IT_H_

void SysTick_Handler(void);
void TIM2_IRQHandler(void);
void UART1_IRQHandler(void);

#endif  // MM32F3277_SYSTEM_MM32F3X_IT_H_
