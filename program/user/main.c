/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern nmea_rmc gps_rmc;
int main(void) {
    delay_config();
    led_config();
#ifdef IS_PROCESS_MCU
    delayms(2000);
    spi2_config();
//    xpt2046_config();
    gui_config();
//    xpt2046_calibrate();
    uart1_config(115200);
    uart3_config(115200);
    uart6_config(9600);
    gps_config();
#endif
    cm_backtrace_init("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
    timer2_config();
    while (1) {
#ifdef IS_PROCESS_MCU
        gui_show_gnrmc_information(&gps_rmc);
#endif
        LED1_TOGGLE();
        delayms(100);
    }
}
