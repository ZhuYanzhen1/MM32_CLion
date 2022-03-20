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
float v_north;
float v_east;
kalman_filter_float kalman_v_north = {0};
kalman_filter_float kalman_v_east = {0};
float v_north_final;
float v_east_final;

/* Kalman fusion to obtain northward displacement and eastward displacement */
float distance_north;
float distance_east;
kalman_filter_float kalman_distance_north = {0};
kalman_filter_float kalman_distance_earth = {0};

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
    debugger_register_variable(dbg_float32, &small_packets.kalman_north, "kalman");
    debugger_register_variable(dbg_int16, &small_packets.north, "north");
    timer2_config();

    xTaskCreate(fusion_task, "sensor_fusion", 4096, NULL, 2, &fusion_taskhandler);
    xTaskCreate(ledblink_task, "led_blink", 64, NULL, 1, &led_taskhandler);
    xTaskCreate(guiupdate_task, "gui_update", 2048, NULL, 1, &gui_taskhandler);
    xTaskCreate(touchscan_task, "touch_scan", 1024, NULL, 1, &touch_taskhandler);
    vTaskDelete(NULL);
}

void fusion_task(void *parameters) {
    kalman_config_v(&kalman_v_north);
    kalman_config_v(&kalman_v_east);
    kalman_config_distance(&kalman_distance_north, 384400);
    kalman_config_distance(&kalman_distance_earth, 1487900);
    while (1) {
        while (gps_rmc.status != 'A')
            delayms(1);
        coordinate_system_transformation_neu(small_packets.kalman_north);

        v_north = kalman_update(&kalman_v_north, neu.north_v, neu.north_acceleration,
                                0.031f, 0);
        v_east = kalman_update(&kalman_v_east, neu.east_v, neu.east_acceleration,
                               0.031f, 0);
        if (v_north < 1 && v_north > -1) v_north_final = neu.north_v;
        else v_north_final = v_north;
        if (v_east < 1 && v_east > -1) v_east_final = neu.east_v;
        else v_east_final = v_east;
        distance_north = kalman_update(&kalman_distance_north, neu.north_distance,
                                       v_north_final, 0.031f, 0);   // dt不清楚，估计得改
        distance_east = kalman_update(&kalman_distance_earth, neu.east_distance,
                                      v_east_final, 0.031f, 0);     // dt不清楚，估计得改
        delayms(30);
    }
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
    Button_Struct_t test_btn;
    test_btn.x_pos = 10;
    test_btn.y_pos = 120;
    test_btn.width = 60;
    test_btn.height = 30;
    test_btn.Text = "Test";
    gui_button_init(&test_btn);
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
        gui_show_fusion();
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
