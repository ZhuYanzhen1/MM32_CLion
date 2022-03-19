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
    iic1_config();
    spi2_config();
    spi3_config();
    gui_config();
    uart1_config();
    uart3_config();
    uart6_config();
    uart8_config();
    gps_config();
    xpt2046_gpio_config();
    if (at24c02_test_memory() == 0)
        LED1_ON();
    else
        LED1_OFF();
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
