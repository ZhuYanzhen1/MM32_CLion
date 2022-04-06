//
// Created by 16625 on 2022-03-20.
//

#include "main.h"
#include "fusion_form.h"

extern form_struct_t main_form;
extern kalman_data_t kalman_data;

static form_struct_t fusion_form;
static label_struct_t kalman_north_label;
static label_struct_t gnss_status_label;
static label_struct_t kalman_v_lable;
static label_struct_t kalman_north_distance_lable;
static label_struct_t kalman_east_distance_lable;
static button_struct_t fusion_form_return_btn;

void fusion_form_return_btn_callback(void *parameter) {
    (void) parameter;
    gui_form_display(&main_form);
}

void gui_show_fusion(void *parameter) {
    (void) parameter;
    gui_label_settext(&kalman_north_label, " k_north:%.2f", small_packets.chebyshev_north);
    gui_label_settext(&gnss_status_label, " status:%c", gps_rmc.status);
    gui_label_settext(&kalman_north_distance_lable, " k_nd:%.3f", kalman_data.distance_north);
    gui_label_settext(&kalman_east_distance_lable, " k_ed:%.3f", kalman_data.distance_east);
    gui_label_settext(&kalman_v_lable, " k_v:%f", kalman_data.v);
}

void gui_fusion_form_init() {

    gui_label_init(&kalman_north_label, 1, C_BLACK, label_align_left, " k_north");
    gui_label_init(&gnss_status_label, 2, C_BLACK, label_align_left, " status");
    gui_label_init(&kalman_v_lable, 3, C_BLACK, label_align_left, " k_v");
    gui_label_init(&kalman_north_distance_lable, 4, C_BLACK, label_align_left, " k_nd");
    gui_label_init(&kalman_east_distance_lable, 5, C_BLACK, label_align_left, " k_ed");

    fusion_form_return_btn.callback = fusion_form_return_btn_callback;
    gui_button_init(&fusion_form_return_btn, 68, 129, 50, 24, "Back");

    gui_form_init(&fusion_form, "FusionForm", gui_show_fusion);

    gui_form_bind_label(&fusion_form, &kalman_north_label);
    gui_form_bind_label(&fusion_form, &gnss_status_label);
    gui_form_bind_label(&fusion_form, &kalman_v_lable);
    gui_form_bind_label(&fusion_form, &kalman_north_distance_lable);
    gui_form_bind_label(&fusion_form, &kalman_east_distance_lable);
    gui_form_bind_button(&fusion_form, &fusion_form_return_btn);
    gui_form_display(&fusion_form);
}