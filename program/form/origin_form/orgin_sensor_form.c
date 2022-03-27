//
// Created by 16625 on 2022-03-27.
//

#include "orgin_sensor_form.h"
#include "main.h"

extern form_struct_t orgin_form;
static form_struct_t orgin_sensor_form;
static label_struct_t ax_form;
static label_struct_t ay_form;
static label_struct_t az_form;
static label_struct_t mag_x_form;
static label_struct_t mag_y_form;
static label_struct_t mag_z_form;
static button_struct_t sensor_form_return;

static void gui_show_orgin_sensor(void *parameter) {
    (void) parameter;
    gui_label_settext(&ax_form, " ax:%d   ", debug_data.offset_x);
    gui_label_settext(&ay_form, " ay:%d   ", debug_data.offset_y);
    gui_label_settext(&az_form, " az:%d   ", debug_data.offset_z);
    gui_label_settext(&mag_x_form, " mag_x:%d   ", debug_data.offdiag_x);
    gui_label_settext(&mag_y_form, " mag_y:%d   ", debug_data.offdiag_y);
    gui_label_settext(&mag_z_form, " mag_z:%d   ", debug_data.offdiag_z);

}

static void sensor_form_return_callback(void *parameter) {
    (void) parameter;
    gui_form_display(&orgin_form);
}

void gui_orgin_sensor_form_init() {
    sensor_form_return.callback = sensor_form_return_callback;

    gui_label_init(&ax_form, 1, C_BLACK, label_align_left, " ax");
    gui_label_init(&ay_form, 2, C_BLACK, label_align_left, " ay");
    gui_label_init(&az_form, 3, C_BLACK, label_align_left, " az");
    gui_label_init(&mag_x_form, 4, C_BLACK, label_align_left, " mag_x");
    gui_label_init(&mag_y_form, 5, C_BLACK, label_align_left, " mag_y");
    gui_label_init(&mag_z_form, 6, C_BLACK, label_align_left, " mag_z");

    gui_form_init(&orgin_sensor_form, "OrginSensor", gui_show_orgin_sensor);
    gui_button_init(&sensor_form_return, 14, 114 + 6, 100, 26, "Return");

    gui_form_bind_label(&orgin_sensor_form, &ax_form);
    gui_form_bind_label(&orgin_sensor_form, &ay_form);
    gui_form_bind_label(&orgin_sensor_form, &az_form);
    gui_form_bind_label(&orgin_sensor_form, &mag_x_form);
    gui_form_bind_label(&orgin_sensor_form, &mag_y_form);
    gui_form_bind_label(&orgin_sensor_form, &mag_z_form);

    gui_form_bind_button(&orgin_sensor_form, &sensor_form_return);
    gui_form_display(&orgin_sensor_form);
}
