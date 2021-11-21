/**************************************************************************//**
  \file     mm32f3x_it.h
  \brief    mm32f3x interrupt handler function header file
  \author   Lao·Zhu
  \version  V1.0.1
  \date     14. November 2021
 ******************************************************************************/

#ifndef MINIFOC_SYSTEM_GD32F1X0_IT_H
#define MINIFOC_SYSTEM_GD32F1X0_IT_H

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void UART1_IRQHandler(void);

#endif // MINIFOC_SYSTEM_GD32F1X0_IT_H