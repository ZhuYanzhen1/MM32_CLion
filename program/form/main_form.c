//
// Created by LaoZhu on 2022/3/22.
//

#include "main.h"
#include "fusion_form.h"

static form_struct_t main_form;
static button_struct_t fusion_form_btn;

void fusion_form_btn_callback(void *parameter) {
    (void) parameter;
    gui_fusion_form_init();
}

void gui_main_form_update(void *parameter) {
    (void) parameter;
}

void gui_main_form_init(void) {
    fusion_form_btn.callback = fusion_form_btn_callback;
    gui_button_init(&fusion_form_btn, 10, 120, 49, 30, "Fusion");

    gui_form_init(&main_form, "MainForm", gui_main_form_update);

    gui_form_bind_button(&main_form, &fusion_form_btn);
    gui_form_display(&main_form);
}
