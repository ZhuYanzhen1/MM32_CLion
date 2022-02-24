/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern hmc5883l magnetometer;
#define MAG_COEFFICIENT     0.00073f

int main(void) {
    delay_config();
    led_config();
    delayms(2000);
    iic1_software_config();
    hmc5883l_config();
//    iic1_config();
//    at24c02_test_memory();
    xpt2046_gpio_config();
    spi1_config();
    spi2_config();
    spi3_software_init();
    gui_config();
    uart1_config();
    uart3_config();
    uart6_config();

    gps_config();
    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
//    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
    debugger_register_variable(dbg_int16, (void *) &magnetometer.x, "mag_x");
    debugger_register_variable(dbg_int16, (void *) &magnetometer.y, "mag_y");
    debugger_register_variable(dbg_int16, (void *) &magnetometer.z, "mag_z");
    timer2_config();

    while (1) {
//        gui_show_gnrmc_information();       // 46.8ms
//        printf("printf test.\r\n");
//        fflush(stdout);

//        unsigned int X = adis_read_register(0x1200);
//        unsigned int Y = adis_read_register(0x1600);
//        unsigned int Z = adis_read_register(0x1A00);

//        adis_burst_read();
        iic_read_hmc5883l();
//        iic_read_hmc5883l_verification();

        gui_printf(0, 0, C_BLACK, C_WHITE, "%d   ", magnetometer.x);
        gui_printf(0, 12, C_BLACK, C_WHITE, "%d   ", magnetometer.y);
        gui_printf(0, 24, C_BLACK, C_WHITE, "%d   ", magnetometer.z);

//        printf("%f %f %f\r\n", (float) magnetometer.x * MAG_COEFFICIENT,
//               (float) magnetometer.y * MAG_COEFFICIENT, (float) magnetometer.z * MAG_COEFFICIENT);
//        __set_BASEPRI(4 << 4);
//        printf("%d %d %d\r\n", magnetometer.x, magnetometer.y, magnetometer.z);
//        fflush(stdout);
//        __set_BASEPRI(0);

        delayms(200);

//        LED1_TOGGLE();
//        gui_clear_screan(C_BLACK);
//        LED1_TOGGLE();
//        delayms(500);
//        gui_clear_screan(C_RED);
//        LED1_TOGGLE();
//        delayms(500);
//        gui_clear_screan(C_YELLOW);
//        LED1_TOGGLE();
//        delayms(500);
//        gui_clear_screan(C_BLUE);
//        LED1_TOGGLE();
//        delayms(500);
//        gui_clear_screan(C_GREEN);
//        LED1_TOGGLE();
//        delayms(500);
    }
}
