/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern volatile unsigned char uart6_buffer[128];
extern volatile unsigned char uart6_rc_flag;
static float theta = 0, sin_theta = 0;

void gui_show_gnrmc_information(nmea_rmc *gps_rmc);

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
    uart6_config(9600);
    cm_backtrace_init("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(float_32bit, &theta, "theta");
    debugger_register_variable(float_32bit, &sin_theta, "sin_theta");
    UART6_CONFIG_GPS("$PCAS01,5*19\r\n")
    unsigned int apbclock = RCC_GetPCLK1Freq();
    UART6->BRR = (apbclock / 115200) / 16;
    UART6->FRA = (apbclock / 115200) % 16;
    delayms(100);
    UART6_CONFIG_GPS("$PCAS04,3*1A\r\n")
    delayms(100);
    UART6_CONFIG_GPS("$PCAS05,2*1A\r\n")
    delayms(100);
    UART6_CONFIG_GPS("$PCAS03,0,0,0,0,1,0,0,0,0,0,,,0,0*03\r\n")
    delayms(100);
    UART6_CONFIG_GPS("$PCAS02,100*1E\r\n")
    nmea_rmc gps_rmc = {0};
    while (1) {
//        theta = theta + 0.1f;
//        sin_theta = qfp_fsin(theta);
//        debugger_scan_variable(global_time_stamp);
        if (uart6_rc_flag == 1) {
            nmea_gnrmc_analysis(&gps_rmc, (char *) uart6_buffer);
            uart6_rc_flag = 0;
            gui_show_gnrmc_information(&gps_rmc);
            LED1_TOGGLE();
        }
    }
}

void gui_show_gnrmc_information(nmea_rmc *gps_rmc) {
    gui_printf(0, 0 * 12, C_BLACK, C_WHITE,
               "time:%d", gps_rmc->positioning_time.uct_time);
    gui_printf(0, 1 * 12, C_BLACK, C_WHITE,
               "status:%c", gps_rmc->status);
    gui_printf(0, 2 * 12, C_BLACK, C_WHITE,
               "latitude:%d", gps_rmc->latitude);
    gui_printf(0, 3 * 12, C_BLACK, C_WHITE,
               "latitude_direction:%c", gps_rmc->latitude_direction);
    gui_printf(0, 4 * 12, C_BLACK, C_WHITE,
               "longitude:%d", gps_rmc->longitude);
    gui_printf(0, 5 * 12, C_BLACK, C_WHITE,
               "longitude_direction:%c", gps_rmc->longitude_direction);
    gui_printf(0, 6 * 12, C_BLACK, C_WHITE,
               "status:%c", gps_rmc->status);
    gui_printf(0, 7 * 12, C_BLACK, C_WHITE,
               "speed:%d", gps_rmc->speed_to_ground_section);
    gui_printf(0, 8 * 12, C_BLACK, C_WHITE,
               "direction:%d", gps_rmc->direction_of_ground_truth);
    gui_printf(0, 9 * 12, C_BLACK, C_WHITE,
               "positioning_mode:%c", gps_rmc->mode);
}