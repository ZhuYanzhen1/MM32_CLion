//
// Created by 16625 on 2022-03-27.
//

#include "orgin_sensor_form.h"
#include "main.h"

extern form_struct_t orgin_form;

static form_struct_t orgin_sensor_form;
static label_struct_t ax_label;
static label_struct_t ay_label;
static label_struct_t az_label;
static label_struct_t mag_x_label;
static label_struct_t mag_y_label;
static label_struct_t mag_z_label;
static button_struct_t sensor_return_btn;

static void gui_show_orgin_sensor(void *parameter) {
    (void) parameter;
    gui_label_settext(&ax_label, " ax:%d   ", debug_data.offset_x);
    gui_label_settext(&ay_label, " ay:%d   ", debug_data.offset_y);
    gui_label_settext(&az_label, " az:%d   ", debug_data.offset_z);
    gui_label_settext(&mag_x_label, " mag_x:%d   ", debug_data.offdiag_x);
    gui_label_settext(&mag_y_label, " mag_y:%d   ", debug_data.offdiag_y);
    gui_label_settext(&mag_z_label, " mag_z:%d   ", debug_data.offdiag_z);

}

static void sensor_form_return_callback(void *parameter) {
    (void) parameter;
    gui_form_display(&orgin_form);
}

void gui_orgin_sensor_form_init() {
    gui_label_init(&ax_label, 1, C_BLACK, label_align_left, " ax");
    gui_label_init(&ay_label, 2, C_BLACK, label_align_left, " ay");
    gui_label_init(&az_label, 3, C_BLACK, label_align_left, " az");
    gui_label_init(&mag_x_label, 4, C_BLACK, label_align_left, " mag_x");
    gui_label_init(&mag_y_label, 5, C_BLACK, label_align_left, " mag_y");
    gui_label_init(&mag_z_label, 6, C_BLACK, label_align_left, " mag_z");

    sensor_return_btn.callback = sensor_form_return_callback;
    gui_button_init(&sensor_return_btn, 14, 114 + 6, 100, 26, "Return");
    gui_form_init(&orgin_sensor_form, "OrginSensor", gui_show_orgin_sensor);

    gui_form_bind_label(&orgin_sensor_form, &ax_label);
    gui_form_bind_label(&orgin_sensor_form, &ay_label);
    gui_form_bind_label(&orgin_sensor_form, &az_label);
    gui_form_bind_label(&orgin_sensor_form, &mag_x_label);
    gui_form_bind_label(&orgin_sensor_form, &mag_y_label);
    gui_form_bind_label(&orgin_sensor_form, &mag_z_label);

    gui_form_bind_button(&orgin_sensor_form, &sensor_return_btn);
    gui_form_display(&orgin_sensor_form);
}
