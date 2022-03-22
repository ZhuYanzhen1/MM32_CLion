//
// Created by 16625 on 2022-03-20.
//

#include "main.h"
#include "fusion_form.h"

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

void gui_show_fusion(void *parameter) {
    (void) parameter;
    gui_label_settext(&kalman_north_label, " k_north:%f   ", small_packets.kalman_north);
    gui_label_settext(&gnss_status_label, " status:%c ", gps_rmc.status);
    gui_label_settext(&north_distance_label, " nd:%f  ", neu.north_distance);
    gui_label_settext(&east_distance_label, " ed:%f  ", neu.east_distance);
    gui_label_settext(&kalman_v_lable, " k_v:%f  ", kalman_data.v);
    gui_label_settext(&neu_v_lable, " v:%f  ", neu.v);
    gui_label_settext(&kalman_north_distance_lable, " k_nd:%f  ", kalman_data.distance_north);
    gui_label_settext(&kalman_east_distance_lable, " k_ed:%f  ", kalman_data.distance_east);
}

void gui_fusion_form_init() {
    gui_label_init(&kalman_north_label, 0, C_BLACK, label_align_left, " k_north");
    gui_label_init(&gnss_status_label, 1, C_BLACK, label_align_left, " status");
    gui_label_init(&north_distance_label, 2, C_BLACK, label_align_left, " nd");
    gui_label_init(&east_distance_label, 3, C_BLACK, label_align_left, " ed");
    gui_label_init(&kalman_v_lable, 4, C_BLACK, label_align_left, " k_v");
    gui_label_init(&neu_v_lable, 5, C_BLACK, label_align_left, " v");
    gui_label_init(&kalman_north_distance_lable, 6, C_BLACK, label_align_left, " k_nd");
    gui_label_init(&kalman_east_distance_lable, 7, C_BLACK, label_align_left, " k_ed");

    gui_form_init(&fusion_form, "FusionForm", gui_show_fusion);

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
