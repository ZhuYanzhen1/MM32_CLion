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
kalman_filter_float kalman_north = {0};
kalman_filter_float kalman_distance_north = {0};
kalman_filter_float kalman_distance_earth = {0};
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

    float temp_angle_north = ture_north_begin();
    kalman_config_angle(&kalman_north, temp_angle_north);
    kalman_config_distance(&kalman_distance_north, 2599405);     //2599405    工一楼顶 //384400 西八楼顶
    kalman_config_distance(&kalman_distance_earth, 12608766);    //12608766            //1487900

    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
    debugger_register_variable(dbg_int32, (void *) (&gps_rmc.latitude), "latitude");
    debugger_register_variable(dbg_float32, (void *) (&distance_north), "n_distance");
    debugger_register_variable(dbg_float32, (void *) (&distance_east), "e_distance");
    debugger_register_variable(dbg_float32, &kalman_distance_north.vel, "kalman_v");
    debugger_register_variable(dbg_float32, (void *) &neu.north_v, "n_v");
    debugger_register_variable(dbg_float32, (void *) (&neu.north_acceleration), "neu_na");
    debugger_register_variable(dbg_float32, &true_north_final, "kalman");
    debugger_register_variable(dbg_float32, (void *) (&true_north), "angle_north");
//    debugger_register_variable(dbg_float32, (void *) (&neu.north_v), "v_n");



    timer2_config();

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms

        time[0] = global_time_stamp;

        iic_read_hmc5883l();
        hmc5883l_correction();
        //TODO burst_read 模式之后，就不能再单独读取寄存器了（两个不能共用）
        adis_burst_read();    // GPS融合的时候需要用到这条
        true_north = qfp_fadd(qfp_fmul(qfp_fatan2(magnetometer_correction.y, magnetometer_correction.x),
                                       qfp_fdiv(180, PI)), 180);
        true_north_final = kalman_update(&kalman_north, true_north,
                                         -(float) imu.z_gyro * 0.1f, 0.101f, 1);
//        roll = acrtan(z / y) + x_gyro
//        pitch = arctan(z / x) + y_gyro
//滤波
        // 最开始的时候，读不到GPS信息，卡尔曼的参数就是0，那么下面就会乱迭代
        while (gps_rmc.status != 'A');
        coordinate_system_transformation_neu(true_north_final, 0.101f);

        distance_north = kalman_update(&kalman_distance_north, neu.north_distance,
                                       neu.north_v, 0.101f, 0);
        distance_east = kalman_update(&kalman_distance_earth, neu.east_distance,
                                      neu.east_v, 0.101f, 0);

//        printf("%f  \r\n", true_north);
//        gui_printf(5, 12, C_BLACK, C_WHITE, "true_north:%.4f", true_north);
//        gui_flush();            // 错开GUI的DMA刷新，但是UART6的DMA可能会受到这个的影响。

//在delay这里卡住过2次
        delayms(100);

        time[1] = global_time_stamp;
        time[2] = time[1] - time[0]; // dt = ((float) time[2] * 0.001f)
    }
}

// 任务1：将GPS经纬度信息的单位转化为十进制的° (✔，但未测试)
// 任务2：将GPS接收到的速度的单位从节变为°/s (✔)，并转换为东北天坐标系下的速度(✔)
// 任务3：将3轴的加速度转换为东北天坐标系下的加速度(✔)
// 任务4：卡尔曼融合 (✔)
// 4.1：卡尔曼初始化参数
// 4.2：经度和东向的加速度融合；纬度和北向的加速度融合
// 4.3：将纬度转换为以m为单位，与北向的速度去
