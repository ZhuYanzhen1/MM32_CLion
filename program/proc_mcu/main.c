/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

/* Geomagnetometer after correction Calculation of the true north angle */
float true_north;

/* Kalman fusion true north angle
 * (true north angle from geomagnetometer + z-axis angular velocity from gyroscope) */
float true_north_final;
kalman_filter_float kalman_angle_north = {0};

/* Kalman fusion to obtain northward and eastward velocities
 * (GPS velocity + imu acceleration) */
float v_north;
float v_east;
kalman_filter_float kalman_v_north = {0};
kalman_filter_float kalman_v_east = {0};
float v_north_final;
float v_east_final;

/* Kalman fusion to obtain northward displacement and eastward displacement */
float distance_north;
float distance_east;
kalman_filter_float kalman_distance_north = {0};
kalman_filter_float kalman_distance_earth = {0};

unsigned int time[3];

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

    float temp_angle_north = ture_north_begin();
    kalman_config_angle(&kalman_angle_north, temp_angle_north);
    kalman_config_v(&kalman_v_north);
    kalman_config_v(&kalman_v_east);
    kalman_config_distance(&kalman_distance_north, 384400);//得改     //337785    工一楼顶 //384400 西八楼顶
    kalman_config_distance(&kalman_distance_earth, 1487900);//得改    //377330            //1487900

    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");

    timer2_config();

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms

        time[0] = global_time_stamp;

        iic_read_hmc5883l();
        hmc5883l_correction();
        adis_burst_read();
        true_north = qfp_fadd(qfp_fmul(qfp_fatan2(magnetometer_correction.y, magnetometer_correction.x),
                                       qfp_fdiv(180, PI)), 180);
        true_north_final = kalman_update(&kalman_angle_north, true_north,
                                         -(float) imu.z_gyro * 0.1f, 0.101f, 1);

        while (gps_rmc.status != 'A');  //用到GPS的时候开这个，不用的时候不开
        coordinate_system_transformation_neu(true_north_final);

        v_north = kalman_update(&kalman_v_north, neu.north_v, neu.north_acceleration,
                                0.101f, 0);
        v_east = kalman_update(&kalman_v_east, neu.east_v, neu.east_acceleration,
                               0.101f, 0);
        if (v_north < 1 && v_north > -1) v_north_final = neu.north_v;
        else v_north_final = v_north;
        if (v_east < 1 && v_east > -1) v_east_final = neu.east_v;
        else v_east_final = v_east;
        distance_north = kalman_update(&kalman_distance_north, neu.north_distance,
                                       v_north_final, 0.101f, 0);
        distance_east = kalman_update(&kalman_distance_earth, neu.east_distance,
                                      v_east_final, 0.101f, 0);

//        printf("%f  %f\r\n", neu.east_distance, distance_east);
//        printf("%f  %f\r\n", neu.east_v, v_east_final);
//        gui_printf(5, 12, C_BLACK, C_WHITE, "true_north:%.4f", true_north);
//        gui_flush();            // 错开GUI的DMA刷新，但是UART6的DMA可能会受到这个的影响。

        delayms(100);

        time[1] = global_time_stamp;
        time[2] = time[1] - time[0]; // dt = ((float) time[2] * 0.001f)
    }
}

// 接下来的计划：卡尔曼融合GPS得到的速度以及imu的加速度

// 磁力计融合陀螺仪得到角度
// roll = acrtan(z / y) + x_gyro
// pitch = arctan(z / x) + y_gyro

//TODO burst_read 模式之后，就不能再单独读取寄存器了（两个不能共用）

// 最开始的时候，读不到GPS信息，卡尔曼的参数就是0，那么下面就会乱迭代