/**************************************************************************/ /**
    \file       calibrate_form.c
    \brief      calibrate_form function Source File
    \author     LaoZhu
    \version    V1.6.1
    \date       24. July 2022
******************************************************************************/

#include "main.h"
#include "calibrate_form.h"
#include "calibrate_mag_form.h"
#include "calibrate_touch_form.h"

extern form_struct_t main_form;
form_struct_t calibrate_form;
static button_struct_t calibrate_mag_form_btn;
static button_struct_t calibrate_touch_form_btn;
static button_struct_t calibrate_cold_start_btn;
static button_struct_t calibrate_form_return_btn;

void calibrate_mag_form_btn_callback(void *parameter) {
    (void) parameter;
    uart2_sendbyte(0x02);
    uart2_dma_buffer_size = 96;
    gui_mag_calibrate_form_init();
}

void calibrate_touch_form_btn_callback(void *parameter) {
    (void) parameter;
    gui_calibrate_touch_form_init();
}

void calibrate_cold_start_btn_callback(void *parameter) {
    (void) parameter;
    char output_cold_start[] = {0xF1, 0xD9, 0x06, 0x40, 0x01, 0x00, 0x01, 0x48, 0x22};
    for (unsigned char i = 0; i < (unsigned char) sizeof(output_cold_start); i++)
        uart4_sendbyte((output_cold_start)[i]);
    delayms(100);
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
    calibrate_cold_start_btn.callback = calibrate_cold_start_btn_callback;
    calibrate_form_return_btn.callback = calibrate_form_return_btn_callback;

    gui_button_init(&calibrate_cold_start_btn, 14, 18 + 6, 100, 26, "Cold-Start");
    gui_button_init(&calibrate_mag_form_btn, 14, 50 + 6, 100, 26, "Cal-Magnet");
    gui_button_init(&calibrate_touch_form_btn, 14, 82 + 6, 100, 26, "Cal-Touch");
    gui_button_init(&calibrate_form_return_btn, 14, 114 + 6, 100, 26, "Return");

    gui_form_init(&calibrate_form, "Calibrate", gui_calibrate_form_update);

    gui_form_bind_button(&calibrate_form, &calibrate_mag_form_btn);
    gui_form_bind_button(&calibrate_form, &calibrate_touch_form_btn);
    gui_form_bind_button(&calibrate_form, &calibrate_cold_start_btn);
    gui_form_bind_button(&calibrate_form, &calibrate_form_return_btn);
    gui_form_display(&calibrate_form);
}
