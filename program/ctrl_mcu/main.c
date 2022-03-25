/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern unsigned int uart6_dma_buffer_1[PROC_MCU_SEND_AMOUNT];
extern unsigned int uart6_dma_buffer_2[PROC_MCU_SEND_AMOUNT];

int main(void) {
    unsigned char counter = 150;
    delay_config();
    led_config();
    uart1_config();
    uart6_config();
    uart6_dma_nvic_config();
    uart6_dma_receive_config(uart6_dma_buffer_1, PROC_MCU_SEND_AMOUNT);
    uart6_dma_set_transmit_buffer(uart6_dma_buffer_1, PROC_MCU_SEND_AMOUNT);
    uart7_config();
    uart7_dma_nvic_config();
//    uart7_dma_receive_config(uart7_dma_buffer_1, 0);
//    uart7_dma_set_transmit_buffer(uart7_dma_buffer_1, 0);
    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_float32, &proc_data.distance_north, "nd");
    debugger_register_variable(dbg_float32, &proc_data.distance_east, "ed");
    debugger_register_variable(dbg_float32, &proc_data.north_angle, "north");
    timer2_config();
    timer3_config();
    while (1) {
        WRITE_REG(TIM3->CCR1, counter);
        counter++;
        if (counter == 200) {
            while (1) {
                counter--;
                WRITE_REG(TIM3->CCR1, counter);
                if (counter == 100)
                    break;
                delayms(20);
            }
        }
        delayms(20);
    }
}
