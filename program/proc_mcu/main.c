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
extern adis_point imu_point;
extern int text_wz;
extern float roll, pitch, yaw;

unsigned int count = 0;

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
    debugger_register_variable(dbg_int16, (void *) &imu_point.delta_angle_z, "angle_z");
    timer2_config();

    imu_dr_gpio_config();

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms

        if (count % 10 == 0)
            iic_read_hmc5883l();

        adis_burst_read();
        MadgwickAHRSupdateIMU();
        count++;

//        if (!DR_HIGH) {
//            adis_read_v_and_angle();
//            count++;
//            while (DR_HIGH);
//        }
//        if (count == 0) global_time_stamp = 0;
//        if (count >= 2000) {
//            printf("%d \r\n", imu_point.delta_angle_z);
//            count = 0;
//            text_wz /= 2000;
//            accumulator += text_wz;
//            text_wz = 0;
//        }
        if (count >= 60) {
            gui_printf(5, 0, C_BLACK, C_WHITE, "roll: %f  ", roll);
            gui_printf(5, 12, C_BLACK, C_WHITE, "yaw: %f  ", yaw);
            gui_printf(5, 24, C_BLACK, C_WHITE, "pitch:%f", pitch);
            gui_flush();
            count = 0;
        }
        delayms(2);
//            printf("%d %d %d\r\n", imu.x_acll, imu.y_acll, imu.z_acll);
//        delayms(50);
//        adis_burst_read();
//        gui_printf(5, 0, C_BLACK, C_WHITE, "roll: %f  ", roll);
//        gui_printf(5, 12, C_BLACK, C_WHITE, "yaw: %f  ", yaw);
//        gui_printf(5, 24, C_BLACK, C_WHITE, "pitch:%f", pitch);
//        gui_printf(5, 24 + 12 * 1, C_BLACK, C_WHITE, "angle_x:%d", imu_point.delta_angle_x);
//        gui_printf(5, 24 + 12 * 2, C_BLACK, C_WHITE, "angle_y:%d", imu_point.delta_angle_y);
//        gui_printf(5, 24 + 12 * 3, C_BLACK, C_WHITE, "angle_z:%d", imu_point.delta_angle_z);
//        gui_printf(5, 24 + 12 * 4, C_BLACK, C_WHITE, "v_x:%d", imu_point.delta_v_x);
//        gui_printf(5, 24 + 12 * 5, C_BLACK, C_WHITE, "v_y:%d", imu_point.delta_v_y);
//        gui_printf(5, 24 + 12 * 6, C_BLACK, C_WHITE, "v_z:%d", imu_point.delta_v_z);
//        gui_flush();            // 错开GUI的DMA刷新，但是UART6的DMA可能会受到这个的影响。
//        delayms(50);

    }
}
