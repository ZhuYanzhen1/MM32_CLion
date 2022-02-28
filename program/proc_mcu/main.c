/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern hmc_correction magnetometer_correction;

short wz_gyro;
float true_north_final;
float true_north;
kalman_filter_t kalman;

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
//    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
    debugger_register_variable(dbg_float32, &true_north, "mag");
    debugger_register_variable(dbg_float32, &true_north_final, "true_north_final");
    timer2_config();

    kalman_config(&kalman);

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms

        wz_gyro = adis_read_register(0x0E00);
        iic_read_hmc5883l();
        hmc5883l_correction();
        true_north = qfp_fadd(qfp_fmul(qfp_fatan2(magnetometer_correction.y, magnetometer_correction.x),
                                       qfp_fdiv(180, PI)), 180);
        true_north_final = kalman_update(&kalman, true_north, wz_gyro, 0.01f);

//        printf("%f %f %f", magnetometer_correction.x, magnetometer_correction.y, magnetometer_correction.z);
//        gui_printf(5, 12, C_BLACK, C_WHITE, "true_north:%.4f", true_north);
//        gui_flush();            // 错开GUI的DMA刷新，但是UART6的DMA可能会受到这个的影响。

        delayms(10);
    }
}
