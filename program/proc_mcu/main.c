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
    xpt2046_gpio_config();
    spi1_config();
    spi2_config();
//    spi3_config();
    software_spi3_init();
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

//        adis_read_prod_id();
        adis_burst_read();
        LED1_TOGGLE();
        delayms(500);
    }
}
