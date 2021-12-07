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

#endif  // MM32F3277_USER_CONFIG_H_
