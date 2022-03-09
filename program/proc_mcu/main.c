/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

unsigned int time[3];
float temp[3];
extern calpara_t params;
char calflg = 0;

int main(void) {
    delay_config();
    led_config();
    delayms(2000);
    iic1_software_config();
    hmc5883l_config();
    spi1_config();
    spi2_config();
    spi3_software_init();
    gui_config();
    uart1_config();
    uart3_config();
    uart6_config();
    gps_config();
    xpt2046_gpio_config();
//    xpt2046_calibrate();
    kalman_config();

    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");

    timer2_config();

    iic_read_hmc5883l();
    delayms(1000);

    while (calflg == 0) {
        iic_read_hmc5883l();
        temp[0] = (float) magnetometer.x * FACTOR_MAGNETOMETER_MGS;
        temp[1] = (float) magnetometer.y * FACTOR_MAGNETOMETER_MGS;
        temp[2] = (float) magnetometer.z * FACTOR_MAGNETOMETER_MGS;
        calflg = CompassCal(temp);
        delayms(100);
    }

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms

        time[0] = global_time_stamp;
        LED1_TOGGLE();
//        printf("%f  %f\r\n", neu.east_distance, distance_east);
//        printf("%f  %f\r\n", neu.east_v, v_east_final);
//        gui_printf(5, 12, C_BLACK, C_WHITE, "true_north:%.4f", true_north);


        gui_printf(5, 12 * 0, C_BLACK, C_WHITE, "offset_x:%.4f", params.offset[0]);
        gui_printf(5, 12 * 1, C_BLACK, C_WHITE, "offset_y:%.4f", params.offset[1]);
        gui_printf(5, 12 * 2, C_BLACK, C_WHITE, "offset_z:%.4f", params.offset[2]);
        gui_printf(5, 12 * 3, C_BLACK, C_WHITE, "diag_x:%.4f", params.diag[0]);
        gui_printf(5, 12 * 4, C_BLACK, C_WHITE, "diag_y:%.4f", params.diag[1]);
        gui_printf(5, 12 * 5, C_BLACK, C_WHITE, "diag_z:%.4f", params.diag[2]);
        gui_printf(5, 12 * 6, C_BLACK, C_WHITE, "offdiag_x:%.4f", params.offdiag[0]);
        gui_printf(5, 12 * 7, C_BLACK, C_WHITE, "offdiag_y:%.4f", params.offdiag[1]);
        gui_printf(5, 12 * 8, C_BLACK, C_WHITE, "offdiag_z:%.4f", params.offdiag[2]);
        gui_flush();            // 错开GUI的DMA刷新，但是UART6的DMA可能会受到这个的影响。
        delayms(100);

        time[1] = global_time_stamp;
        time[2] = time[1] - time[0];
    }
}

// 接下来的计划：卡尔曼融合GPS得到的速度以及imu的加速度

// 磁力计融合陀螺仪得到角度
// roll = acrtan(z / y) + x_gyro
// pitch = arctan(z / x) + y_gyro

//TODO burst_read 模式之后，就不能再单独读取寄存器了（两个不能共用）

// 最开始的时候，读不到GPS信息，卡尔曼的参数就是0，那么下面就会乱迭代
