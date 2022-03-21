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

/////////////////////////////////////// TaskHandler ///////////////////////////////////////
static struct rt_thread led_taskhandler;
static unsigned char ledtask_stack[128];
static struct rt_thread gui_taskhandler;
static unsigned char guitask_stack[4096];
static struct rt_thread touch_taskhandler;
static unsigned char touchtask_stack[1024];
static struct rt_thread fusion_taskhandler;
static unsigned char fusion_stack[4096];
struct rt_semaphore touch_semaphore;

void ledblink_task(void *parameter);
void guiupdate_task(void *parameter);
void touchscan_task(void *parameter);
void fusion_task(void *parameter);

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

    rt_thread_init(&fusion_taskhandler, "fusion", fusion_task, RT_NULL,
                   &fusion_stack[0], sizeof(fusion_stack), 7, 1);
    rt_thread_startup(&fusion_taskhandler);
    return 0;
}

/////////////////////////////////////// Task Function ///////////////////////////////////////
void touchscan_task(void *parameters) {
//    touch_event = xEventGroupCreate();
//    while (1) {
//        xEventGroupWaitBits(touch_event, 0x00000001, pdTRUE, pdFALSE, portMAX_DELAY);
//        EXTI->IMR &= ~EXTI_Line4;
//        unsigned char x_pos, y_pos;
//        xpt2046_scan(&x_pos, &y_pos);
//        printf("XPos:%d YPos:%d\r\n", x_pos, y_pos);
//        fflush(stdout);
//        while (!GPIO_ReadInputDataBit(TOUCH_PEN_PORT, TOUCH_PEN_PIN))
//            delayms(20);
//        EXTI_ClearFlag(EXTI_Line4);
//        EXTI->IMR |= EXTI_Line4;
//    }
}

void guiupdate_task(void *parameter) {
//    form_struct_t testform;
//    button_struct_t test_btn;
//    test_btn.x_pos = 10;
//    test_btn.y_pos = 120;
//    test_btn.width = 60;
//    test_btn.height = 30;
//    test_btn.text = "Test";
//    gui_button_init(&test_btn);
//
//    testform.text = "MainWindow";
//    gui_form_init(&testform);
    while (1) {
        gui_show_fusion();
//        gui_show_fix();
//        gui_show_debug();
//        gui_show_gnrmc_information();
//        gui_show_gnrmc_debug();
        delayms(100);
    }
}

void ledblink_task(void *parameter) {
    while (1) {
        LED1_TOGGLE();
        delayms(500);
    }
}

void fusion_task(void *parameters) {
    kalman_config_v(&kalman_v_north);
    kalman_config_v(&kalman_v_east);
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
