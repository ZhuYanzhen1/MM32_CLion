/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern calpara_t params;

int main(void) {
    delay_config();
    led_config();
    delayms(2000);
//    iic1_software_config();
//    hmc5883l_config();
    spi2_config();
    spi3_config();
//    spi3_software_init();
    gui_config();
    uart1_config();
    uart3_config();
    uart6_config();
    gps_config();
    xpt2046_gpio_config();
//    xpt2046_calibrate();
//    kalman_config();
//    calibration_acll();
    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
//    debugger_register_variable(dbg_float32, &north, "compass");

    timer2_config();

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms
        LED1_TOGGLE();
//        printf("%f  %f\r\n", neu.east_distance, distance_east);
//        printf("%f  %f\r\n", neu.east_v, v_east_final);
        gui_printf(5, 12, C_BLACK, C_WHITE, "time: %d", global_time_stamp);

//        iic_read_hmc5883l();
//        hmc5883l_correction();
//        north = qfp_fadd(qfp_fmul(qfp_fatan2(magnetometer_correction.y,
//                                             magnetometer_correction.x),
//                                  qfp_fdiv(180, PI)), 180);

        gui_flush();            // 错开GUI的DMA刷新，但是UART6的DMA可能会受到这个的影响。
        delayms(100);
    }
}

// 接下来的计划：卡尔曼融合GPS得到的速度以及imu的加速度

// 磁力计融合陀螺仪得到角度
// yaw = acrtan(y / x) & z_gyro
// roll = acrtan(z / y) & x_gyro
// pitch = arctan(z / x) & y_gyro

//TODO burst_read 模式之后，就不能再单独读取寄存器了（两个不能共用）

// 最开始的时候，读不到GPS信息，卡尔曼的参数就是0，那么下面就会乱迭代
