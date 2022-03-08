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

    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");

    timer2_config();

    while (calflg == 0) {
        iic_read_hmc5883l();
        temp[0] = magnetometer.x;
        temp[1] = magnetometer.y;
        temp[2] = magnetometer.z;
        calflg = CompassCal(temp);
        delayms(100);
    }

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms

        time[0] = global_time_stamp;

//        printf("%f  %f\r\n", neu.east_distance, distance_east);
//        printf("%f  %f\r\n", neu.east_v, v_east_final);
//        gui_printf(5, 12, C_BLACK, C_WHITE, "true_north:%.4f", true_north);
//        gui_flush();            // 错开GUI的DMA刷新，但是UART6的DMA可能会受到这个的影响。

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
