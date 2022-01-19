/**************************************************************************/ /**
    \file     config.h
    \brief    file that stores user configuration macros.
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#ifndef MM32F3277_USER_CONFIG_H_
#define MM32F3277_USER_CONFIG_H_

#define USE_FREERTOS 0
#define configTICK_RATE_HZ 1000
#define VARIABLE_BUFFER_SIZE 32
#define configTOTAL_HEAP_SIZE ((unsigned int) (10 * 1024))

/* enable bare metal(no OS) platform */
#define CMB_USING_BARE_METAL_PLATFORM
/* cpu platform type, must config by user */
#define CMB_CPU_PLATFORM_TYPE          CMB_CPU_ARM_CORTEX_M3
/* enable dump stack information */
#define CMB_USING_DUMP_STACK_INFO
/* language of print information */
#define CMB_PRINT_LANGUAGE             CMB_PRINT_LANUUAGE_ENGLISH

//
//TODO 每一次写优先级的时候，记得用宏封装起来放到这里
//

#define TIM2_PRIORITY 10
#define UART1_PRIORITY 2
#define UART3_PRIORITY 3
#define UART6_DMA_PRIORITY 2
#define UART1_DMA_PRIORITY 3
#define EXTI0_PRIORITY 14

#define UART1_BAUDRATE 115200
#define UART2_BAUDRATE 115200
#define UART3_BAUDRATE 9600

#endif  // MM32F3277_USER_CONFIG_H_
