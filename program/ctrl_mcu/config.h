/**************************************************************************/ /**
    \file     config.h
    \brief    file that stores user configuration macros.
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

#ifndef MM32F3277_USER_CONFIG_H_
#define MM32F3277_USER_CONFIG_H_

#include "version.h"

//////////////////////////////////////// System & Debugger Macro ////////////////////////////////////////
/*! \brief Not Using FreeRTOS */
#define USE_FREERTOS            0
/*! \brief Systick interrupt frequency set to 1kHz */
#define configTICK_RATE_HZ      1000
/*! \brief Debugger variable maximum size set to 12 */
#define VARIABLE_BUFFER_SIZE    12
/*! \brief Not use mdtp protocol to print buffer */
#define DEBUG_USE_PROTOCOL      0


//////////////////////////////////////// CMBackTrace Macro ////////////////////////////////////////
/*! \brief Enable bare metal(no OS) platform */
#define CMB_USING_BARE_METAL_PLATFORM
/*! \brief CPU platform type set to Cortex-M3 */
#define CMB_CPU_PLATFORM_TYPE          CMB_CPU_ARM_CORTEX_M3
/*! \brief Enable dump stack information */
#define CMB_USING_DUMP_STACK_INFO
/*! \brief Language of print information set as English */
#define CMB_PRINT_LANGUAGE             CMB_PRINT_LANUUAGE_ENGLISH


//////////////////////////////////////// Interrupt Priority ////////////////////////////////////////
/*! \brief Variable scanner timer interrupt priority set to 5 */
#define TIM2_PRIORITY           5
/*! \brief PC debugger receive interrupt priority set to 3 */
#define UART1_PRIORITY          3
/*! \brief Debugger send DMA interrupt priority set to 4 */
#define UART1_DMA_PRIORITY      4
/*! \brief ProcessMCU data receive DMA interrupt priority set to 4 */
#define UART6_DMA_PRIORITY      1
/*! \brief Motor Driver temperature receive interrupt priority set to 2 */
#define UART7_PRIORITY          2

//////////////////////////////////////// Peripherals Speed ////////////////////////////////////////
#define UART1_BAUDRATE          115200
#define UART6_BAUDRATE          115200
#define UART7_BAUDRATE          115200

#endif  // MM32F3277_USER_CONFIG_H_
