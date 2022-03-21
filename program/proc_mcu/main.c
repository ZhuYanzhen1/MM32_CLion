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
struct rt_mailbox touch_point_mailbox;
static unsigned int touch_mailbox_buffer[10];

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
void fusion_task(void *parameters) {
    (void) parameters;
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

void touchscan_task(void *parameters) {
    unsigned char x_pos, y_pos;
    (void) parameters;
    rt_mb_init(&touch_point_mailbox, "touch_m", &touch_mailbox_buffer, 10, RT_IPC_FLAG_FIFO);
    rt_sem_init(&touch_semaphore, "touch_s", 0, RT_IPC_FLAG_FIFO);
    while (1) {
        rt_sem_take(&touch_semaphore, RT_WAITING_FOREVER);
        EXTI->IMR &= ~EXTI_Line4;
        xpt2046_scan(&x_pos, &y_pos);
        rt_mb_send(&touch_point_mailbox, x_pos << 8 | y_pos);
        while (!GPIO_ReadInputDataBit(TOUCH_PEN_PORT, TOUCH_PEN_PIN))
            delayms(20);
        delayms(100);
        EXTI_ClearFlag(EXTI_Line4);
        EXTI->IMR |= EXTI_Line4;
    }
}

void guiupdate_task(void *parameters) {
    unsigned long recv_point = 0;
    (void) parameters;
    while (1) {
        delayms(200);
        if (rt_mb_recv(&touch_point_mailbox, &recv_point, 1) == RT_EOK) {
            printf("x:%d y:%d\r\n", recv_point >> 8, recv_point & 0x00ff);
            fflush(stdout);
        }
    }
}

void ledblink_task(void *parameters) {
    (void) parameters;
    while (1) {
        LED1_TOGGLE();
        delayms(500);
    }
}
