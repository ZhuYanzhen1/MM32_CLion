//
// Created by 16625 on 2022-03-26.
//

#include "main.h"
#include "calibrate_mag_form.h"

static form_struct_t mag_calibrate_form;
static label_struct_t offset_x_form;
static label_struct_t offset_y_form;
static label_struct_t offset_z_form;
static label_struct_t diag_x_form;
static label_struct_t diag_y_form;
static label_struct_t diag_z_form;
static label_struct_t offdiag_x_form;
static label_struct_t offdiag_y_form;
static label_struct_t offdiag_z_form;
static label_struct_t residual_form;
static label_struct_t num_form;

void gui_show_mag_calibrate(void *parameter) {
    (void) parameter;
    gui_label_settext(&offset_x_form, " offset_x:%d   ", debug_data.offset_x);
    gui_label_settext(&offset_y_form, " offset_y:%d   ", debug_data.offset_y);
    gui_label_settext(&offset_z_form, " offset_z:%d   ", debug_data.offset_z);
    gui_label_settext(&diag_x_form, " diag_x:%d   ", debug_data.diag_x);
    gui_label_settext(&diag_y_form, " diag_y:%d   ", debug_data.diag_y);
    gui_label_settext(&diag_z_form, " diag_z:%d   ", debug_data.diag_z);
    gui_label_settext(&offdiag_x_form, " offdiag_x:%d   ", debug_data.offdiag_x);
    gui_label_settext(&offdiag_y_form, " offdiag_y:%d   ", debug_data.offdiag_y);
    gui_label_settext(&offdiag_z_form, " offdiag_z:%d   ", debug_data.offdiag_z);
    gui_label_settext(&residual_form, " residual:%d   ", debug_data.residual);
    gui_label_settext(&num_form, " num:%d   ", debug_data.num);

}

void gui_mag_calibrate_form_init() {
    gui_label_init(&offset_x_form, 1, C_BLACK, label_align_left, " offset_x");
    gui_label_init(&offset_y_form, 2, C_BLACK, label_align_left, " offset_y");
    gui_label_init(&offset_z_form, 3, C_BLACK, label_align_left, " offset_z");
    gui_label_init(&diag_x_form, 4, C_BLACK, label_align_left, " diag_x");
    gui_label_init(&diag_y_form, 5, C_BLACK, label_align_left, " diag_y");
    gui_label_init(&diag_z_form, 6, C_BLACK, label_align_left, " diag_z");
    gui_label_init(&offdiag_x_form, 7, C_BLACK, label_align_left, " offdiag_x");
    gui_label_init(&offdiag_y_form, 8, C_BLACK, label_align_left, " offdiag_y");
    gui_label_init(&offdiag_z_form, 9, C_BLACK, label_align_left, " offdiag_z");
    gui_label_init(&residual_form, 10, C_BLACK, label_align_left, " residual");
    gui_label_init(&num_form, 0, C_BLACK, label_align_left, " num");

    gui_form_init(&mag_calibrate_form, "Cal-Magnet", gui_show_mag_calibrate);

    gui_form_bind_label(&mag_calibrate_form, &offset_x_form);
    gui_form_bind_label(&mag_calibrate_form, &offset_y_form);
    gui_form_bind_label(&mag_calibrate_form, &offset_z_form);
    gui_form_bind_label(&mag_calibrate_form, &diag_x_form);
    gui_form_bind_label(&mag_calibrate_form, &diag_y_form);
    gui_form_bind_label(&mag_calibrate_form, &diag_z_form);
    gui_form_bind_label(&mag_calibrate_form, &offdiag_x_form);
    gui_form_bind_label(&mag_calibrate_form, &offdiag_y_form);
    gui_form_bind_label(&mag_calibrate_form, &offdiag_z_form);
    gui_form_bind_label(&mag_calibrate_form, &residual_form);
    gui_form_bind_label(&mag_calibrate_form, &num_form);

    gui_form_display(&mag_calibrate_form);
}

