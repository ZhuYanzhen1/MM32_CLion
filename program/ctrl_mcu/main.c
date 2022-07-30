/**************************************************************************/ /**
    \file     main.c
    \brief    main function Source File
    \author   Lao·Zhu & ZGL
    \version  V1.6.1
    \date     17. July 2022
 ******************************************************************************/

#include "main.h"

/*! \brief FAT file system structure */
FATFS filesystem;

/*! \brief File system buffer arrays */
static unsigned char fs_buffer[FF_MAX_SS * 4];

/*! \brief Determining whether to perform LQR control */
extern volatile unsigned char lqr_flag;

/*! \brief Motor expect speed */
volatile unsigned short speed = 0;

/*! \brief The nearest reference point to the vehicle */
static volatile unsigned short playground_ind = 0;

/*! \brief Information received from proc_mcu */
extern unsigned int uart6_dma_buffer_1[CTRL_MCU_RECEIVE_AMOUNT];
extern unsigned int uart6_dma_buffer_2[CTRL_MCU_RECEIVE_AMOUNT];

/*! \brief Temperature of the motor */
extern unsigned char temperature;

/*! \brief Messages sent to the brushless driver board */
unsigned int uart7_dma_send_buffer[UART7_DMA_SEND_BUFFER] = {0};

int main(void) {
    /* Initialize systick system clock for time delay */
    delay_config();

    /* Initialize the PWM GPIO for timer3 */
    tim3_pwm_gpio_config();

    /* Initialize other peripherals for communication */
    led_config();
    uart1_config();
    uart6_config();
    uart6_dma_nvic_config();
    uart6_dma_receive_config(uart6_dma_buffer_1, CTRL_MCU_RECEIVE_AMOUNT);
    uart6_dma_set_transmit_buffer(uart6_dma_buffer_1, CTRL_MCU_RECEIVE_AMOUNT);
    uart7_config();
    uart7_dma_sent_config(uart7_dma_send_buffer, UART7_DMA_SEND_BUFFER);

    /* Delay to wait for the radio transmission module to be ready */
    delayms(1000);

    /* Initialize debugger-related structures and print debugging information */
    cm_backtrace_config("CtrlMCU", "1.6.1", "1.6.1");
    debugger_register_variable(dbg_uint32, &global_time_stamp, "time");
    printf("CtrlMCU %s %s %s\r\n", __DATE__, __TIME__, GIT_HASH);
    timer2_config();

    /* Initialize Timer 3 and SPI3 and start PWM output */
    spi3_config();
    delayms(2000);
    timer3_config();

    /* Switch SD card read/write interface to MCU */
    sdcard_switch_device(0);

    /* Try to mount the SD card to the file system */
    FRESULT result = f_mount(&filesystem, "0:", 1);
    if (result == FR_NO_FILESYSTEM) {
        printf("Making FileSystem...\r\n");
        if (f_mkfs("0:", 0, fs_buffer, sizeof(fs_buffer)) == FR_OK) {
            f_setlabel((const TCHAR *) "0:SD");
            printf("Make FileSystem Success\r\n");
        } else {
            printf("Error To Make FileSystem\r\n");
            while (1);
        }
    } else if (result != FR_OK) {
        printf("Error To Mount FileSystem\r\n");
        while (1);
    }
    printf("FileSystem Mount Success!\r\n");

    /* Write the current firmware information and scan files in SD card*/
    fs_write_current_info();
    fs_scan_files("0:");

    /* Define the variables needed to find the closest point at departure */
    static unsigned char find_counter = 0;
    static unsigned short start_point = 0;
    volatile static unsigned short index = 0;

    /* Find the closest reference point to the car at the time of departure */
    while (1) {
        LED1_TOGGLE();
        delayms(500);

        /* Transferring motor temperature information to ProcMCU */
        uart6_sendbyte(temperature);

        /* Determine whether the positioning is stable */
        if (proc_data.distance_east != 0 && lqr_flag == 1) {
            lqr_flag = 0;

            /* Find the nearest reference point to the current point */
            playground_ind = find_index(playground_ind);
            start_point = playground_ind;
            find_counter++;
            if (find_counter > 3)
                break;
            LED1_TOGGLE();
        }
    }

    /* Start control, start the car */
    while (1) {
        /* Ensure that the current positioning information is valid */
        if (proc_data.distance_east != 0) {
            /* 10 cycles of servo control with a 400ms cycle */
            for (unsigned char i = 0; i < 10; i++) {
                while (1) {
                    if (lqr_flag == 1) {
                        lqr_flag = 0;
                        LED1_TOGGLE();

                        /* Load the current information into the structure */
                        basic_status_t current_status = {proc_data.distance_north,
                                                         proc_data.distance_east,
                                                         proc_data.north_angle};

                        /* Find the nearest reference point to the current point */
                        playground_ind = find_index(playground_ind);

                        /* Override the reference point and feed it to the LQR controller to calculate the servo angle */
                        index = playground_ind + OVERRUN_POINT;
                        unsigned char angle = lqr_control(index, current_status);

                        /* Write the servo angle control amount to the comparator */
                        WRITE_REG(TIM3->CCR1, angle);
                        break;
                    }
                }
            }
            /* Set the motor speed, and set the speed to 0 if the end point is reached */
            speed = 23000;
            if (playground_ind > INDEX_NUM - 10)
                speed = 0;

            /* Sending motor speed to the motor driver */
            sdtp_data_transmit_speed(speed, uart7_dma_send_buffer);
            uart7_dma_set_send_buffer(uart7_dma_send_buffer, UART7_DMA_SEND_BUFFER);

            /* Transferring motor temperature information to ProcMCU */
            uart6_sendbyte(temperature);
        }
    }
}
