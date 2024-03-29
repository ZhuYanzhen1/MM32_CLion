/**************************************************************************/ /**
    \file       orgin_form.c
    \brief      orgin_form function Source File
    \author     LaoZhu
    \version    V1.6.1
    \date       24. July 2022
******************************************************************************/

#include "orign_form.h"
#include "main.h"
#include "orgin_gnss_form.h"
#include "orgin_sensor_form.h"

extern form_struct_t main_form;

form_struct_t orgin_form;
static button_struct_t orgin_gnss_form_btn;
static button_struct_t orgin_sensor_form_btn;
static button_struct_t orgin_form_return_btn;

void orgin_gnss_form_btn_callback(void *parameter) {
    (void) parameter;
    gui_orgin_gnss_form_init();
}

void orgin_sensor_form_btn_callback(void *parameter) {
    (void) parameter;
    gui_orgin_sensor_form_init();
    uart2_sendbyte(0x03);
    uart2_dma_buffer_size = 96; // debug_data_size
}

void orgin_form_return_btn_callback(void *parameter) {
    (void) parameter;
    gui_form_display(&main_form);
}

void gui_orgin_form_update(void *parameter) {
    (void) parameter;
}

void gui_orgin_form_init(void) {
    orgin_gnss_form_btn.callback = orgin_gnss_form_btn_callback;
    orgin_sensor_form_btn.callback = orgin_sensor_form_btn_callback;
    orgin_form_return_btn.callback = orgin_form_return_btn_callback;

    gui_button_init(&orgin_gnss_form_btn, 14, 50 + 6, 100, 26, "Ori-Gnss");
    gui_button_init(&orgin_sensor_form_btn, 14, 82 + 6, 100, 26, "Ori-Sensor");
    gui_button_init(&orgin_form_return_btn, 14, 114 + 6, 100, 26, "Return");

    gui_form_init(&orgin_form, "Orgin", gui_orgin_form_update);

    gui_form_bind_button(&orgin_form, &orgin_gnss_form_btn);
    gui_form_bind_button(&orgin_form, &orgin_sensor_form_btn);
    gui_form_bind_button(&orgin_form, &orgin_form_return_btn);
    gui_form_display(&orgin_form);
}
