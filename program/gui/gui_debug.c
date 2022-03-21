//
// Created by 16625 on 2022-03-20.
//

#include "main.h"

extern unsigned int usart3_dma_buffer_1[74];
extern unsigned int usart3_dma_buffer_2[74];
extern kalman_data_t kalman_data;

static form_struct_t fusion_form;
static label_struct_t kalman_north_label;
static label_struct_t gnss_status_label;
static label_struct_t north_distance_label;
static label_struct_t east_distance_label;
static label_struct_t kalman_v_lable;
static label_struct_t neu_v_lable;
static label_struct_t kalman_north_distance_lable;
static label_struct_t kalman_east_distance_lable;

void gui_init_fusion() {
    gui_label_init(&kalman_north_label, 0, C_BLACK, label_align_left, " k_north");
    gui_label_init(&gnss_status_label, 1, C_BLACK, label_align_left, " status");
    gui_label_init(&north_distance_label, 2, C_BLACK, label_align_left, " nd");
    gui_label_init(&east_distance_label, 3, C_BLACK, label_align_left, " ed");
    gui_label_init(&kalman_v_lable, 4, C_BLACK, label_align_left, " k_v");
    gui_label_init(&neu_v_lable, 5, C_BLACK, label_align_left, " v");
    gui_label_init(&kalman_north_distance_lable, 6, C_BLACK, label_align_left, " k_nd");
    gui_label_init(&kalman_east_distance_lable, 7, C_BLACK, label_align_left, " k_ed");

    gui_form_init(&fusion_form, "FusionWindow");

    gui_form_bind_label(&fusion_form, &kalman_north_label);
    gui_form_bind_label(&fusion_form, &gnss_status_label);
    gui_form_bind_label(&fusion_form, &north_distance_label);
    gui_form_bind_label(&fusion_form, &east_distance_label);
    gui_form_bind_label(&fusion_form, &kalman_v_lable);
    gui_form_bind_label(&fusion_form, &neu_v_lable);
    gui_form_bind_label(&fusion_form, &kalman_north_distance_lable);
    gui_form_bind_label(&fusion_form, &kalman_east_distance_lable);
    gui_form_display(&fusion_form);
}

void gui_show_fusion() {
    gui_label_settext(&kalman_north_label, " k_north:%f   ", small_packets.kalman_north);
    gui_label_settext(&gnss_status_label, " status:%c ", gps_rmc.status);
    gui_label_settext(&north_distance_label, " nd:%f  ", neu.north_distance);
    gui_label_settext(&east_distance_label, " ed:%f  ", neu.east_distance);
    gui_label_settext(&kalman_v_lable, " k_v:%f  ", kalman_data.v);
    gui_label_settext(&neu_v_lable, " v:%f  ", neu.v);
    gui_label_settext(&kalman_north_distance_lable, " k_nd:%f  ", kalman_data.distance_north);
    gui_label_settext(&kalman_east_distance_lable, " k_ed:%f  ", kalman_data.distance_east);
    gui_form_update(&fusion_form);
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
