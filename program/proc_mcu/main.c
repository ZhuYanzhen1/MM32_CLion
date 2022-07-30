/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu & ZGL
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

#include "main.h"

/* Statistics of other controllers */
unsigned char pc_connect_flag = 0;
float statistic_max_speed = 0;
float statistic_max_current = 0;
extern unsigned char battery_current;

/* Variables related to gps */
extern unsigned int temp_stable[100][2];
extern unsigned int packages_to_be_unpacked_1[READ_MCU_AMOUNT];
unsigned int proc_to_ctrl_package[PROC_MCU_SEND_AMOUNT] = {0};
unsigned int proc_to_ctrl_buffer[4] = {0};

/* Kalman fusion to obtain northward and eastward velocities */
kalman_data_t kalman_data = {0};
kalman_filter_t kalman_v = {0};
kalman_filter_t kalman_distance_north = {0};
kalman_filter_t kalman_distance_east = {0};

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
QueueHandle_t touch_point_queue;

int main(void) {
    xTaskCreate(initialize_task, "initialize", 1024, NULL, 1,
                &initialize_taskhandler);
    vTaskStartScheduler();
    return 0;
}

//////////////////////////////////////// Tasks ////////////////////////////////////////
void initialize_task(void *parameters) {
    (void) parameters;
    delay_config();
    led_config();
    iic1_config();
    at24c02_readparams();
    spi2_config();
    spi3_config();
    uart1_config();
    uart2_config();
    uart2_dma_nvic_config();
    uart2_dma_receive_config(packages_to_be_unpacked_1, uart2_dma_buffer_size);
    uart2_dma_set_transmit_buffer(packages_to_be_unpacked_1, uart2_dma_buffer_size);
    uart4_config();
    uart6_config();
//    for (unsigned char counter = 0; counter < 5; ++counter) {
//        uart4_sendbyte(pc_connect_flag);
//        delayms(200);
//    }
    xpt2046_gpio_config();
    cm_backtrace_config("ProcMCU", "1.6.1", "1.6.1");
    gui_config();
    delayms(2000);
    uart3_config();
    gps_config();
#if STARTUP_CALIBRATE == 1
    xpt2046_calibrate();
    at24c02_saveparams();
#endif

    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
    printf("ProcMCU %s %s %s\r\n", __DATE__, __TIME__, GIT_HASH);
    timer2_config();

    xTaskCreate(fusion_task, "sensor_fusion", 1024, NULL, 4,
                &fusion_taskhandler);
    xTaskCreate(ledblink_task, "led_blink", 1024, NULL, 2,
                &led_taskhandler);
    xTaskCreate(guiupdate_task, "gui_update", 2048, NULL, 2,
                &gui_taskhandler);
    xTaskCreate(touchscan_task, "touch_scan", 128, NULL, 2,
                &touch_taskhandler);
    vTaskDelete(NULL);
}

void fusion_task(void *parameters) {
    (void) parameters;
    kalman_config_v(&kalman_v);
        status_V:
    while (gps_rmc.status == 'V') {
        delayms(20);
        proc_to_ctrl_buffer[0] = 0;
        proc_to_ctrl_buffer[1] = 0;
        proc_to_ctrl_buffer[2] = 0;
        proc_to_ctrl_buffer[3] = 0;
        precossing_proc_to_control(proc_to_ctrl_package, proc_to_ctrl_buffer);
        for (unsigned char i = 0; i < PROC_MCU_SEND_AMOUNT; i++) {
            uart3_sendbyte(proc_to_ctrl_package[i]);
        }
    }
    unsigned char stable_flag = 0;
    while (stable_flag == 0) {
        for (unsigned short i = 0; i < STABLE_NUM; i++) {
            unsigned int same_counter = 0;
            for (unsigned short j = i + 1; j < STABLE_NUM; j++) {
                if (temp_stable[j][0] == 0)
                    break;
                if (temp_stable[i][0] == temp_stable[j][0] && temp_stable[i][1] == temp_stable[j][1])
                    same_counter++;
            }
            if (same_counter > 20) {
                stable_flag = 1;
                break;
            }
        }
        delayms(200);
    }
    kalman_config_distance(&kalman_distance_north, neu.north_distance);
    kalman_config_distance(&kalman_distance_east, neu.east_distance);
    unsigned int last_glbal_time_stamp = global_time_stamp - 21;
    while (1) {
        float dt = (float) (global_time_stamp - last_glbal_time_stamp) * 0.001f;
        last_glbal_time_stamp = global_time_stamp;
        if (gps_rmc.status == 'V')
            goto status_V;

        sensor_unit_conversion();
        float last_output_v = kalman_data.v;
        kalman_data.v = kalman_update(&kalman_v, neu.v, neu.acceleration, dt);
        kalman_data.v = kalman_data.v * 0.3f + 0.7f * last_output_v;
        statistic_max_speed = kalman_data.v > statistic_max_speed ? kalman_data.v : statistic_max_speed;

        kalman_v.gps_valid_flag = 1;
        coordinate_system_transformation_kalman_v(small_packets.north);
        kalman_data.distance_north = kalman_update(&kalman_distance_north, neu.north_distance,
                                                   neu.north_v, dt);
        kalman_data.distance_east = kalman_update(&kalman_distance_east, neu.east_distance,
                                                  neu.east_v, dt);

        proc_to_ctrl_buffer[0] = *((unsigned int *) (&kalman_data.distance_north));
        proc_to_ctrl_buffer[1] = *((unsigned int *) (&kalman_data.distance_east));
        proc_to_ctrl_buffer[2] = *((unsigned int *) (&small_packets.north));
        proc_to_ctrl_buffer[3] = *((unsigned int *) (&kalman_data.v));
        precossing_proc_to_control(proc_to_ctrl_package, proc_to_ctrl_buffer);
        for (unsigned char i = 0; i < PROC_MCU_SEND_AMOUNT; i++)
            uart4_sendbyte(proc_to_ctrl_package[i]);

        statistic_max_current =
            (float) battery_current > statistic_max_current ? (float) battery_current : statistic_max_current;

        delayms(39);
    }
}

