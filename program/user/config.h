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
#define common_sendbyte uart1_sendbyte
#define configTOTAL_HEAP_SIZE ((unsigned int) (10 * 1024))

/* enable bare metal(no OS) platform */
/* #define CMB_USING_BARE_METAL_PLATFORM */
/* enable OS platform */
/* #define CMB_USING_OS_PLATFORM */
/* OS platform type, must config when CMB_USING_OS_PLATFORM is enable */
/* #define CMB_OS_PLATFORM_TYPE           CMB_OS_PLATFORM_RTT or CMB_OS_PLATFORM_UCOSII or CMB_OS_PLATFORM_UCOSIII or CMB_OS_PLATFORM_FREERTOS */
/* cpu platform type, must config by user */
#define CMB_CPU_PLATFORM_TYPE          /* CMB_CPU_ARM_CORTEX_M0 or CMB_CPU_ARM_CORTEX_M3 or CMB_CPU_ARM_CORTEX_M4 or CMB_CPU_ARM_CORTEX_M7 */
/* enable dump stack information */
/* #define CMB_USING_DUMP_STACK_INFO */
/* language of print information */
/* #define CMB_PRINT_LANGUAGE             CMB_PRINT_LANGUAGE_ENGLISH(default) or CMB_PRINT_LANGUAGE_CHINESE */

#endif  // MM32F3277_USER_CONFIG_H_
