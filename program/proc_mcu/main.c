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
    iic1_software_config();
//    iic1_config();
//    at24c02_test_memory();
    xpt2046_gpio_config();
    spi1_config();
    spi2_config();
    spi3_software_init();
    gui_config();
    uart1_config();
    uart3_config();
    uart6_config();

    gps_config();
    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
    timer2_config();

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms
//        printf("printf test.\r\n");
//        fflush(stdout);

//        unsigned int X = adis_read_register(0x1200);
//        unsigned int Y = adis_read_register(0x1600);
//        unsigned int Z = adis_read_register(0x1A00);

//        adis_burst_read();
        iic_read_hmc5883l();

//        LED1_TOGGLE();
//        gui_clear_screan(C_BLACK);
//        LED1_TOGGLE();
//        delayms(500);
//        gui_clear_screan(C_RED);
//        LED1_TOGGLE();
//        delayms(500);
//        gui_clear_screan(C_YELLOW);
//        LED1_TOGGLE();
//        delayms(500);
//        gui_clear_screan(C_BLUE);
//        LED1_TOGGLE();
//        delayms(500);
//        gui_clear_screan(C_GREEN);
//        LED1_TOGGLE();
//        delayms(500);
    }
}
