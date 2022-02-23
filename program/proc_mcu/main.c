/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern hmc5883l magnetometer;

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

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms
//        adis_burst_read();
        iic_read_hmc5883l();
        printf("x:%d, y:%d, z:%d\r\n", magnetometer.x, magnetometer.y, magnetometer.z);
        delayms(50);
        gui_printf(5, 0, C_BLACK, C_WHITE, "mag_x: %d   ", magnetometer.x);
        gui_printf(5, 12, C_BLACK, C_WHITE, "mag_y: %d   ", magnetometer.y);
        gui_printf(5, 24, C_BLACK, C_WHITE, "mag_z: %d   ", magnetometer.z);
        gui_flush();            // 错开GUI的DMA刷新，但是UART6的DMA可能会受到这个的影响。
        delayms(50);
    }
}
