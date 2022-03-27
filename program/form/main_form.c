//
// Created by LaoZhu on 2022/3/22.
//

#include "main.h"

form_struct_t main_form;
static button_struct_t origin_form_btn;
static button_struct_t fusion_form_btn;
static button_struct_t calibrate_form_btn;
static button_struct_t debug_form_btn;

void origin_form_btn_callback(void *parameter) {
    (void) parameter;
}

void fusion_form_btn_callback(void *parameter) {
    (void) parameter;
}

void calibrate_form_btn_callback(void *parameter) {
    (void) parameter;
}

void debug_form_btn_callback(void *parameter) {
    (void) parameter;
}

void gui_main_form_update(void *parameter) {
    (void) parameter;
}

void gui_main_form_init(void) {
    origin_form_btn.callback = origin_form_btn_callback;
    fusion_form_btn.callback = fusion_form_btn_callback;
    calibrate_form_btn.callback = calibrate_form_btn_callback;
    debug_form_btn.callback = debug_form_btn_callback;

    gui_button_init(&origin_form_btn, 14, 18 + 6, 100, 26, " Origin Data");
    gui_button_init(&fusion_form_btn, 14, 50 + 6, 100, 26, "Fusion Data");
    gui_button_init(&calibrate_form_btn, 14, 82 + 6, 100, 26, "Calibrate");
    gui_button_init(&debug_form_btn, 14, 114 + 6, 100, 26, " Debug Mode");

    gui_form_init(&main_form, "MainForm", gui_main_form_update);

    gui_form_bind_button(&main_form, &origin_form_btn);
    gui_form_bind_button(&main_form, &fusion_form_btn);
    gui_form_bind_button(&main_form, &calibrate_form_btn);
    gui_form_bind_button(&main_form, &debug_form_btn);
    gui_form_display(&main_form);
}
