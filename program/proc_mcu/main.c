/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern hmc5883l magnetometer;
extern adis16470_t imu;

fix16_t roll, pitch, yaw;
qf16 orientation;
fix16_t last_fusion_time;
fix16_t current_time;

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
//    debugger_register_variable(dbg_int16, &imu.x_acll, "imu.x_acll");
//    debugger_register_variable(dbg_int16, &imu.y_acll, "imu.y_acll");
//    debugger_register_variable(dbg_int16, &imu.z_acll, "imu.z_acll");
    timer2_config();

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms

        iic_read_hmc5883l();
        adis_burst_read();
//        MadgwickAHRSupdate();

//        current_time = systemTime(); //获得实时时间
//        const fix16_t deltaT_ms = fix16_from_int(current_time - last_fusion_time);
        const fix16_t deltaT_ms = 100;
        const fix16_t deltaT = fix16_mul(deltaT_ms, F16(0.001));

        last_fusion_time = current_time;

        get_sensors();//是否会起作用
        fusion_predict(deltaT);
        fusion_update(deltaT);

        fusion_fetch_angles(&roll, &pitch, &yaw);
//        fusion_fetch_quaternion(&orientation);

        printf("%d %d %d\r\n", imu.x_acll, imu.y_acll, imu.z_acll);
        delayms(50);
        adis_burst_read();
        gui_printf(5, 0, C_BLACK, C_WHITE, "roll: %f   ", roll);
        gui_printf(5, 12, C_BLACK, C_WHITE, "yaw: %f   ", yaw);
        gui_printf(5, 24, C_BLACK, C_WHITE, "pitch: %f   ", pitch);
        gui_flush();            // 错开GUI的DMA刷新，但是UART6的DMA可能会受到这个的影响。
        delayms(50);

    }
}
