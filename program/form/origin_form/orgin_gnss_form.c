//
// Created by 16625 on 2022-03-27.
//

#include "main.h"
#include "orgin_gnss_form.h"

extern form_struct_t orgin_form;
static form_struct_t orgin_gnss_form;
static label_struct_t status_form;
static label_struct_t latitude_form;
static label_struct_t longitude_form;
static label_struct_t latitude_direction_form;
static label_struct_t longitude_direction_form;
static label_struct_t speed_to_ground_form;
static label_struct_t direction_truth_form;
static button_struct_t gnss_form_return;

static void gui_show_orgin_gnss(void *parameter) {
    (void) parameter;
    gui_label_settext(&status_form, " status:%c   ", gps_rmc.status);
    gui_label_settext(&latitude_form, " lat:%d   ", gps_rmc.latitude);
    gui_label_settext(&longitude_form, " lon:%d   ", gps_rmc.longitude);
    gui_label_settext(&latitude_direction_form, " lat_dire:%c   ", gps_rmc.latitude_direction);
    gui_label_settext(&longitude_direction_form, " lon_dire:%d   ", gps_rmc.longitude_direction);
    gui_label_settext(&speed_to_ground_form, " speed:%d   ", gps_rmc.speed_to_ground_section);
    gui_label_settext(&direction_truth_form, " turth:%d   ", gps_rmc.direction_of_ground_truth);
}

static void gnss_form_return_callback(void *parameter) {
    (void) parameter;
    gui_form_display(&orgin_form);
}

void gui_orgin_gnss_form_init() {
    gnss_form_return.callback = gnss_form_return_callback;

    gui_label_init(&status_form, 7, C_BLACK, label_align_left, " status");
    gui_label_init(&latitude_form, 1, C_BLACK, label_align_left, " lat");
    gui_label_init(&longitude_form, 2, C_BLACK, label_align_left, " lon");
    gui_label_init(&latitude_direction_form, 3, C_BLACK, label_align_left, " lat_dire");
    gui_label_init(&longitude_direction_form, 4, C_BLACK, label_align_left, " lon_dire");
    gui_label_init(&speed_to_ground_form, 5, C_BLACK, label_align_left, " speed");
    gui_label_init(&direction_truth_form, 6, C_BLACK, label_align_left, " turth");

    gui_form_init(&orgin_gnss_form, "OrginGnss", gui_show_orgin_gnss);
    gui_button_init(&gnss_form_return, 14, 114 + 6, 100, 26, "Return");

    gui_form_bind_label(&orgin_gnss_form, &status_form);
    gui_form_bind_label(&orgin_gnss_form, &latitude_form);
    gui_form_bind_label(&orgin_gnss_form, &longitude_form);
    gui_form_bind_label(&orgin_gnss_form, &latitude_direction_form);
    gui_form_bind_label(&orgin_gnss_form, &longitude_direction_form);
    gui_form_bind_label(&orgin_gnss_form, &speed_to_ground_form);
    gui_form_bind_label(&orgin_gnss_form, &direction_truth_form);

    gui_form_bind_button(&orgin_gnss_form, &gnss_form_return);
    gui_form_display(&orgin_gnss_form);
}