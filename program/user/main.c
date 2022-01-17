/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern nmea_rmc gps_rmc;
static float theta = 0, sin_theta = 0;
int main(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    delay_config();
    led_config();
    key_config();
    delayms(2000);
    spi2_config();
//    xpt2046_config();
    gui_config();
//    xpt2046_calibrate();
    uart1_config(115200);
    uart3_config(115200);
//    uart6_config(9600);
    uart6_dma_init(9600);
    gps_config();

    cm_backtrace_init("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(float_32bit, &theta, "theta");
    debugger_register_variable(float_32bit, &sin_theta, "sin_theta");
    while (1) {
//        theta = theta + 0.1f;
//        sin_theta = qfp_fsin(theta);
//        debugger_scan_variable(global_time_stamp);
//        if (uart6_rc_flag == 1) {
//            nmea_gnrmc_analysis(&gps_rmc, (char *) uart6_buffer);
//            uart6_rc_flag = 0;
//            gui_show_gnrmc_information(&gps_rmc);
//            LED1_TOGGLE();
//        }
//        if (dma_receive_ok == first_ok) {
//            nmea_gnrmc_analysis(&gps_rmc, (char *) first_data);
        gui_show_gnrmc_information(&gps_rmc);
        LED1_TOGGLE();
        delayms(200);
//        } else if (dma_receive_ok == second_ok) {
//            nmea_gnrmc_analysis(&gps_rmc, (char *) second_data);
//            gui_show_gnrmc_information(&gps_rmc);
//            LED1_TOGGLE();
//        }
    }
}
