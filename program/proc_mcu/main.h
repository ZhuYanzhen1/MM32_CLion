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
#include "./pin.h"
#include "delay.h"
#include "hal_conf.h"
#include "mm32_device.h"
#include "printf.h"

///////////// Hardware /////////////
#include "led.h"
#include "dma.h"
#include "spi.h"
#include "uart.h"
#include "timer.h"
#include "gpio.h"
#include "iic.h"

///////////// Math /////////////
#include "qfplib.h"
#include "verification.h"
#include "kalman.h"
#include "data_conversion.h"
#include "mag_calibrate.h"

///////////// Device /////////////
#include "xpt2046.h"
#include "gps.h"
#include "at24c02.h"
#include "sensor_decode.h"

///////////// Debug /////////////
#include "debugger.h"
#include "cm_backtrace.h"

///////////// Protocol /////////////
#include "mdtp_unpack.h"
#include "mdtp_pack.h"
#include "sdtp_unpack.h"
#include "sdtp_pack.h"
#include "gps_parser.h"

//////////// GUI ////////////
#include "tft_lcd.h"
#include "gui_base.h"

#endif  // MM32F3277_USER_MAIN_H_
