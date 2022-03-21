/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#include "main.h"

extern unsigned char packages_to_be_unpacked[READ_MCU_AMOUNT];

/* Kalman fusion to obtain northward and eastward velocities
 * (GPS velocity + imu acceleration) */
kalman_data_t kalman_data;
kalman_filter_t kalman_v = {0};
kalman_filter_t kalman_distance_north = {0};
kalman_filter_t kalman_distance_earth = {0};

//////////////////////////////////// Task Handler ////////////////////////////////////
TaskHandle_t led_taskhandler;
TaskHandle_t gui_taskhandler;
TaskHandle_t touch_taskhandler;
TaskHandle_t fusion_taskhandler;
TaskHandle_t initialize_taskhandler;
void touchscan_task(void *parameters);
void guiupdate_task(void *parameters);
void ledblink_task(void *parameters);
void fusion_task(void *parameters);
void initialize_task(void *parameters);

EventGroupHandle_t touch_event = NULL;

int main(void) {
    xTaskCreate(initialize_task, "initialize", 1024, NULL, 1,
                &initialize_taskhandler);
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

    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
    debugger_register_variable(dbg_float32, &small_packets.kalman_north, "kalman");
    timer2_config();

    xTaskCreate(fusion_task, "sensor_fusion", 4096, NULL, 2,
                &fusion_taskhandler);
    xTaskCreate(ledblink_task, "led_blink", 64, NULL, 1,
                &led_taskhandler);
    xTaskCreate(guiupdate_task, "gui_update", 2048, NULL, 1,
                &gui_taskhandler);
    xTaskCreate(touchscan_task, "touch_scan", 1024, NULL, 1,
                &touch_taskhandler);
    vTaskDelete(NULL);
}

void fusion_task(void *parameters) {
    kalman_config_v(&kalman_v);
    kalman_config_distance(&kalman_distance_north, 384400);
    kalman_config_distance(&kalman_distance_earth, 1487900);
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
        while (gps_rmc.status != 'A')
            delayms(1);
        sensor_unit_conversion();
        kalman_data.v = kalman_update(&kalman_v, neu.v, neu.acceleration,
                                      0.031f, 0);
        coordinate_system_transformation_kalman_v(small_packets.kalman_north);
        kalman_data.distance_north = kalman_update(&kalman_distance_north, neu.north_distance,
                                                   kalman_data.v_north, 0.031f, 0);
        kalman_data.distance_east = kalman_update(&kalman_distance_earth, neu.east_distance,
                                                  kalman_data.v_east, 0.031f, 0);
        delayms(30);
    }
}

void touchscan_task(void *parameters) {
    touch_event = xEventGroupCreate();
    while (1) {
        xEventGroupWaitBits(touch_event, 0x00000001, pdTRUE, pdFALSE,
                            portMAX_DELAY);
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
    form_struct_t test_form;
    label_struct_t test_label;

    test_label.colum = 0;
    test_label.align = label_align_right;
    test_label.color = C_RED;
    gui_label_settext(&test_label, "Hello");
    gui_label_init(&test_label);

    test_form.text = "MainWindow";
    gui_form_init(&test_form);

    gui_form_bind_label(&test_form, &test_label);
    gui_form_display(&test_form);

    while (1) {
//        gui_show_fusion();
//        gui_show_fix();
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