void touchscan_task(void *parameters) {
    unsigned char x_pos, y_pos;
    unsigned short combine_pos;
    (void) parameters;
    touch_event = xEventGroupCreate();
    touch_point_queue = xQueueCreate(10, 2);
    while (1) {
        xEventGroupWaitBits(touch_event, 0x00000001, pdTRUE, pdFALSE, portMAX_DELAY);
        EXTI->IMR &= ~EXTI_Line4;
        xpt2046_scan(&x_pos, &y_pos);
        combine_pos = x_pos << 8 | y_pos;
        xQueueSend(touch_point_queue, &combine_pos, 0);
        while (!GPIO_ReadInputDataBit(TOUCH_PEN_PORT, TOUCH_PEN_PIN))
            delayms(20);
        delayms(200);
        EXTI_ClearFlag(EXTI_Line4);
        EXTI->IMR |= EXTI_Line4;
    }
}

void guiupdate_task(void *parameters) {
    unsigned short combine_pos = 0x0000;
    (void) parameters;
    gui_main_form_init();
    while (1) {
        delayms(200);
        if (xQueueReceive(touch_point_queue, &combine_pos, 0) == pdPASS)
            gui_form_update(combine_pos >> 8, combine_pos & 0x00FF);
        else
            gui_form_update(0, 0);
    }
}

#if USE_FREERTOS_REPORT == 1
static char pcWriteBuffer[360] = {0};
void print_task_status(void);
void ledblink_task(void *parameters) {
    unsigned char led_counter = 0;
    (void) parameters;
    while (1) {
        led_counter++;
        LED1_TOGGLE();
        delayms(500);
        if (led_counter == 20) {
            print_task_status();
            led_counter = 0;
        }
    }
}
void print_task_status(void) {
    vTaskList((char *) &pcWriteBuffer);
    char tmp_c = pcWriteBuffer[120];
    pcWriteBuffer[120] = 0x00;
    if (tmp_c != 0x00)
        printf("%s", pcWriteBuffer);
    else
        printf("%s\r\n\r\n", pcWriteBuffer);
    _fflush(stdout);
    if (tmp_c != 0x00) {
        delayms(30);
        pcWriteBuffer[120] = tmp_c;
        tmp_c = pcWriteBuffer[240];
        pcWriteBuffer[240] = 0x00;
        if (tmp_c != 0x00)
            printf("%s", &pcWriteBuffer[120]);
        else
            printf("%s\r\n\r\n", &pcWriteBuffer[120]);
        _fflush(stdout);
        if (tmp_c != 0x00) {
            delayms(30);
            pcWriteBuffer[240] = tmp_c;
            printf("%s\r\n\r\n", &pcWriteBuffer[240]);
            _fflush(stdout);
        }
    }
}
#else
void ledblink_task(void *parameters) {
    (void) parameters;
    while (1) {
        LED1_TOGGLE();
        delayms(200);
    }
}
#endif  // USE_FREERTOS_REPORT == 1
