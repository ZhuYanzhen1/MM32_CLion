/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern calpara_t params;

void hardware_init(void) {
    delay_config();
    led_config();
    iic1_config();
    spi2_config();
    spi3_config();
    uart1_config();
    uart3_config();
    uart6_config();
    uart8_config();
    xpt2046_gpio_config();
    cm_backtrace_config("mm32f3277", "1.3.3", "1.3.3");
}

int main(void) {
    gps_config();
    gui_config();
//    xpt2046_calibrate();
//    at24c02_saveparams();
    at24c02_readparams();
//    kalman_config();
//    calibration_acll();

    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
    timer2_config();

    while (1) {
        unsigned char x_pos, y_pos;
        printf("time:%d\r\n", global_time_stamp);
        fflush(stdout);
        LED1_TOGGLE();
        xpt2046_scan(&x_pos, &y_pos);
        gui_printf(10, 11 * 12, C_BLACK, C_WHITE, "XPos:%03d", x_pos);
        gui_printf(10, 12 * 12, C_BLACK, C_WHITE, "YPos:%03d", y_pos);
        gui_show_gnrmc_information();       // 46.8ms
        delayms(200);
    }
}

// 最开始的时候，读不到GPS信息，卡尔曼的参数就是0，那么下面就会乱迭代
