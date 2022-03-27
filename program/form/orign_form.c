//
// Created by 16625 on 2022-03-27.
//
#include "main.h"
#include "orign_form.h"

static form_struct_t orgin_gnss_form;
static label_struct_t status_form;
static label_struct_t latitude_form;
static label_struct_t longitude_form;
static label_struct_t latitude_direction_form;
static label_struct_t longitude_direction_form;
static label_struct_t speed_to_ground_form;
static label_struct_t direction_truth_form;

static form_struct_t orgin_sensor_form;
static label_struct_t ax_form;
static label_struct_t ay_form;
static label_struct_t az_form;
static label_struct_t mag_x_form;
static label_struct_t mag_y_form;
static label_struct_t mag_z_form;

void gui_show_orgin_gnss(void *parameter) {
    (void) parameter;
    gui_label_settext(&status_form, " status:%c   ", gps_rmc.status);
    gui_label_settext(&latitude_form, " lat:%d   ", gps_rmc.latitude);
    gui_label_settext(&longitude_form, " lon:%d   ", gps_rmc.longitude);
    gui_label_settext(&latitude_direction_form, " lat_dire:%c   ", gps_rmc.latitude_direction);
    gui_label_settext(&longitude_direction_form, " lon_dire:%d   ", gps_rmc.longitude_direction);
    gui_label_settext(&speed_to_ground_form, " speed:%d   ", gps_rmc.speed_to_ground_section);
    gui_label_settext(&direction_truth_form, " turth:%d   ", gps_rmc.direction_of_ground_truth);
}

void gui_orgin_gnss_form_init() {
    gui_label_init(&status_form, 7, C_BLACK, label_align_left, " status");
    gui_label_init(&latitude_form, 1, C_BLACK, label_align_left, " lat");
    gui_label_init(&longitude_form, 2, C_BLACK, label_align_left, " lon");
    gui_label_init(&latitude_direction_form, 3, C_BLACK, label_align_left, " lat_dire");
    gui_label_init(&longitude_direction_form, 4, C_BLACK, label_align_left, " lon_dire");
    gui_label_init(&speed_to_ground_form, 5, C_BLACK, label_align_left, " speed");
    gui_label_init(&direction_truth_form, 6, C_BLACK, label_align_left, " turth");

    gui_form_init(&orgin_gnss_form, "OrginGnss", gui_show_orgin_gnss);

    gui_form_bind_label(&orgin_gnss_form, &status_form);
    gui_form_bind_label(&orgin_gnss_form, &latitude_form);
    gui_form_bind_label(&orgin_gnss_form, &longitude_form);
    gui_form_bind_label(&orgin_gnss_form, &latitude_direction_form);
    gui_form_bind_label(&orgin_gnss_form, &longitude_direction_form);
    gui_form_bind_label(&orgin_gnss_form, &speed_to_ground_form);
    gui_form_bind_label(&orgin_gnss_form, &direction_truth_form);

    gui_form_display(&orgin_gnss_form);
}

void gui_show_orgin_sensor(void *parameter) {
    (void) parameter;
    gui_label_settext(&ax_form, " ax:%d   ", debug_data.offset_x);
    gui_label_settext(&ay_form, " ay:%d   ", debug_data.offset_y);
    gui_label_settext(&az_form, " az:%d   ", debug_data.offset_z);
    gui_label_settext(&mag_x_form, " mag_x:%d   ", debug_data.offdiag_x);
    gui_label_settext(&mag_y_form, " mag_y:%d   ", debug_data.offdiag_y);
    gui_label_settext(&mag_z_form, " mag_z:%d   ", debug_data.offdiag_z);

}

void gui_orgin_sensor_form_init() {
    gui_label_init(&ax_form, 1, C_BLACK, label_align_left, " ax");
    gui_label_init(&ay_form, 2, C_BLACK, label_align_left, " ay");
    gui_label_init(&az_form, 3, C_BLACK, label_align_left, " az");
    gui_label_init(&mag_x_form, 7, C_BLACK, label_align_left, " mag_x");
    gui_label_init(&mag_y_form, 8, C_BLACK, label_align_left, " mag_y");
    gui_label_init(&mag_z_form, 9, C_BLACK, label_align_left, " mag_z");

    gui_form_init(&orgin_sensor_form, "OrginSensor", gui_show_orgin_sensor);

    gui_form_bind_label(&orgin_sensor_form, &ax_form);
    gui_form_bind_label(&orgin_sensor_form, &ay_form);
    gui_form_bind_label(&orgin_sensor_form, &az_form);
    gui_form_bind_label(&orgin_sensor_form, &mag_x_form);
    gui_form_bind_label(&orgin_sensor_form, &mag_y_form);
    gui_form_bind_label(&orgin_sensor_form, &mag_z_form);

    gui_form_display(&orgin_sensor_form);
}
