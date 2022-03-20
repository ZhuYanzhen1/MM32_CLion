/**************************************************************************/ /**
    \file       gps.c
    \brief      Contains gps initialization and
                display of received gps related information on the screen
    \author     ZGL
    \version    V1.3.2
    \date       06. March 2021
******************************************************************************/

#include "gps_parser.h"
#include "gps.h"
#include "uart.h"
#include "delay.h"
#include "gui_base.h"
#include "dma.h"
#include "hal_conf.h"

#define UART3_CONFIG_GPS(cmdbuf)            for (unsigned char i = 0; i < (unsigned char) sizeof(cmdbuf); i++)\
                                                uart3_sendbyte((cmdbuf)[i]);\
                                            delayms(100);
#define CLOSE_PACKAGE_OUTPUT(a, b, c, d)    output_rmc[7]=a;\
                                            output_rmc[8]=b;\
                                            output_rmc[9]=c;\
                                            output_rmc[10]=d;\
                                            UART3_CONFIG_GPS(output_rmc)

extern unsigned int usart3_dma_buffer_1[74];
extern unsigned int usart3_dma_buffer_2[74];

/*!
    \brief  Initialization of gps-related content
            1：Baud rate 115200
            2：10Hz output
            3：Output RMC packages only
            4：Using GPS and BeiDou
            5：Use NMEA version 4.1 compatible protocol
            6：uart3_dma_receive_config
            7：dma_nvic_config
            8：DMA_Cmd
            TODO 没加冷启动热启动的相关初始化代码
*/
void gps_config() {
    char output_frequency_10Hz[] = {0xF1, 0xD9, 0x06, 0x42, 0x14, 0x00, 0x00, 0x0A,
                                    0x38, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x02, 0x24};
    char output_rmc[] = {0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0,
                         0x05, 0x01, 0x00, 0x1A};
    uart3_dma_nvic_config();
    uart3_dma_receive_config(usart3_dma_buffer_1, 74);
    DMA_Cmd(DMA1_Channel3, DISABLE);
    delayms(10);
    UART3_CONFIG_GPS(output_frequency_10Hz)
    UART3_CONFIG_GPS(output_rmc)
    CLOSE_PACKAGE_OUTPUT(0x00, 0x00, 0xFA, 0x0F)
    CLOSE_PACKAGE_OUTPUT(0x01, 0x00, 0xFB, 0x11)
    CLOSE_PACKAGE_OUTPUT(0x02, 0x00, 0xFC, 0x13)
    CLOSE_PACKAGE_OUTPUT(0x04, 0x00, 0xFE, 0x17)
    CLOSE_PACKAGE_OUTPUT(0x06, 0x00, 0x00, 0x1B)
    CLOSE_PACKAGE_OUTPUT(0x07, 0x00, 0x01, 0x1D)
    CLOSE_PACKAGE_OUTPUT(0x08, 0x00, 0x02, 0x1F)
    CLOSE_PACKAGE_OUTPUT(0x20, 0x00, 0x1A, 0x4F)
    DMA_Cmd(DMA1_Channel3, ENABLE);
}

/*!
    \brief      Test code with a display showing the processed gnrmc package
    \param[in]  gps_rmc: Recommended minimum positioning information
                time, status, latitude, longitude, speed, direction, positioning mode
*/
void gui_show_gnrmc_information() {
    gui_printf(0, 0 * 12, C_BLACK, C_WHITE,
               "time:%d", gps_rmc.positioning_time.uct_time);
    gui_printf(0, 1 * 12, C_BLACK, C_WHITE,
               "status:%c", gps_rmc.status);
    gui_printf(0, 2 * 12, C_BLACK, C_WHITE,
               "latitude:%d", gps_rmc.latitude);
    gui_printf(0, 3 * 12, C_BLACK, C_WHITE,
               "latitude_direction:%c", gps_rmc.latitude_direction);
    gui_printf(0, 4 * 12, C_BLACK, C_WHITE,
               "longitude:%d", gps_rmc.longitude);
    gui_printf(0, 5 * 12, C_BLACK, C_WHITE,
               "longitude_direction:%c", gps_rmc.longitude_direction);
    gui_printf(0, 6 * 12, C_BLACK, C_WHITE,
               "speed:%d", gps_rmc.speed_to_ground_section);
    gui_printf(0, 7 * 12, C_BLACK, C_WHITE,
               "direction:%d", gps_rmc.direction_of_ground_truth);
    gui_printf(0, 8 * 12, C_BLACK, C_WHITE,
               "positioning_mode:%c", gps_rmc.mode);
}

void show_gnrmc_debug() {
    for (int j = 0; j < 3; ++j)
        for (int i = 0; i < 21; ++i)
            gui_putchar(i * 6, j * 12,
                        (usart3_dma_buffer_1[i + 21 * j] == '\r' || usart3_dma_buffer_1[i + 21 * j] == '\n') ?
                        ' ' : usart3_dma_buffer_1[i + 21 * j], C_BLACK, C_WHITE);
    for (int i = 0; i < 11; ++i)
        gui_putchar(i * 6, 36, (usart3_dma_buffer_1[i + 63] == '\r' || usart3_dma_buffer_1[i + 63] == '\n') ?
                               ' ' : usart3_dma_buffer_1[i + 63], C_BLACK, C_WHITE);

}

