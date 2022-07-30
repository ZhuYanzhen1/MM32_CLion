/**************************************************************************/ /**
    \file       calibrate_mag_form.c
    \brief      calibrate_mag_form function Source File
    \author     LaoZhu
    \version    V1.6.1
    \date       24. July 2022
******************************************************************************/
#include "main.h"
#include "calibrate_mag_form.h"

extern form_struct_t calibrate_form;
static form_struct_t mag_calibrate_form;

static label_struct_t num_label;
static label_struct_t offset_x_label;
static label_struct_t offset_y_label;
static label_struct_t offset_z_label;
static label_struct_t a_colum1_label;
static label_struct_t a_colum2_label;
static label_struct_t a_colum3_label;
static label_struct_t radius_label;

static button_struct_t calibrate_mag_btn;
static button_struct_t calibrate_mag_form_return_btn;

void calibrate_mag_btn_callback(void *parameter) {
    (void) parameter;
    uart2_sendbyte(0x01);
    uart2_dma_buffer_size = 96;
}

void calibrate_mag_form_return_btn_callback(void *parameter) {
    (void) parameter;
    uart2_sendbyte(0x00);
    uart2_dma_buffer_size = 16; // small_packets_size
    gui_form_display(&calibrate_form);
}

void gui_mag_calibrate_form_update(void *parameter) {
    (void) parameter;
    gui_label_settext(&num_label, " num:%d", debug_data.num);
    gui_label_settext(&offset_x_label, " Offset X:%.3f", (float) debug_data.offset_x / 1000.0f);
    gui_label_settext(&offset_y_label, " Offset Y:%.3f", (float) debug_data.offset_y / 1000.0f);
    gui_label_settext(&offset_z_label, " Offset Z:%.3f", (float) debug_data.offset_z / 1000.0f);
    gui_label_settext(&a_colum1_label, " %.3f  %.3f  %.3f ", (float) debug_data.diag_x / 1000.0f,
                      (float) debug_data.offdiag_z / 1000.0f, (float) debug_data.offdiag_y / 1000.0f);
    gui_label_settext(&a_colum2_label, " %.3f  %.3f  %.3f ", (float) debug_data.offdiag_z / 1000.0f,
                      (float) debug_data.diag_y / 1000.0f, (float) debug_data.offdiag_x / 1000.0f);
    gui_label_settext(&a_colum3_label, " %.3f  %.3f  %.3f ", (float) debug_data.offdiag_y / 1000.0f,
                      (float) debug_data.offdiag_x / 1000.0f, (float) debug_data.diag_z / 1000.0f);
    gui_label_settext(&radius_label, " Radius:%.3f", (float) debug_data.residual / 1000.0f);
}

void gui_mag_calibrate_form_init() {
    gui_label_init(&num_label, 0, C_BLACK, label_align_left, " num");
    gui_label_init(&offset_x_label, 1, C_BLACK, label_align_left, " Offset X:");
    gui_label_init(&offset_y_label, 2, C_BLACK, label_align_left, " Offset Y:");
    gui_label_init(&offset_z_label, 3, C_BLACK, label_align_left, " Offset Z:");
    gui_label_init(&a_colum1_label, 4, C_BLACK, label_align_middle, "  ");
    gui_label_init(&a_colum2_label, 5, C_BLACK, label_align_middle, "  ");
    gui_label_init(&a_colum3_label, 6, C_BLACK, label_align_middle, "  ");
    gui_label_init(&radius_label, 7, C_BLACK, label_align_left, " Radius:");

    calibrate_mag_btn.callback = calibrate_mag_btn_callback;
    calibrate_mag_form_return_btn.callback = calibrate_mag_form_return_btn_callback;
    gui_button_init(&calibrate_mag_btn, 4, 120, 58, 30, "Calibrate");
    gui_button_init(&calibrate_mag_form_return_btn, 66, 120, 58, 30, "Return");

    gui_form_init(&mag_calibrate_form, "Cal-Magnet", gui_mag_calibrate_form_update);

    gui_form_bind_label(&mag_calibrate_form, &num_label);
    gui_form_bind_label(&mag_calibrate_form, &offset_x_label);
    gui_form_bind_label(&mag_calibrate_form, &offset_y_label);
    gui_form_bind_label(&mag_calibrate_form, &offset_z_label);
    gui_form_bind_label(&mag_calibrate_form, &a_colum1_label);
    gui_form_bind_label(&mag_calibrate_form, &a_colum2_label);
    gui_form_bind_label(&mag_calibrate_form, &a_colum3_label);
    gui_form_bind_label(&mag_calibrate_form, &radius_label);

    gui_form_bind_button(&mag_calibrate_form, &calibrate_mag_btn);
    gui_form_bind_button(&mag_calibrate_form, &calibrate_mag_form_return_btn);

    gui_form_display(&mag_calibrate_form);
}
