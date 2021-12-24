/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

static float theta = 0, sin_theta = 0;

int main(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    delay_config();
    led_config();
    key_config();
    spi2_config();
    xpt2046_config();
    gui_config();
    xpt2046_calibrate();
    uart1_config(115200);
    uart3_config(115200);
    cm_backtrace_init("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(float_32bit, &theta, "theta");
    debugger_register_variable(float_32bit, &sin_theta, "sin_theta");
    while (1) {
        theta = theta + 0.1f;
        sin_theta = qfp_fsin(theta);
        debugger_scan_variable(global_time_stamp);
        LED1_TOGGLE();
        delayms(500);
    }
}
