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
//        WRITE_REG(TIM3->CCR1, counter);
//        counter++;
//        if (counter == 200) {
//            while (1) {
//                counter--;
//                WRITE_REG(TIM3->CCR1, counter);
//                if (counter == 100)
//                    break;
//                delayms(20);
//            }
//        }

        float fai_r = 0.23f, delta_r = 0.086f, L = 0.4f, v_r = 5.1f, dt = 0.01f;
        float Matrix_A[3][3] = {{1, 0, -v_r * dt * qfp_fsin(fai_r)},
                                {0, 1, v_r * dt * qfp_fcos(fai_r)},
                                {0, 0, 1}};
        float Matrix_B[3][2] = {{qfp_fcos(fai_r) * dt, 0},
                                {qfp_fsin(fai_r) * dt, 0},
                                {qfp_ftan(delta_r) * dt / L, v_r * dt / (L * qfp_fcos(delta_r) * qfp_fcos(delta_r))}};
        float Q = 1;
        float R = 1;
        float Matrix_P[3][3] = {0};
        LED1_ON();
        solveRiccatiIteration(Matrix_A, Matrix_B, Q, R, Matrix_P);
        LED1_OFF();

        printf("------------Matrix P------------\r\n");
        _fflush(stdout);
        delayms(30);
        for (int counter1 = 0; counter1 < 3; ++counter1) {
            for (int counter2 = 0; counter2 < 3; ++counter2)
                printf("%f   ", Matrix_P[counter1][counter2]);
            printf("\r\n");
            _fflush(stdout);
            delayms(30);
        }

        delayms(20);
    }
}
