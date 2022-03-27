//
// Created by LaoZhu on 2022/3/27.
//

#include "main.h"
#include "calibrate_form.h"
#include "calibrate_touch_form.h"

extern form_struct_t main_form;
form_struct_t calibrate_form;
static button_struct_t calibrate_mag_form_btn;
static button_struct_t calibrate_touch_form_btn;
static button_struct_t calibrate_form_return_btn;

void calibrate_mag_form_btn_callback(void *parameter) {
    (void) parameter;
}

void calibrate_touch_form_btn_callback(void *parameter) {
    (void) parameter;
    gui_calibrate_touch_form_init();
}

void calibrate_form_return_btn_callback(void *parameter) {
    (void) parameter;
    gui_form_display(&main_form);
}

void gui_calibrate_form_update(void *parameter) {
    (void) parameter;
}

void gui_calibrate_form_init(void) {
    calibrate_mag_form_btn.callback = calibrate_mag_form_btn_callback;
    calibrate_touch_form_btn.callback = calibrate_touch_form_btn_callback;
    calibrate_form_return_btn.callback = calibrate_form_return_btn_callback;

    gui_button_init(&calibrate_mag_form_btn, 14, 50 + 6, 100, 26, "Cal-Magnet");
    gui_button_init(&calibrate_touch_form_btn, 14, 82 + 6, 100, 26, "Cal-Touch");
    gui_button_init(&calibrate_form_return_btn, 14, 114 + 6, 100, 26, "Return");

    gui_form_init(&calibrate_form, "Calibrate", gui_calibrate_form_update);

    gui_form_bind_button(&calibrate_form, &calibrate_mag_form_btn);
    gui_form_bind_button(&calibrate_form, &calibrate_touch_form_btn);
    gui_form_bind_button(&calibrate_form, &calibrate_form_return_btn);
    gui_form_display(&calibrate_form);
}
