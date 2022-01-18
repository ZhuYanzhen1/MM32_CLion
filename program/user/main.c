/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"


//
//TODO  3、两个串口的DMA中断优先级怎么配置；调试的统一优先级往低了设置
// 4、哪些地方要防止函数重入；main和中断同时用到的函数；用到的时候记得留意一下，然后到Trello里面区评论记录
//

extern nmea_rmc gps_rmc;
static float theta = 0, sin_theta = 0;

unsigned int debug_dma[12] = {0};

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
    uart1_dma_nvic_config();
#endif
    cm_backtrace_init("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_float32, &theta, "theta");
    debugger_register_variable(dbg_float32, &sin_theta, "sin_theta");
    timer2_config();

    /*
    __set_BASEPRI(6 << 4);//屏蔽所有主优先级号>=x的中断（X根据你的需要来设置）//X只能是偶数：2、4、6、8
    //临界区
    __set_BASEPRI(0);//恢复中断状态
*/

    while (1) {
#ifdef IS_PROCESS_MCU
        theta += 0.01f;
        sin_theta = qfp_fsin(theta);
        gui_show_gnrmc_information(&gps_rmc);
#endif
        LED1_TOGGLE();
        delayms(100);
    }
}
