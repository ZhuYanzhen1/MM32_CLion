/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern calpara_t params;
extern unsigned char packages_to_be_unpacked[READ_MCU_AMOUNT];

TaskHandle_t led_taskhandler;
TaskHandle_t gui_taskhandler;
TaskHandle_t touch_taskhandler;
TaskHandle_t initialize_taskhandler;
void touchscan_task(void *parameters);
void guiupdate_task(void *parameters);
void ledblink_task(void *parameters);
void initialize_task(void *parameters);

void gui_show_fix() {
    gui_printf(0, 0 * 12, C_BLACK, C_WHITE,
               "ax:%d   ", small_packets.ax);
    gui_printf(0, 1 * 12, C_BLACK, C_WHITE,
               "ay:%d   ", small_packets.ay);
    gui_printf(0, 2 * 12, C_BLACK, C_WHITE,
               "az:%d   ", small_packets.az);
    gui_printf(0, 3 * 12, C_BLACK, C_WHITE,
               "north:%d   ", small_packets.north);
    gui_printf(0, 4 * 12, C_BLACK, C_WHITE,
               "kalman_north:%d   ", small_packets.kalman_north);
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

void initialize_task(void *parameters) {
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
    debugger_register_variable(dbg_int16, &small_packets.north, "north");
    debugger_register_variable(dbg_float32, &small_packets.kalman_north, "compass");
    timer2_config();

    xTaskCreate(ledblink_task, "led_blink", 64, NULL, 1, &led_taskhandler);
    xTaskCreate(guiupdate_task, "gui_update", 2048, NULL, 1, &gui_taskhandler);
    xTaskCreate(touchscan_task, "touch_scan", 1024, NULL, 1, &touch_taskhandler);
    vTaskDelete(NULL);
}

int main(void) {
    xTaskCreate(initialize_task, "initialize", 1024, NULL, 1, &initialize_taskhandler);
    vTaskStartScheduler();
    return 0;
}

void touchscan_task(void *parameters) {
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

void guiupdate_task(void *parameters) {
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
        gui_show_fix();
//        gui_show_debug();
//        gui_show_gnrmc_information();
//        show_gnrmc_debug();
        delayms(100);
    }
}

void ledblink_task(void *parameters) {
    while (1) {
        LED1_TOGGLE();
        delayms(500);
    }
}
