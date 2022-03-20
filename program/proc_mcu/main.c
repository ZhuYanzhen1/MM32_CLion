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

//////////////////////////////////// Task Handler ////////////////////////////////////
TaskHandle_t led_taskhandler;
TaskHandle_t gui_taskhandler;
TaskHandle_t touch_taskhandler;
TaskHandle_t initialize_taskhandler;
void touchscan_task(void *parameters);
void guiupdate_task(void *parameters);
void ledblink_task(void *parameters);
void initialize_task(void *parameters);

EventGroupHandle_t touch_event = NULL;

int main(void) {
    xTaskCreate(initialize_task, "initialize", 1024, NULL, 1, &initialize_taskhandler);
    vTaskStartScheduler();
    return 0;
}

//////////////////////////////////////// Tasks ////////////////////////////////////////
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

void touchscan_task(void *parameters) {
    touch_event = xEventGroupCreate();
    while (1) {
        xEventGroupWaitBits(touch_event, 0x00000001, pdTRUE, pdFALSE, portMAX_DELAY);
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
//        gui_show_gnrmc_debug();
        delayms(100);
    }
}

void ledblink_task(void *parameters) {
    while (1) {
        LED1_TOGGLE();
        delayms(500);
    }
}
