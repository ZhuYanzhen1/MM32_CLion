/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern calpara_t params;

/////////////////////////////////////// TaskHandler ///////////////////////////////////////
static struct rt_thread led_taskhandler;
static unsigned char ledtask_stack[128];
static struct rt_thread gui_taskhandler;
static unsigned char guitask_stack[4096];
static struct rt_thread touch_taskhandler;
static unsigned char touchtask_stack[1024];

void ledblink_task(void *parameter);
void guiupdate_task(void *parameter);
void touchscan_task(void *parameter);

/////////////////////////////////////// Initialize Task ///////////////////////////////////////
void hardware_init(void) {
    delay_config();
    led_config();
    iic1_config();
    spi2_config();
    spi3_config();
    uart1_config();
    uart3_config();
    uart6_config();
    uart8_config();
    xpt2046_gpio_config();
    cm_backtrace_config("mm32f3277", "1.3.3", "1.3.3");
}

int main(void) {
    gps_config();
    gui_config();
    at24c02_readparams();
//    kalman_config();
//    calibration_acll();
    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
    timer2_config();

    rt_thread_init(&led_taskhandler, "led", ledblink_task, RT_NULL,
                   &ledtask_stack[0], sizeof(ledtask_stack), 7, 1);
    rt_thread_startup(&led_taskhandler);

    rt_thread_init(&gui_taskhandler, "gui", guiupdate_task, RT_NULL,
                   &guitask_stack[0], sizeof(guitask_stack), 7, 1);
    rt_thread_startup(&gui_taskhandler);

    rt_thread_init(&touch_taskhandler, "touch", touchscan_task, RT_NULL,
                   &touchtask_stack[0], sizeof(touchtask_stack), 7, 1);
    rt_thread_startup(&touch_taskhandler);
    return 0;
}

/////////////////////////////////////// Task Function ///////////////////////////////////////
void touchscan_task(void *parameter) {
    while (1) {
        if (!GPIO_ReadInputDataBit(TOUCH_PEN_PORT, TOUCH_PEN_PIN)) {
            unsigned char x_pos, y_pos;
            xpt2046_scan(&x_pos, &y_pos);
            while (!GPIO_ReadInputDataBit(TOUCH_PEN_PORT, TOUCH_PEN_PIN))
                delayms(50);
        }
        delayms(50);
    }
}

void guiupdate_task(void *parameter) {
    while (1) {
        gui_show_gnrmc_information();
        delayms(200);
    }
}

void ledblink_task(void *parameter) {
    while (1) {
        LED1_TOGGLE();
        delayms(500);
    }
}
