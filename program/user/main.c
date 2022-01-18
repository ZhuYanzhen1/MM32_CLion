/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"


//
//TODO 1、如果不注释下列代码，会提示说找不到寄存器 BASEPRI；
// 2、set_BASEPRI(uint32_t basePri) 这个参数指的是什么；
// 3、两个串口的DMA中断优先级怎么配置；中断分组要怎么分；
// 4、哪些地方要防止函数重入；
// 5、怎么验证是否串口1的DMA可用；
// 6、
//


extern nmea_rmc gps_rmc;
static float theta = 0, sin_theta = 0;
int main(void) {
    delay_config();
    led_config();
#ifdef IS_PROCESS_MCU
    delayms(2000);
    spi2_config();
//    xpt2046_config();
    gui_config();
//    xpt2046_calibrate();
    uart1_config(115200);
    uart3_config(115200);
    uart6_config(9600);
    gps_config();
//    uart1_dma_nvic_config();
//    uart1_dma_sent_config();

#endif
    cm_backtrace_init("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_float32, &theta, "theta");
    debugger_register_variable(dbg_float32, &sin_theta, "sin_theta");
    while (1) {
#ifdef IS_PROCESS_MCU
        gui_show_gnrmc_information(&gps_rmc);
#endif
        LED1_TOGGLE();
        delayms(200);
    }
}
