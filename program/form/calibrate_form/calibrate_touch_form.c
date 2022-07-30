/**************************************************************************/ /**
    \file       calibrate_touch_form.c
    \brief      calibrate_touch_form function Source File
    \author     LaoZhu
    \version    V1.6.1
    \date       24. July 2022
******************************************************************************/

#include "main.h"
#include "calibrate_touch_form.h"

extern TaskHandle_t gui_taskhandler;
extern short touch_x_offset, touch_y_offset;
extern float touch_x_factor, touch_y_factor;

extern form_struct_t calibrate_form;
static form_struct_t calibrate_touch_form;
static label_struct_t touch_xoffset_label;
static label_struct_t touch_yoffset_label;
static label_struct_t touch_xfactor_label;
static label_struct_t touch_yfactor_label;
static button_struct_t calibrate_touch_btn;
static button_struct_t calibrate_touch_form_return_btn;

static void calibrate_touch_btn_callback(void *parameter) {
    (void) parameter;
    vTaskSuspend(gui_taskhandler);
    xpt2046_calibrate();
    at24c02_saveparams();
    gui_form_display(&calibrate_touch_form);
    vTaskResume(gui_taskhandler);
}

static void calibrate_touch_form_return_btn_callback(void *parameter) {
    (void) parameter;
    gui_form_display(&calibrate_form);
}

static void gui_calibrate_touch_form_update(void *parameter) {
    (void) parameter;
    gui_label_settext(&touch_xoffset_label, " X Offset:%d", touch_x_offset);
    gui_label_settext(&touch_yoffset_label, " Y Offset:%d", touch_y_offset);
    gui_label_settext(&touch_xfactor_label, " X Factor:%.5f", touch_x_factor);
    gui_label_settext(&touch_yfactor_label, " Y Factor:%.5f", touch_y_factor);
}

void gui_calibrate_touch_form_init(void) {
    calibrate_touch_btn.callback = calibrate_touch_btn_callback;
    calibrate_touch_form_return_btn.callback = calibrate_touch_form_return_btn_callback;

    gui_label_init(&touch_xoffset_label, 0, C_BLACK, label_align_left, " X Offset:");
    gui_label_init(&touch_yoffset_label, 1, C_BLACK, label_align_left, " Y Offset:");
    gui_label_init(&touch_xfactor_label, 2, C_BLACK, label_align_left, " X Factor:");
    gui_label_init(&touch_yfactor_label, 3, C_BLACK, label_align_left, " Y Factor:");

    gui_button_init(&calibrate_touch_btn, 14, 82 + 6, 100, 26, "Calibrate");
    gui_button_init(&calibrate_touch_form_return_btn, 14, 114 + 6, 100, 26, "Return");

    gui_form_init(&calibrate_touch_form, "Cal-Touch", gui_calibrate_touch_form_update);

    gui_form_bind_label(&calibrate_touch_form, &touch_xoffset_label);
    gui_form_bind_label(&calibrate_touch_form, &touch_yoffset_label);
    gui_form_bind_label(&calibrate_touch_form, &touch_xfactor_label);
    gui_form_bind_label(&calibrate_touch_form, &touch_yfactor_label);

    gui_form_bind_button(&calibrate_touch_form, &calibrate_touch_btn);
    gui_form_bind_button(&calibrate_touch_form, &calibrate_touch_form_return_btn);
    gui_form_display(&calibrate_touch_form);
}
