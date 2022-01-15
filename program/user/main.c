/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern unsigned char uart6_buffer[256];
static float theta = 0, sin_theta = 0;
void gui_show_gnrmc_information(nmea_rmc gps_rmc);
int main(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    delay_config();
    led_config();
    key_config();
    spi2_config();
//    xpt2046_config();
    gui_config();
//    xpt2046_calibrate();
    uart1_config(115200);
    uart3_config(115200);
    uart6_config(9600);
    cm_backtrace_init("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(float_32bit, &theta, "theta");
    debugger_register_variable(float_32bit, &sin_theta, "sin_theta");
    UART6_CONFIG_GPS("$PCAS01,1*1D\r\n")
    UART6_CONFIG_GPS("$PCAS04,3*1A\r\n")
    UART6_CONFIG_GPS("$PCAS05,2*1A\r\n")
    UART6_CONFIG_GPS("$PCAS03,0,0,0,0,1,0,0,0,0,0,,,0,0*03\r\n")
    UART6_CONFIG_GPS("$PCAS02,100*1E\r\n")
    nmea_rmc gps_rmc = {0};
    while (1) {
        theta = theta + 0.1f;
        sin_theta = qfp_fsin(theta);
        debugger_scan_variable(global_time_stamp);
        nmea_gnrmc_analysis(&gps_rmc, uart6_buffer);
        for (int j = 0; j < 12; ++j)
            for (int i = 0; i < 21; ++i)
                gui_putchar(i * 6, j * 12, uart6_buffer[i + 21 * j], C_BLACK, C_WHITE);
        for (int i = 0; i < 4; ++i)
            gui_putchar(i * 6, 144, uart6_buffer[i + 252], C_BLACK, C_WHITE);
//        gui_show_gnrmc_information(gps_rmc);
        LED1_TOGGLE();
        delayms(50);
    }
}

void gui_show_gnrmc_information(nmea_rmc gps_rmc) {
    gui_printf(0, 0 * 12, C_BLACK, C_WHITE,
               "time:%d", gps_rmc.positioning_time);
    gui_printf(0, 1 * 12, C_BLACK, C_WHITE,
               "status:%c", gps_rmc.status);
    gui_printf(0, 2 * 12, C_BLACK, C_WHITE,
               "latitude:%d", gps_rmc.latitude);
    gui_printf(0, 3 * 12, C_BLACK, C_WHITE,
               "latitude_direction:%c", gps_rmc.latitude_direction);
    gui_printf(0, 4 * 12, C_BLACK, C_WHITE,
               "longitude:%d", gps_rmc.longitude);
    gui_printf(0, 5 * 12, C_BLACK, C_WHITE,
               "longitude_direction:%c", gps_rmc.longitude_direction);
    gui_printf(0, 6 * 12, C_BLACK, C_WHITE,
               "status:%c", gps_rmc.status);
    gui_printf(0, 7 * 12, C_BLACK, C_WHITE,
               "speed:%c", gps_rmc.speed_to_ground_section);
    gui_printf(0, 8 * 12, C_BLACK, C_WHITE,
               "direction:%c", gps_rmc.direction_of_ground_truth);
    gui_printf(0, 9 * 12, C_BLACK, C_WHITE,
               "positioning_mode:%c", gps_rmc.mode);
}