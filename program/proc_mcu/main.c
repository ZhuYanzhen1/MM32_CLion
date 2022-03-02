/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern hmc_correction magnetometer_correction;

int wz_gyro;
float true_north_final;
float true_north;
kalman_filter_float kalman_north;
kalman_filter_float kalman_distance_north;
kalman_filter_float kalman_distance_earth;

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

    kalman_config_ture_north(&kalman_north);
    kalman_config_distance(&kalman_distance_north);
    kalman_config_distance(&kalman_distance_earth);

    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
//    debugger_register_variable(dbg_float32, &true_north, "mag");
//    debugger_register_variable(dbg_float32, &true_north_final, "kalman");
    debugger_register_variable(dbg_int32, &gps_rmc.latitude, "latitude");
    debugger_register_variable(dbg_int32, &gps_rmc.longitude, "longitude");
//    debugger_register_variable(dbg_int32, &gps_rmc.speed_to_ground_section, "speed");
//    debugger_register_variable(dbg_int32, &gps_rmc.direction_of_ground_truth, "direction");
//    debugger_register_variable(dbg_int32, &gps_rmc.date, "date");
//    debugger_register_variable(dbg_int16, (void *) &imu.x_acll, "x");
//    debugger_register_variable(dbg_int16, (void *) &imu.y_acll, "y");
//    debugger_register_variable(dbg_int16, (void *) &imu.z_acll, "z");
//    debugger_register_variable(dbg_float32, &true_north, "north");
    timer2_config();

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms
/*
//        wz_gyro = adis_read_register(0x0E00);
//        iic_read_hmc5883l();
//        hmc5883l_correction();
//        true_north = qfp_fadd(qfp_fmul(qfp_fatan2(magnetometer_correction.y, magnetometer_correction.x),
//                                       qfp_fdiv(180, PI)), 180);
//        true_north_final = kalman_update_ture_north(&kalman_north, true_north, -(float) wz_gyro * 0.1f, 0.02f);
*/
        adis_burst_read();

        iic_read_hmc5883l();
        hmc5883l_correction();
        true_north = qfp_fadd(qfp_fmul(qfp_fatan2(magnetometer_correction.y, magnetometer_correction.x),
                                       qfp_fdiv(180, PI)), 180);
        true_north_final = kalman_update_ture_north(&kalman_north, true_north,
                                                    -(float) wz_gyro * 0.1f, 0.02f);

        coordinate_system_transformation_neu(0.02f);
        kalman_update_distance(&kalman_distance_north, neu.north_distance, neu.north_v,
                               neu.north_acceleration, 0.02f);
        kalman_update_distance(&kalman_distance_earth, neu.earth_distance, neu.earth_v,
                               neu.earth_acceleration, 0.02f);

//        printf("%f %f %f", magnetometer_correction.x, magnetometer_correction.y, magnetometer_correction.z);
//        gui_printf(5, 12, C_BLACK, C_WHITE, "true_north:%.4f", true_north);
//        gui_flush();            // 错开GUI的DMA刷新，但是UART6的DMA可能会受到这个的影响。
        delayms(200);
    }
}

// 任务1：将GPS经纬度信息的单位转化为十进制的° (✔，但未测试)
// 任务2：将GPS接收到的速度的单位从节变为°/s (✔)，并转换为东北天坐标系下的速度(✔)
// 任务3：将3轴的加速度转换为东北天坐标系下的加速度(✔)
// 任务4：卡尔曼融合 (✔)
// 4.1：卡尔曼初始化参数
// 4.2：经度和东向的加速度融合；纬度和北向的加速度融合
// 4.3：将纬度转换为以m为单位，与北向的速度去
