/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.6.1
    \date     17. July 2022
 ******************************************************************************/

#include "main.h"

extern volatile unsigned char lqr_flag;

FATFS filesystem;
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

    delayms(1000);
    cm_backtrace_config("CtrlMCU", "1.6.1", "1.6.1");
    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
    printf("CtrlMCU %s %s %s\r\n", __DATE__, __TIME__, GIT_HASH);
    timer2_config();

    timer4_config();
    spi3_config();
    delayms(2000);
    timer3_config();

    FRESULT result = f_mount(&filesystem, "0:", 1);
    if (result == FR_NO_FILESYSTEM) {
        printf("Making FileSystem...\r\n");
        if (f_mkfs("0:", 0, fs_buffer, sizeof(fs_buffer)) == FR_OK) {
            f_setlabel((const TCHAR *) "0:SD");
            printf("Make FileSystem Success\r\n");
        } else {
            printf("Error To Make FileSystem\r\n");
            while (1);
        }
    } else if (result != FR_OK) {
        printf("Error To Mount FileSystem\r\n");
        while (1);
    }
    printf("FileSystem Mount Success!\r\n");
    fs_scan_files("0:");

    static unsigned char find_counter = 0;
    static unsigned short start_point = 0;
    volatile static unsigned short index = 0;
    while (1) {
//        for (unsigned char i = 0; i < 100; i++) {
////            if (i < 10) {
////                speed = 10000;
////                sdtp_data_transmit_speed(speed, uart7_dma_send_buffer);
////                uart7_dma_set_send_buffer(uart7_dma_send_buffer, UART7_DMA_SEND_BUFFER);
////            } else {
//////                speed = (speed > 5000) ? (speed - 5000) : 0;
////                speed = 0;
////                sdtp_data_transmit_speed(speed, uart7_dma_send_buffer);
////                uart7_dma_set_send_buffer(uart7_dma_send_buffer, UART7_DMA_SEND_BUFFER);
////            }
//            speed = 0;
//            sdtp_data_transmit_speed(speed, uart7_dma_send_buffer);
//            uart7_dma_set_send_buffer(uart7_dma_send_buffer, UART7_DMA_SEND_BUFFER);
//            delayms(400);
//        }
//    }

        while (1) { // 寻点稳定再发车
            LED1_TOGGLE();
            if (proc_data.distance_east != 0 && lqr_flag == 1) {
                lqr_flag = 0;
                playground_ind =
                    dichotomy(((playground_ind - 2) <= 0) ? 0 : (playground_ind - 2),
                              (playground_ind + INDEX_OFFSET > INDEX_NUM) ? INDEX_NUM : (playground_ind
                                  + INDEX_OFFSET));
                start_point = playground_ind;
                find_counter++;
                if (find_counter > 10)
                    break;
                LED1_TOGGLE();
                delayms(50);
            }
        }
        while (1) {
            if (proc_data.distance_east != 0) {
                for (unsigned char i = 0; i < 20; i++) {
                    while (1) {
                        if (lqr_flag == 1) {
                            lqr_flag = 0;
                            LED1_TOGGLE();

                            basic_status_t current_status = {proc_data.distance_north,
                                                             proc_data.distance_east,
                                                             proc_data.north_angle};

                            playground_ind = find_index(playground_ind);

                            index = playground_ind + OVERRUN_POINT;
                            unsigned char angle = lqr_control(index, current_status);

                            WRITE_REG(TIM3->CCR1, angle);
                            break;
                        }
                    }
                }
                speed = 25000;  // 23000
                if (playground_ind > INDEX_NUM - 130)
                    speed = 0;
                sdtp_data_transmit_speed(speed, uart7_dma_send_buffer);
                uart7_dma_set_send_buffer(uart7_dma_send_buffer, UART7_DMA_SEND_BUFFER);
//            printf("%.3f, %.3f , \r\n", proc_data.distance_north, proc_data.distance_east);
            }
        }
    }
}
