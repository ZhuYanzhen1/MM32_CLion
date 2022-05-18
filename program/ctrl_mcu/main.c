/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

//#define INDEX_NUM   747

extern volatile unsigned char lqr_flag;

FATFS filesystem;
volatile short angle = 158;
volatile unsigned short speed = 0;
unsigned short playground_ind = 0;
static unsigned char fs_buffer[FF_MAX_SS * 4];

extern unsigned int uart6_dma_buffer_1[CTRL_MCU_RECEIVE_AMOUNT];
extern unsigned int uart6_dma_buffer_2[CTRL_MCU_RECEIVE_AMOUNT];

unsigned int uart7_dma_send_buffer[UART7_DMA_SEND_BUFFER] = {0};

int main(void) {
    delay_config();
    tim3_pwm_gpio_config();
    led_config();
    uart1_config();
    uart6_config();
    uart6_dma_nvic_config();
    uart6_dma_receive_config(uart6_dma_buffer_1, CTRL_MCU_RECEIVE_AMOUNT);
    uart6_dma_set_transmit_buffer(uart6_dma_buffer_1, CTRL_MCU_RECEIVE_AMOUNT);
    uart7_config();
    uart7_dma_sent_config(uart7_dma_send_buffer, UART7_DMA_SEND_BUFFER);
    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_float32, &proc_data.distance_north, "nd");
    debugger_register_variable(dbg_float32, &proc_data.distance_east, "ed");
    timer2_config();
    timer4_config();
    spi3_config();
    delayms(2000);
    timer3_config();

    FRESULT result = f_mount(&filesystem, "0:", 1);
    if (result == FR_NO_FILESYSTEM) {
        if (f_mkfs("0:", 0, fs_buffer, sizeof(fs_buffer)) == FR_OK)
            f_setlabel((const TCHAR *) "0:FLASH");
        else
            while (1);
    } else if (result != FR_OK)
        while (1);

    fs_get_free("0:");
    fs_scan_files("0:");

//    ch372_config();

    while (1) {
        LED1_TOGGLE();
        if (proc_data.distance_east != 0) {
            for (unsigned char i = 0; i < 20; i++) {
                while (1) {
                    if (lqr_flag == 1) {
                        lqr_flag = 0;
                        playground_ind =
                            dichotomy(((playground_ind - 2) <= 0) ? 0 : (playground_ind - 2),
                                      (playground_ind + INDEX_OFFSET > INDEX_NUM) ? INDEX_NUM : (playground_ind
                                          + INDEX_OFFSET));

                        lqr_control(playground_ind + 5);
                        WRITE_REG(TIM3->CCR1, angle);
                        break;
                    }
                }
            }
            speed = 4000;
            sdtp_data_transmit_speed(speed, uart7_dma_send_buffer);
            uart7_dma_set_send_buffer(uart7_dma_send_buffer, UART7_DMA_SEND_BUFFER);
            printf("%.3f, %.3f , \r\n", proc_data.distance_north, proc_data.distance_east);
        }

//        if (playground_ind > INDEX_NUM - 100) {
//            for (unsigned short i = 0; i < 800; i++) {
//                speed = (speed > 1000) ? (speed - 1000) : 0;
//                sdtp_data_transmit_speed(speed, uart7_dma_send_buffer);
//                uart7_dma_set_send_buffer(uart7_dma_send_buffer, UART7_DMA_SEND_BUFFER);
//                WRITE_REG(TIM3->CCR1, 158);
//                delayms(400);
//            }
//            while (1);
//        }
    }
}
