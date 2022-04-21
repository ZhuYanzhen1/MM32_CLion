/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

volatile short angle = 150;
volatile unsigned short speed = 0;
unsigned short playground_ind = 0;

extern unsigned int uart6_dma_buffer_1[CTRL_MCU_RECEIVE_AMOUNT];
extern unsigned int uart6_dma_buffer_2[CTRL_MCU_RECEIVE_AMOUNT];

unsigned int uart7_dma_send_buffer[UART7_DMA_SEND_BUFFER] = {0};

int main(void) {
    delay_config();
    led_config();
    uart1_config();
    uart6_config();
    uart6_dma_nvic_config();
    uart6_dma_receive_config(uart6_dma_buffer_1, CTRL_MCU_RECEIVE_AMOUNT);
    uart6_dma_set_transmit_buffer(uart6_dma_buffer_1, CTRL_MCU_RECEIVE_AMOUNT);
    uart7_config();
//    uart7_dma_nvic_config();
//    uart7_dma_receive_config(uart7_dma_receive_buffer, UART7_DMA_RECEIVE_BUFFER);
    uart7_dma_sent_config(uart7_dma_send_buffer, UART7_DMA_SEND_BUFFER);
    cm_backtrace_config("mm32f3277", "1.0.1", "1.0.1");
    debugger_register_variable(dbg_uint16, &control_signal.joystick_x, "x");
    debugger_register_variable(dbg_uint16, &control_signal.joystick_y, "y");
    debugger_register_variable(dbg_float32, &proc_data.distance_north, "nd");
    debugger_register_variable(dbg_float32, &proc_data.distance_east, "ed");
    timer2_config();
    timer3_config();
    timer4_config();
    spi3_config();

    delayms(5000);

//    while (1) {
//        WRITE_REG(TIM3->CCR1, 100);           // 控制舵机打角，输入值范围100~200

//        float fai_r = 0.23f, delta_r = 0.086f, L = 0.4f, v_r = 5.1f, dt = 0.01f;
//        float error_x = 1.2f, error_y = 0.4f, error_fai = 0.17f;
//        float a[3][3] = {{1, 0, -v_r * dt * qfp_fsin(fai_r)},
//                         {0, 1, v_r * dt * qfp_fcos(fai_r)},
//                         {0, 0, 1}};
//        float b[3][2] = {{qfp_fcos(fai_r) * dt, 0},
//                         {qfp_fsin(fai_r) * dt, 0},
//                         {qfp_ftan(delta_r) * dt / L, v_r * dt / (L * qfp_fcos(delta_r) * qfp_fcos(delta_r))}};
//        float x[3][1] = {{error_x}, {error_y}, {error_fai}};
//        float p[3][3] = {0};
//        float control_val[2][1] = {0};
//        float r = 1;
//        float q = 1;

//        solve_riccati_equation(a, b, q, r, p);
//        solve_feedback_value(p, a, b, x, r, control_val);
//        printf("dv=%.4f ds=%.4f\r\n", control_val[0][0], control_val[1][0]);
//        _fflush(stdout);
//        for (unsigned char i = 0; i < UART7_DMA_SEND_BUFFER; i++)
//            uart7_dma_send_buffer[i] = 3;
//}
//    for (int counter = 0; counter < 10; ++counter) {
//        WRITE_REG(TIM3->CCR1, 110);
//        sdtp_data_transmit_speed(3000, uart7_dma_send_buffer);
//        uart7_dma_set_send_buffer(uart7_dma_send_buffer, UART7_DMA_SEND_BUFFER);
//        delayms(1000);
//    }
//
//    while (1) {
//        WRITE_REG(TIM3->CCR1, 150);
//        sdtp_data_transmit_speed(0, uart7_dma_send_buffer);
//        uart7_dma_set_send_buffer(uart7_dma_send_buffer, UART7_DMA_SEND_BUFFER);
//        delayms(1000);
//    }
    w25q32_write_enable();
    unsigned char write_data[128] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                                     11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                     21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                                     31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                                     41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
                                     51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
                                     61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
                                     71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
                                     81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
                                     91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
                                     101, 102, 103, 104, 105, 106, 107, 108,
                                     109, 110, 111, 112, 113, 114, 115, 116,
                                     117, 118, 119, 120, 121, 122, 123, 124,
                                     125, 126, 127, 128};

    unsigned char read_data[128] = {0};
//    w25q32_write(write_data, 0, 128);

    w25q32_read(read_data, 0, 128);
    unsigned char counter = 0;
    while (1) {
        // 前，左是 0~32766
//        angle = 200 - control_signal.joystick_x / 655;
//        if (control_signal.joystick_y > 32766)
//            speed = 2000;
//        else
//            speed = 2000 + (32767 - control_signal.joystick_y) * 3000 / 32766;
        if (proc_data.distance_east != 0) {
            /* 国防生 */
//            if (playground_ind < 837)
//                playground_ind =
//                    dichotomy(((playground_ind - 2) <= 0) ? 0 : (playground_ind - 2),
//                              (playground_ind + INDEX_OFFSET > 837) ? 837 : (playground_ind + INDEX_OFFSET));
//
//            lqr_control(playground_ind);
//            WRITE_REG(TIM3->CCR1, angle);
//            printf("%.3f, %.3f \r", proc_data.distance_north, proc_data.distance_east);
            /* 工一楼顶 */
            if (playground_ind < 175)
                playground_ind =
                    dichotomy(((playground_ind - 2) <= 0) ? 0 : (playground_ind - 2),
                              (playground_ind + INDEX_OFFSET > 175) ? 175 : (playground_ind + INDEX_OFFSET));

            lqr_control(playground_ind);
            WRITE_REG(TIM3->CCR1, angle);
//            printf("%.3f, %.3f \r", proc_data.distance_north, proc_data.distance_east);
        }


//        sdtp_data_transmit_speed(speed, uart7_dma_send_buffer);
//        uart7_dma_set_send_buffer(uart7_dma_send_buffer, UART7_DMA_SEND_BUFFER);
        delayms(200);

    }
}
