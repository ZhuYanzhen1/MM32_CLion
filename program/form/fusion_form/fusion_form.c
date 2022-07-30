/**************************************************************************/ /**
    \file       fusion_form.c
    \brief      fusion_form function Source File
    \author     LaoZhu
    \version    V1.6.1
    \date       24. July 2022
******************************************************************************/

#include "main.h"
#include "fusion_form.h"

extern form_struct_t main_form;
extern kalman_data_t kalman_data;
extern float statistic_max_speed;
extern float statistic_max_current;

static form_struct_t fusion_form;
static label_struct_t kalman_north_label;
static label_struct_t gnss_status_label;
static label_struct_t kalman_v_lable;
static label_struct_t kalman_north_distance_lable;
static label_struct_t kalman_east_distance_lable;
static label_struct_t max_speed_lable;
static label_struct_t max_battery_current_lable;
static button_struct_t fusion_form_return_btn;

void fusion_form_return_btn_callback(void *parameter) {
    (void) parameter;
    gui_form_display(&main_form);
}

void gui_show_fusion(void *parameter) {
    (void) parameter;
    gui_label_settext(&kalman_north_label, " k_north:%.2f", small_packets.north);
    gui_label_settext(&gnss_status_label, " status:%c", gps_rmc.status);
    gui_label_settext(&kalman_north_distance_lable, " k_nd:%.3f", kalman_data.distance_north);
    gui_label_settext(&kalman_east_distance_lable, " k_ed:%.3f", kalman_data.distance_east);
    gui_label_settext(&kalman_v_lable, " k_v:%f", kalman_data.v);
    gui_label_settext(&max_speed_lable, " max_v:%.3f", statistic_max_speed);
    gui_label_settext(&max_battery_current_lable, " max_i:%.1f", statistic_max_current);
}

void gui_fusion_form_init() {

    gui_label_init(&kalman_north_label, 1, C_BLACK, label_align_left, " k_north");
    gui_label_init(&gnss_status_label, 2, C_BLACK, label_align_left, " status");
    gui_label_init(&kalman_v_lable, 3, C_BLACK, label_align_left, " k_v");
    gui_label_init(&kalman_north_distance_lable, 4, C_BLACK, label_align_left, " k_nd");
    gui_label_init(&kalman_east_distance_lable, 5, C_BLACK, label_align_left, " k_ed");
    gui_label_init(&max_speed_lable, 6, C_BLACK, label_align_left, " max_v");
    gui_label_init(&max_battery_current_lable, 7, C_BLACK, label_align_left, " max_i");

    fusion_form_return_btn.callback = fusion_form_return_btn_callback;
    gui_button_init(&fusion_form_return_btn, 68, 129, 50, 24, "Back");

    gui_form_init(&fusion_form, "FusionForm", gui_show_fusion);

    gui_form_bind_label(&fusion_form, &kalman_north_label);
    gui_form_bind_label(&fusion_form, &gnss_status_label);
    gui_form_bind_label(&fusion_form, &kalman_v_lable);
    gui_form_bind_label(&fusion_form, &kalman_north_distance_lable);
    gui_form_bind_label(&fusion_form, &kalman_east_distance_lable);
    gui_form_bind_label(&fusion_form, &max_speed_lable);
    gui_form_bind_label(&fusion_form, &max_battery_current_lable);
    gui_form_bind_button(&fusion_form, &fusion_form_return_btn);
    gui_form_display(&fusion_form);
}
