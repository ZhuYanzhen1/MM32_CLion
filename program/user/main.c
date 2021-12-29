/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern unsigned char uart6_buffer[256];
static float theta = 0, sin_theta = 0;

int main(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    delay_config();
    led_config();
    key_config();
    spi2_config();
//    xpt2046_config();
    gui_config();
//    xpt2046_calibrate();
    uart1_config(115200);
    uart3_config(115200);
    uart6_config(9600);
    cm_backtrace_init("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(float_32bit, &theta, "theta");
    debugger_register_variable(float_32bit, &sin_theta, "sin_theta");
    while (1) {
        theta = theta + 0.1f;
        sin_theta = qfp_fsin(theta);
        debugger_scan_variable(global_time_stamp);
        for (int j = 0; j < 12; ++j)
            for (int i = 0; i < 21; ++i)
                gui_putchar(i * 6, j * 12, uart6_buffer[i + 21 * j], C_BLACK, C_WHITE);
        for (int i = 0; i < 4; ++i)
            gui_putchar(i * 6, 144, uart6_buffer[i + 252], C_BLACK, C_WHITE);
        LED1_TOGGLE();
        delayms(50);
    }
}
