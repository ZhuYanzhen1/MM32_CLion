/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

int main(void) {
    delay_config();
    led_config();
#ifdef IS_PROCESS_MCU
    delayms(2000);
    spi2_config();
//    xpt2046_gpio_config();
    gui_config();
//    xpt2046_calibrate();
    uart1_config();
    uart3_config();
    uart6_config();
    gps_config();
#endif  // IS_PROCESS_MCU
    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
    timer2_config();
    while (1) {
#ifdef IS_PROCESS_MCU
        gui_show_gnrmc_information();
#endif  // IS_PROCESS_MCU
        LED1_TOGGLE();
        delayms(100);
    }
}
