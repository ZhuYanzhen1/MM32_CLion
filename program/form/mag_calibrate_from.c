//
// Created by 16625 on 2022-03-26.
//

#include "main.h"
#include "mag_calibrate_from.h"

static form_struct_t mag_calibrate_from;
static label_struct_t mag_x_from;
static label_struct_t mag_y_from;
static label_struct_t mag_z_from;
static label_struct_t offset_x_from;
static label_struct_t offset_y_from;
static label_struct_t offset_z_from;
static label_struct_t diag_x_from;
static label_struct_t diag_y_from;
static label_struct_t diag_z_from;
static label_struct_t offdiag_x_from;
static label_struct_t offdiag_y_from;
static label_struct_t offdiag_z_from;
static label_struct_t residual_from;
static label_struct_t num_from;

void gui_show_mag_calibrate(void *parameter) {
    (void) parameter;
    gui_label_settext(&offset_x_from, " offset_x:%d   ", debug_data.offset_x);
    gui_label_settext(&offset_y_from, " offset_y:%d   ", debug_data.offset_y);
    gui_label_settext(&offset_z_from, " offset_z:%d   ", debug_data.offset_z);
    gui_label_settext(&diag_x_from, " diag_x:%d   ", debug_data.diag_x);
    gui_label_settext(&diag_y_from, " diag_y:%d   ", debug_data.diag_y);
    gui_label_settext(&diag_z_from, " diag_z:%d   ", debug_data.diag_z);
    gui_label_settext(&offdiag_x_from, " offdiag_x:%d   ", debug_data.offdiag_x);
    gui_label_settext(&offdiag_y_from, " offdiag_y:%d   ", debug_data.offdiag_y);
    gui_label_settext(&offdiag_z_from, " offdiag_z:%d   ", debug_data.offdiag_z);
    gui_label_settext(&residual_from, " residual:%d   ", debug_data.residual);
    gui_label_settext(&num_from, " num:%d   ", debug_data.num);

}

void gui_mag_calibrate_from_init() {
    gui_label_init(&offset_x_from, 1, C_BLACK, label_align_left, " offset_x");
    gui_label_init(&offset_y_from, 2, C_BLACK, label_align_left, " offset_y");
    gui_label_init(&offset_z_from, 3, C_BLACK, label_align_left, " offset_z");
    gui_label_init(&diag_x_from, 4, C_BLACK, label_align_left, " diag_x");
    gui_label_init(&diag_y_from, 5, C_BLACK, label_align_left, " diag_y");
    gui_label_init(&diag_z_from, 6, C_BLACK, label_align_left, " diag_z");
    gui_label_init(&offdiag_x_from, 7, C_BLACK, label_align_left, " offdiag_y");
    gui_label_init(&offdiag_y_from, 8, C_BLACK, label_align_left, " k_nd");
    gui_label_init(&offdiag_z_from, 9, C_BLACK, label_align_left, " offdiag_z");
    gui_label_init(&residual_from, 10, C_BLACK, label_align_left, " residual");
    gui_label_init(&num_from, 0, C_BLACK, label_align_left, " num");

    gui_form_init(&mag_calibrate_from, "MagCalibrateFrom", gui_show_mag_calibrate);

    gui_form_bind_label(&mag_calibrate_from, &offset_x_from);
    gui_form_bind_label(&mag_calibrate_from, &offset_y_from);
    gui_form_bind_label(&mag_calibrate_from, &offset_z_from);
    gui_form_bind_label(&mag_calibrate_from, &diag_x_from);
    gui_form_bind_label(&mag_calibrate_from, &diag_y_from);
    gui_form_bind_label(&mag_calibrate_from, &diag_z_from);
    gui_form_bind_label(&mag_calibrate_from, &offdiag_x_from);
    gui_form_bind_label(&mag_calibrate_from, &offdiag_y_from);
    gui_form_bind_label(&mag_calibrate_from, &offdiag_z_from);
    gui_form_bind_label(&mag_calibrate_from, &residual_from);
    gui_form_bind_label(&mag_calibrate_from, &num_from);

    gui_form_display(&mag_calibrate_from);
}

