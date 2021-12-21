/**************************************************************************/ /**
    \file     main.h
    \brief    main function Header File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#ifndef MM32F3277_USER_MAIN_H_
#define MM32F3277_USER_MAIN_H_

///////////// System /////////////
#include "config.h"
#include "delay.h"
#include "hal_conf.h"
#include "malloc.h"
#include "mm32_device.h"
#include "printf.h"

///////////// Hardware /////////////
#include "led.h"
#include "key.h"
#include "spi.h"
#include "uart.h"

///////////// Math /////////////
#include "fft.h"
#include "qfplib.h"
#include "verification.h"

///////////// Device /////////////
#include "adis16470.h"

///////////// Debug /////////////
#include "debugger.h"
#include "decrypt.h"
#include "cm_backtrace.h"

#endif  // MM32F3277_USER_MAIN_H_
