/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

#define SHOW_FIX
//#define SHOW_DEBUG

extern calpara_t params;
extern unsigned char packages_to_be_unpacked_fix[12];
extern unsigned char packages_to_be_unpacked_variable[DEBUG_BYTE];
extern unsigned char uart8_counter;
extern decode_fixed small_packets;
extern decode_debug debug_data;

void gui_show_fix() {
    gui_printf(0, 0 * 12, C_BLACK, C_WHITE,
               "ax:%d   ", small_packets.ax);
    gui_printf(0, 1 * 12, C_BLACK, C_WHITE,
               "ay:%d   ", small_packets.ay);
    gui_printf(0, 2 * 12, C_BLACK, C_WHITE,
               "az:%d   ", small_packets.az);
    gui_printf(0, 3 * 12, C_BLACK, C_WHITE,
               "north:%d   ", small_packets.pitch);
    gui_printf(0, 4 * 12, C_BLACK, C_WHITE,
               "yaw:%d   ", small_packets.yaw);
}

void gui_show_debug() {
    gui_printf(0, 0 * 12, C_BLACK, C_WHITE,
               "mag_x:%d   ", debug_data.mag_x);
    gui_printf(0, 1 * 12, C_BLACK, C_WHITE,
               "mag_y:%d   ", debug_data.mag_y);
    gui_printf(0, 2 * 12, C_BLACK, C_WHITE,
               "mag_z:%d   ", debug_data.mag_z);
//    gui_printf(0, 3 * 12, C_BLACK, C_WHITE,
//               "offset_x:%d", debug_data.offset_x);
//    gui_printf(0, 4 * 12, C_BLACK, C_WHITE,
//               "offset_y:%d", debug_data.offset_y);
//    gui_printf(0, 5 * 12, C_BLACK, C_WHITE,
//               "offset_z:%d", debug_data.offset_z);
//    gui_printf(0, 6 * 12, C_BLACK, C_WHITE,
//               "bias_x:%d", debug_data.bias_x);
//    gui_printf(0, 7 * 12, C_BLACK, C_WHITE,
//               "bias_y:%d", debug_data.bias_y);
//    gui_printf(0, 8 * 12, C_BLACK, C_WHITE,
//               "bias_z:%d", debug_data.bias_z);
//    gui_printf(0, 9 * 12, C_BLACK, C_WHITE,
//               "offbias_x:%d", debug_data.offbias_x);
//    gui_printf(0, 10 * 12, C_BLACK, C_WHITE,
//               "offbias_y:%d", debug_data.offbias_y);
//    gui_printf(0, 11 * 12, C_BLACK, C_WHITE,
//               "offbias_z:%d", debug_data.offbias_z);
//    gui_printf(0, 12 * 12, C_BLACK, C_WHITE,
//               "residual:%d", debug_data.residual);
//    gui_printf(0, 13 * 12, C_BLACK, C_WHITE,
//               "step_length:%d", debug_data.step_length);
//    gui_printf(0, 14 * 12, C_BLACK, C_WHITE,
//               "num:%d", debug_data.num);
}

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
//        printf("time:%d\r\n", global_time_stamp);
        fflush(stdout);
        LED1_TOGGLE();
        xpt2046_scan(&x_pos, &y_pos);
        gui_printf(10, 11 * 12, C_BLACK, C_WHITE, "XPos:%03d", x_pos);
        gui_printf(10, 12 * 12, C_BLACK, C_WHITE, "YPos:%03d", y_pos);
#ifdef SHOW_FIX
        if (packages_to_be_unpacked_fix[0] == 0xff && packages_to_be_unpacked_fix[11] == 0xff)
            unpacking_fixed_length_data(&packages_to_be_unpacked_fix[1]);
        else uart8_counter = 0;
        gui_show_fix();
#endif

#ifdef SHOW_DEBUG
        if (packages_to_be_unpacked_variable[0] == 0xa5 && packages_to_be_unpacked_variable[1] == 0x5a)
            unpacking_variable_length_data(&packages_to_be_unpacked_variable[3]);
        else
            uart8_counter = 0;
        gui_show_debug();
#endif
//        printf("%d %d %d\r\n", debug_data.mag_x, debug_data.mag_y, debug_data.mag_z);
        printf("%d\r\n", small_packets.pitch);
//        gui_show_gnrmc_information();       // 46.8ms
        delayms(200);
    }
}

// 最开始的时候，读不到GPS信息，卡尔曼的参数就是0，那么下面就会乱迭代
