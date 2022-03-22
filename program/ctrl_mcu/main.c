/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern unsigned int proc_package[READ_MCU_AMOUNT];
extern decode_proc proc_data;

int main(void) {
    delay_config();
    uart6_config();
    uart6_dma_nvic_config();
//    uart6_dma_receive_config();
    led_config();
    uart1_config();
    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_float32, &proc_data.distance_north, "nd");
    debugger_register_variable(dbg_float32, &proc_data.distance_east, "ed");
    debugger_register_variable(dbg_float32, &proc_data.north_angle, "north");
    timer2_config();
    while (1) {
        for (unsigned short packets_counter = 0; packets_counter < READ_MCU_AMOUNT; packets_counter++) {
            if (proc_package[packets_counter] == 0xff
                && proc_package[packets_counter + 15] == 0xff) {
                unpacking_proc_to_control(&proc_package[packets_counter + 1]);
                packets_counter = (packets_counter + 15);  // 移动到包尾位置
            }
        }
        LED1_TOGGLE();
        delayms(500);
    }
}
