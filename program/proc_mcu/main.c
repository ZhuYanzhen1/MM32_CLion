/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern unsigned char packages_to_be_unpacked[READ_MCU_AMOUNT];

void gui_show_fix() {
    gui_printf(0, 0 * 12, C_BLACK, C_WHITE,
               "ax:%d   ", small_packets.ax);
    gui_printf(0, 1 * 12, C_BLACK, C_WHITE,
               "ay:%d   ", small_packets.ay);
    gui_printf(0, 2 * 12, C_BLACK, C_WHITE,
               "az:%d   ", small_packets.az);
    gui_printf(0, 3 * 12, C_BLACK, C_WHITE,
               "north:%d   ", small_packets.pitch);
    gui_printf(0, 4 * 12, C_BLACK, C_WHITE,
               "yaw:%d   ", small_packets.yaw);
}

void gui_show_debug() {
    gui_printf(0, 0 * 12, C_BLACK, C_WHITE,
               "mag_x:%d   ", debug_data.mag_x);
    gui_printf(0, 1 * 12, C_BLACK, C_WHITE,
               "mag_y:%d   ", debug_data.mag_y);
    gui_printf(0, 2 * 12, C_BLACK, C_WHITE,
               "mag_z:%d   ", debug_data.mag_z);
//    gui_printf(0, 3 * 12, C_BLACK, C_WHITE,
//               "offset_x:%d", debug_data.offset_x);
//    gui_printf(0, 4 * 12, C_BLACK, C_WHITE,
//               "offset_y:%d", debug_data.offset_y);
//    gui_printf(0, 5 * 12, C_BLACK, C_WHITE,
//               "offset_z:%d", debug_data.offset_z);
//    gui_printf(0, 6 * 12, C_BLACK, C_WHITE,
//               "bias_x:%d", debug_data.bias_x);
//    gui_printf(0, 7 * 12, C_BLACK, C_WHITE,
//               "bias_y:%d", debug_data.bias_y);
//    gui_printf(0, 8 * 12, C_BLACK, C_WHITE,
//               "bias_z:%d", debug_data.bias_z);
//    gui_printf(0, 9 * 12, C_BLACK, C_WHITE,
//               "offbias_x:%d", debug_data.offbias_x);
//    gui_printf(0, 10 * 12, C_BLACK, C_WHITE,
//               "offbias_y:%d", debug_data.offbias_y);
//    gui_printf(0, 11 * 12, C_BLACK, C_WHITE,
//               "offbias_z:%d", debug_data.offbias_z);
//    gui_printf(0, 12 * 12, C_BLACK, C_WHITE,
//               "residual:%d", debug_data.residual);
//    gui_printf(0, 13 * 12, C_BLACK, C_WHITE,
//               "step_length:%d", debug_data.step_length);
//    gui_printf(0, 14 * 12, C_BLACK, C_WHITE,
//               "num:%d", debug_data.num);
}

/////////////////////////////////////// TaskHandler ///////////////////////////////////////
static struct rt_thread led_taskhandler;
static unsigned char ledtask_stack[128];
static struct rt_thread gui_taskhandler;
static unsigned char guitask_stack[4096];
static struct rt_thread touch_taskhandler;
static unsigned char touchtask_stack[1024];
struct rt_semaphore touch_semaphore;

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
    uart6_config();
    uart8_config();
    xpt2046_gpio_config();
    cm_backtrace_config("mm32f3277", "1.3.3", "1.3.3");
}

int main(void) {
    gui_config();
    delayms(2000);
    uart3_config();
    gps_config();
//    xpt2046_calibrate();
//    at24c02_saveparams();
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
    rt_sem_init(&touch_semaphore, "touch_s", 0, RT_IPC_FLAG_FIFO);
    while (1) {
        rt_sem_take(&touch_semaphore, RT_WAITING_FOREVER);
        EXTI->IMR &= ~EXTI_Line4;
        unsigned char x_pos, y_pos;
        xpt2046_scan(&x_pos, &y_pos);
        printf("XPos:%d YPos:%d\r\n", x_pos, y_pos);
        fflush(stdout);
        while (!GPIO_ReadInputDataBit(TOUCH_PEN_PORT, TOUCH_PEN_PIN))
            delayms(20);
        EXTI_ClearFlag(EXTI_Line4);
        EXTI->IMR |= EXTI_Line4;
    }
}

void guiupdate_task(void *parameter) {
    while (1) {
        for (unsigned short packets_counter = 0; packets_counter < READ_MCU_AMOUNT; packets_counter++) {
            if (packages_to_be_unpacked[packets_counter] == 0xff
                && packages_to_be_unpacked[packets_counter + 11] == 0xff) {
                unpacking_fixed_length_data(&packages_to_be_unpacked[packets_counter + 1]);
                packets_counter = (packets_counter + 11);  // 移动到包尾位置
            } else if (packages_to_be_unpacked[packets_counter] == 0xa5
                && packages_to_be_unpacked[packets_counter + 1] == 0x5a) {
                unpacking_variable_length_data(&packages_to_be_unpacked[packets_counter + 3]);
                packets_counter = (packets_counter + packages_to_be_unpacked[2] - 1); // 移动到下一个包的前一个位置
            }
        }
//        gui_show_debug();
        gui_show_fix();
//        gui_show_gnrmc_information();
        delayms(50);
    }
}

void ledblink_task(void *parameter) {
    while (1) {
        LED1_TOGGLE();
        delayms(500);
    }
}
