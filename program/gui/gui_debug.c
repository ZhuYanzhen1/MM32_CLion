//
// Created by 16625 on 2022-03-20.
//

#include "gui_debug.h"
#include "gui_base.h"
#include "sensor_decode.h"
#include "gps_parser.h"
#include "data_conversion.h"

extern unsigned int usart3_dma_buffer_1[74];
extern unsigned int usart3_dma_buffer_2[74];

extern float v_north_final;
extern float v_east_final;
extern float distance_north;
extern float distance_east;

void gui_show_fusion() {
    gui_printf(0, 0 * 12, C_BLACK, C_WHITE,
               "k_north:%f   ", small_packets.kalman_north);
    gui_printf(0, 1 * 12, C_BLACK, C_WHITE,
               "north:%d   ", small_packets.north);
    gui_printf(0, 2 * 12, C_BLACK, C_WHITE,
               "k_vn:%f   ", v_north_final);
    gui_printf(0, 3 * 12, C_BLACK, C_WHITE,
               "k_ve:%f   ", v_east_final);
    gui_printf(0, 4 * 12, C_BLACK, C_WHITE,
               "k_dn:%f   ", distance_north);
    gui_printf(0, 5 * 12, C_BLACK, C_WHITE,
               "k_de:%f   ", distance_east);
    gui_printf(0, 6 * 12, C_BLACK, C_WHITE,
               "status:%c", gps_rmc.status);
    gui_printf(0, 7 * 12, C_BLACK, C_WHITE,
               "nd:%f", neu.north_distance);
    gui_printf(0, 8 * 12, C_BLACK, C_WHITE,
               "ed:%f", neu.east_distance);
    gui_printf(0, 9 * 12, C_BLACK, C_WHITE,
               "nv:%f", neu.north_v);
    gui_printf(0, 10 * 12, C_BLACK, C_WHITE,
               "ev:%f", neu.east_v);
}

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

void gui_show_gnrmc_debug() {
    for (int j = 0; j < 3; ++j)
        for (int i = 0; i < 21; ++i)
            gui_putchar(i * 6, j * 12,
                        (usart3_dma_buffer_1[i + 21 * j] == '\r' || usart3_dma_buffer_1[i + 21 * j] == '\n') ?
                        ' ' : usart3_dma_buffer_1[i + 21 * j], C_BLACK, C_WHITE);
    for (int i = 0; i < 11; ++i)
        gui_putchar(i * 6, 36, (usart3_dma_buffer_1[i + 63] == '\r' || usart3_dma_buffer_1[i + 63] == '\n') ?
                               ' ' : usart3_dma_buffer_1[i + 63], C_BLACK, C_WHITE);

}
