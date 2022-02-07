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
    delayms(2000);
    iic1_config(400000);
    spi2_config();
    spi3_config();
    gui_config();
    uart1_config();
    uart3_config();
    uart6_config();
    gps_config();
    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
//    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
//    timer2_config();

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms
//        printf("printf test.\r\n");
//        fflush(stdout);
//        spi3_readwrite_byte(111);
        unsigned int a = 0;
        const unsigned char add = 0x72;
        adis16470_read_register(&add, &a, 1);
        LED1_TOGGLE();
        delayms(500);
    }
}
