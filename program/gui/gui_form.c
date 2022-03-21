//
// Created by LaoZhu on 2022/3/21.
//

#include "gui_form.h"
#include "gui_base.h"
#include "gui_button.h"
#include "gui_label.h"
#include "string.h"

static unsigned char gui_form_counter = 0;
static form_struct_t *current_form = NULL;

void gui_form_init(form_struct_t *form, const char *name) {
    gui_form_counter++;
    form->number = gui_form_counter;
    form->button_num = 0;
    form->label_num = 0;
    form->first_button = NULL;
    form->first_label = NULL;
    form->text = name;
}

void gui_form_bind_label(form_struct_t *form, void *label) {
    form->label_num++;
    if (form->first_label == NULL)
        form->first_label = label;
    else {
        label_struct_t *tmp_label = form->first_label;
        while (tmp_label->next_label != NULL)
            tmp_label = tmp_label->next_label;
        tmp_label->next_label = label;
    }
}

void gui_form_bind_button(form_struct_t *form, void *button) {
    form->button_num++;
    if (form->first_button == NULL)
        form->first_button = button;
    else {
        button_struct_t *tmp_button = form->first_button;
        while (tmp_button->next_button != NULL)
            tmp_button = tmp_button->next_button;
        tmp_button->next_button = button;
    }
}

void gui_form_display(form_struct_t *form) {
    unsigned char button_counter = form->button_num;
    unsigned char label_counter = form->label_num;
    unsigned char text_x = (128 - strlen(form->text) * 6) / 2;
    gui_draw_rectangle(0, 0, 128, 16, FORM_TITLE_COLOR, Filled);
    gui_printf(text_x, 2, C_BLACK, FORM_TITLE_COLOR, "%s", form->text);
    if (button_counter != 0) {
        button_struct_t *tmp_button = form->first_button;
        while (button_counter--) {
            gui_button_update(tmp_button, button_normal_status);
            tmp_button = tmp_button->next_button;
        }
    }
    if (label_counter != 0) {
        label_struct_t *tmp_label = form->first_label;
        while (label_counter--) {
            gui_label_update(tmp_label);
            tmp_label = tmp_label->next_label;
        }
    }
}

void gui_form_update(form_struct_t *form) {
    unsigned char button_counter = form->button_num;
    unsigned char label_counter = form->label_num;
    if (button_counter != 0) {
        button_struct_t *tmp_button = form->first_button;
        while (button_counter--) {
            if (tmp_button->update_flag == 1) {
                gui_button_update(tmp_button, button_normal_status);
                tmp_button->update_flag = 0;
            }
            tmp_button = tmp_button->next_button;
        }
    }
    if (label_counter != 0) {
        label_struct_t *tmp_label = form->first_label;
        while (label_counter--) {
            if (tmp_label->update_flag == 1) {
                gui_label_update(tmp_label);
                tmp_label->update_flag = 0;
            }
            tmp_label = tmp_label->next_label;
        }
    }
}
