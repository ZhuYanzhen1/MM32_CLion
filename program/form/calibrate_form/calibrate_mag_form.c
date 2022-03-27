//
// Created by 16625 on 2022-03-26.
//

#include "main.h"
#include "calibrate_mag_form.h"

static form_struct_t mag_calibrate_form;

static label_struct_t num_label;
static label_struct_t offset_x_label;
static label_struct_t offset_y_label;
static label_struct_t offset_z_label;
static label_struct_t a_colum1_label;
static label_struct_t a_colum2_label;
static label_struct_t a_colum3_label;
static label_struct_t residual_label;

void gui_show_mag_calibrate(void *parameter) {
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
                      (float) debug_data.offdiag_x / 1000.0f, (float) debug_data.diag_x / 1000.0f);
    gui_label_settext(&residual_label, " Radius:%d", (float) debug_data.residual / 1000.0f);
}

void gui_mag_calibrate_form_init() {
    gui_label_init(&num_label, 0, C_BLACK, label_align_left, " num");
    gui_label_init(&offset_x_label, 1, C_BLACK, label_align_left, " Offset X:");
    gui_label_init(&offset_y_label, 2, C_BLACK, label_align_left, " Offset Y:");
    gui_label_init(&offset_z_label, 3, C_BLACK, label_align_left, " Offset Z:");
    gui_label_init(&a_colum1_label, 4, C_BLACK, label_align_middle, "  ");
    gui_label_init(&a_colum2_label, 5, C_BLACK, label_align_middle, "  ");
    gui_label_init(&a_colum3_label, 6, C_BLACK, label_align_middle, "  ");
    gui_label_init(&residual_label, 7, C_BLACK, label_align_left, " Radius:");

    gui_form_init(&mag_calibrate_form, "Cal-Magnet", gui_show_mag_calibrate);

    gui_form_bind_label(&mag_calibrate_form, &num_label);
    gui_form_bind_label(&mag_calibrate_form, &offset_x_label);
    gui_form_bind_label(&mag_calibrate_form, &offset_y_label);
    gui_form_bind_label(&mag_calibrate_form, &offset_z_label);
    gui_form_bind_label(&mag_calibrate_form, &a_colum1_label);
    gui_form_bind_label(&mag_calibrate_form, &a_colum2_label);
    gui_form_bind_label(&mag_calibrate_form, &a_colum3_label);
    gui_form_bind_label(&mag_calibrate_form, &residual_label);

    gui_form_display(&mag_calibrate_form);
}
