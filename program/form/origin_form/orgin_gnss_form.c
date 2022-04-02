//
// Created by 16625 on 2022-03-27.
//

#include "main.h"
#include "orgin_gnss_form.h"

extern form_struct_t orgin_form;

static form_struct_t orgin_gnss_form;
static label_struct_t status_label;
static label_struct_t latitude_label;
static label_struct_t longitude_label;
static label_struct_t latitude_direction_label;
static label_struct_t longitude_direction_label;
static label_struct_t speed_to_ground_label;
static label_struct_t direction_truth_label;
static button_struct_t gnss_return_btn;

static void gui_show_orgin_gnss(void *parameter) {
    (void) parameter;
    gui_label_settext(&status_label, " status:%c   ", gps_rmc.status);
    gui_label_settext(&latitude_label, " lat:%d   ", gps_rmc.latitude);
    gui_label_settext(&longitude_label, " lon:%d   ", gps_rmc.longitude);
    gui_label_settext(&latitude_direction_label, " lat_dire:%c   ", gps_rmc.latitude_direction);
    gui_label_settext(&longitude_direction_label, " lon_dire:%c   ", gps_rmc.longitude_direction);
    gui_label_settext(&speed_to_ground_label, " speed:%d   ", gps_rmc.speed_to_ground_section);
    gui_label_settext(&direction_truth_label, " turth:%d   ", gps_rmc.direction_of_ground_truth);
}

static void gnss_form_return_callback(void *parameter) {
    (void) parameter;
    gui_form_display(&orgin_form);
}

void gui_orgin_gnss_form_init() {
    gui_label_init(&status_label, 7, C_BLACK, label_align_left, " status");
    gui_label_init(&latitude_label, 1, C_BLACK, label_align_left, " lat");
    gui_label_init(&longitude_label, 2, C_BLACK, label_align_left, " lon");
    gui_label_init(&latitude_direction_label, 3, C_BLACK, label_align_left, " lat_dire");
    gui_label_init(&longitude_direction_label, 4, C_BLACK, label_align_left, " lon_dire");
    gui_label_init(&speed_to_ground_label, 5, C_BLACK, label_align_left, " speed");
    gui_label_init(&direction_truth_label, 6, C_BLACK, label_align_left, " turth");

    gnss_return_btn.callback = gnss_form_return_callback;
    gui_button_init(&gnss_return_btn, 14, 114 + 6, 100, 26, "Return");

    gui_form_init(&orgin_gnss_form, "OrginGnss", gui_show_orgin_gnss);

    gui_form_bind_label(&orgin_gnss_form, &status_label);
    gui_form_bind_label(&orgin_gnss_form, &latitude_label);
    gui_form_bind_label(&orgin_gnss_form, &longitude_label);
    gui_form_bind_label(&orgin_gnss_form, &latitude_direction_label);
    gui_form_bind_label(&orgin_gnss_form, &longitude_direction_label);
    gui_form_bind_label(&orgin_gnss_form, &speed_to_ground_label);
    gui_form_bind_label(&orgin_gnss_form, &direction_truth_label);

    gui_form_bind_button(&orgin_gnss_form, &gnss_return_btn);
    gui_form_display(&orgin_gnss_form);
}