//
// Created by 16625 on 2022-01-16.
//

#include "gps.h"
#include "uart.h"
#include "delay.h"
#include "gui_base.h"
#include "gps_parser.h"
#include "dma.h"
#include "hal_conf.h"
#include "config.h"

#define UART6_CONFIG_GPS(cmdbuf)    for (unsigned char i = 0; i < (unsigned char) sizeof(cmdbuf); i++)\
                                        uart6_sendbyte((cmdbuf)[i]);\
                                    delayms(100)

//
//TODO 没加冷启动热启动的相关初始化代码
//

/*!
    \brief  Initialization of gps-related content
            1：Baud rate 115200
            2：10Hz output
            3：Output RMC packages only
            4：Using GPS and BeiDou
            5：Use NMEA version 4.1 compatible protocol
            6：uart6_dma_receive_config
            7：dma_nvic_config
            8：DMA_Cmd
*/
extern unsigned int usart6_dma_buffer_1[74];
extern unsigned int usart6_dma_buffer_2[74];
void gps_config() {
    unsigned int apbclock = RCC_GetPCLK1Freq();
    uart6_dma_nvic_config();
    uart6_dma_receive_config(usart6_dma_buffer_1, 74);
    DMA_Cmd(DMA1_Channel1, DISABLE);

    UART6_CONFIG_GPS("$PCAS01,5*19\r\n");
    UART6->BRR = (apbclock / 115200) / 16;
    UART6->FRA = (apbclock / 115200) % 16;
    delayms(10);
    UART6_CONFIG_GPS("$PCAS04,3*1A\r\n");
    UART6_CONFIG_GPS("$PCAS05,2*1A\r\n");
    UART6_CONFIG_GPS("$PCAS03,0,0,0,0,1,0,0,0,0,0,,,0,0*03\r\n");
    UART6_CONFIG_GPS("$PCAS02,100*1E\r\n");

    DMA_Cmd(DMA1_Channel1, ENABLE);
}

/*!
    \brief      Test code with a display showing the processed gnrmc package
    \param[in]  gps_rmc: Recommended minimum positioning information
                time, status, latitude, longitude, speed, direction, positioning mode
*/
void gui_show_gnrmc_information(nmea_rmc *gps_rmc) {
    gui_printf(0, 0 * 12, C_BLACK, C_WHITE,
               "time:%d", gps_rmc->positioning_time.uct_time);
    gui_printf(0, 1 * 12, C_BLACK, C_WHITE,
               "status:%c", gps_rmc->status);
    gui_printf(0, 2 * 12, C_BLACK, C_WHITE,
               "latitude:%d", gps_rmc->latitude);
    gui_printf(0, 3 * 12, C_BLACK, C_WHITE,
               "latitude_direction:%c", gps_rmc->latitude_direction);
    gui_printf(0, 4 * 12, C_BLACK, C_WHITE,
               "longitude:%d", gps_rmc->longitude);
    gui_printf(0, 5 * 12, C_BLACK, C_WHITE,
               "longitude_direction:%c", gps_rmc->longitude_direction);
    gui_printf(0, 6 * 12, C_BLACK, C_WHITE,
               "status:%c", gps_rmc->status);
    gui_printf(0, 7 * 12, C_BLACK, C_WHITE,
               "speed:%d", gps_rmc->speed_to_ground_section);
    gui_printf(0, 8 * 12, C_BLACK, C_WHITE,
               "direction:%d", gps_rmc->direction_of_ground_truth);
    gui_printf(0, 9 * 12, C_BLACK, C_WHITE,
               "positioning_mode:%c", gps_rmc->mode);
}

