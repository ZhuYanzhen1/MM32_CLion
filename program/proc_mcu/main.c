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
unsigned char counter;
kalman_filter_float kalman_north;
kalman_filter_float kalman_distance_north;
kalman_filter_float kalman_distance_earth;
float distance_north;
float distance_east;
unsigned int time[6];

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

    kalman_config_angle(&kalman_north);
//    kalman_config_distance(&kalman_distance_north, 384400);
//    kalman_config_distance(&kalman_distance_earth, 1487900);

    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
//    debugger_register_variable(dbg_int32, (void *) (&gps_rmc.latitude), "latitude");
//    debugger_register_variable(dbg_float32, (void *) (&distance_north), "n_distance");
//    debugger_register_variable(dbg_float32, (void *) (&distance_east), "e_distance");
    debugger_register_variable(dbg_float32, &true_north_final, "kalman");
//    debugger_register_variable(dbg_float32, &kalman_distance_north.vel, "kalman_v");
//    debugger_register_variable(dbg_float32, (void *) (&neu.north_acceleration), "kalman_a");

    debugger_register_variable(dbg_float32, (void *) (&true_north), "angle_north");
//    debugger_register_variable(dbg_float32, (void *) (&neu.north_v), "v_n");

    timer2_config();

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms
/*
//        wz_gyro = adis_read_register(0x0E00);
//        iic_read_hmc5883l();
//        hmc5883l_correction();
//        true_north = qfp_fadd(qfp_fmul(qfp_fatan2(magnetometer_correction.y, magnetometer_correction.x),
//                                       qfp_fdiv(180, PI)), 180);
//        true_north_final = kalman_update(&kalman_north, true_north, -(float) wz_gyro * 0.1f, 0.02f);
*/
        time[0] = global_time_stamp;

        iic_read_hmc5883l();
        hmc5883l_correction();


        //TODO burst_read 模式之后，就不能再单独读取寄存器了（两个不能共用）
        adis_burst_read();    // GPS融合的时候需要用到这条

        true_north = qfp_fadd(qfp_fmul(qfp_fatan2(magnetometer_correction.y, magnetometer_correction.x),
                                       qfp_fdiv(180, PI)), 180);

        true_north_final = kalman_update(&kalman_north, true_north,
                                         -(float) imu.z_gyro * 0.1f, ((float) time[2] * 0.001f), 1);

//        coordinate_system_transformation_neu(true_north_final, ((float) time[2] * 0.001f));

//        distance_north = kalman_update(&kalman_distance_north, neu.north_distance,
//                                       neu.north_v, ((float) time[2] * 0.001f), 0);
//        distance_east = kalman_update(&kalman_distance_earth, neu.east_distance,
//                                      neu.east_v, ((float) time[2] * 0.001f), 0);

//        counter++;
//        if (counter % 3 == 0) {
//            printf("%f  %f \r\n", true_north, true_north_final);
//            counter = 0;
//        }
//        printf("%f  \r\n", true_north);
//        gui_printf(5, 12, C_BLACK, C_WHITE, "true_north:%.4f", true_north);
//        gui_flush();            // 错开GUI的DMA刷新，但是UART6的DMA可能会受到这个的影响。
        delayms(100);

        time[1] = global_time_stamp;
        time[2] = time[1] - time[0];
    }
}

// 任务1：将GPS经纬度信息的单位转化为十进制的° (✔，但未测试)
// 任务2：将GPS接收到的速度的单位从节变为°/s (✔)，并转换为东北天坐标系下的速度(✔)
// 任务3：将3轴的加速度转换为东北天坐标系下的加速度(✔)
// 任务4：卡尔曼融合 (✔)
// 4.1：卡尔曼初始化参数
// 4.2：经度和东向的加速度融合；纬度和北向的加速度融合
// 4.3：将纬度转换为以m为单位，与北向的速度去
