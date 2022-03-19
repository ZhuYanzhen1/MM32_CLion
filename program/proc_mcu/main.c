/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern calpara_t params;
extern unsigned char uart8_counter;
extern unsigned char packages_to_be_unpacked_fix[12];
extern unsigned char packages_to_be_unpacked_variable[DEBUG_BYTE];
extern decode_fixed small_packets;
extern decode_debug debug_data;

int main(void) {
    delay_config();
    led_config();
    delayms(2000);
//    iic1_software_config();
//    hmc5883l_config();
    spi2_config();
    spi3_config();
    gui_config();
    uart1_config();
    uart3_config();
    uart6_config();
    uart8_config();
    gps_config();
    xpt2046_gpio_config();
//    xpt2046_calibrate();
//    kalman_config();
//    calibration_acll();
    cm_backtrace_config("mm32f3277", "1.3.3", "1.3.3");
    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");

    timer2_config();

    while (1) {
//        unsigned char x_pos, y_pos;
//        gui_show_gnrmc_information();       // 46.8ms
        printf("time:%d\r\n", global_time_stamp);
        fflush(stdout);
//        xpt2046_scan(&x_pos, &y_pos);
//        gui_printf(10, 10, C_BLACK, C_WHITE, "XPos:%03d", x_pos);
//        gui_printf(10, 20, C_BLACK, C_WHITE, "YPos:%03d", y_pos);
//        iic_read_hmc5883l();
//        hmc5883l_correction();
//        north = qfp_fadd(qfp_fmul(qfp_fatan2(magnetometer_correction.y,
//                                             magnetometer_correction.x),
//                                  qfp_fdiv(180, PI)), 180);

//        if (packages_to_be_unpacked_fix[0] == 0xff && packages_to_be_unpacked_fix[11] == 0xff)
//            unpacking_fixed_length_data(&packages_to_be_unpacked_fix[1]);
//        else uart8_counter = 0;
        if (packages_to_be_unpacked_variable[0] == 0xa5 && packages_to_be_unpacked_variable[1] == 0x5a)
            unpacking_variable_length_data(&packages_to_be_unpacked_variable[3]);
        else
            uart8_counter = 0;
        delayms(100);
    }
}

// 磁力计融合陀螺仪得到角度
// yaw = acrtan(y / x) & z_gyro
// roll = acrtan(z / y) & x_gyro
// pitch = arctan(z / x) & y_gyro
