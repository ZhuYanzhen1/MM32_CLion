/**************************************************************************/ /**
    \file     config.h
    \brief    file that stores user configuration macros.
    \author   Lao·Zhu
    \version  V1.0.1
    \date     19. January 2022
 ******************************************************************************/

#ifndef MM32F3277_USER_CONFIG_H_
#define MM32F3277_USER_CONFIG_H_

//////////////////////////////////////// System & Debugger Macro ////////////////////////////////////////
/* not using FreeRTOS */
#define USE_FREERTOS            0
/* systick interrupt frequency set to 1000Hz */
#define configTICK_RATE_HZ      1000
/* debugger variable maximum size set to 12 */
#define VARIABLE_BUFFER_SIZE    12
#define DEBUG_USE_PROTOCOL      1

//////////////////////////////////////// CMBackTrace Macro ////////////////////////////////////////
/* enable bare metal(no OS) platform */
#define CMB_USING_BARE_METAL_PLATFORM
/* cpu platform type, must config by user */
#define CMB_CPU_PLATFORM_TYPE          CMB_CPU_ARM_CORTEX_M3
/* enable dump stack information */
#define CMB_USING_DUMP_STACK_INFO
/* language of print information */
#define CMB_PRINT_LANGUAGE             CMB_PRINT_LANUUAGE_ENGLISH


//////////////////////////////////////// Interrupt Priority ////////////////////////////////////////
#define TIM2_PRIORITY           10
#define UART1_PRIORITY          2
#define UART3_PRIORITY          3
#define UART3_DMA_PRIORITY      2
#define UART1_DMA_PRIORITY      3
#define EXTI1_PRIORITY          14
#define IIC1_DMA_PRIORITY       4
#define SPI2_DMA_PRIORITY       13

//////////////////////////////////////// Peripherals Speed ////////////////////////////////////////
#define UART1_BAUDRATE          115200
#define UART3_BAUDRATE          115200
#define UART6_BAUDRATE          115200

#define IIC1_SPEED              400000

#endif  // MM32F3277_USER_CONFIG_H_
