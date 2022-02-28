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
int true_north_final_int;
int true_north_final_int_temp;
float true_north_final;
float true_north;
kalman_filter_float kalman_float;
kalman_filter_int kalman_int;

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
    debugger_register_variable(dbg_float32, &true_north_final, "north_f");
    debugger_register_variable(dbg_int32, &true_north_final_int_temp, "north_i");
    timer2_config();

    kalman_config(&kalman_float);
    kalman_config_int(&kalman_int);

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms

        wz_gyro = adis_read_register(0x0E00);
        iic_read_hmc5883l();
        hmc5883l_correction();
        true_north = qfp_fadd(qfp_fmul(qfp_fatan2(magnetometer_correction.y, magnetometer_correction.x),
                                       qfp_fdiv(180, PI)), 180);
        true_north_final = kalman_update(&kalman_float, true_north, -(float) wz_gyro * 0.1f, 0.1f);
        true_north_final_int = kalman_update_int(&kalman_int, (int) (true_north * PRECISION_FLOAT_TO_INT),
                                                 -wz_gyro * PRECISION_FLOAT_TO_INT / 10, 10);

        true_north_final_int_temp = true_north_final_int * 100 / PRECISION_FLOAT_TO_INT;
        true_north_final_int_temp >= 0 ? (true_north_final_int_temp = true_north_final_int_temp % 36000) :
        (true_north_final_int_temp = 36000 + true_north_final_int_temp);

//        printf("%f %f %f", magnetometer_correction.x, magnetometer_correction.y, magnetometer_correction.z);
//        gui_printf(5, 12, C_BLACK, C_WHITE, "true_north:%.4f", true_north);
//        gui_flush();            // 错开GUI的DMA刷新，但是UART6的DMA可能会受到这个的影响。

        delayms(100);
    }
}
