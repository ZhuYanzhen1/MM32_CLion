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

void gui_form_init(form_struct_t *form) {
    gui_form_counter++;
    form->number = gui_form_counter;
    form->button_num = 0;
    form->label_num = 0;
    form->first_button = NULL;
    form->first_label = NULL;
    unsigned char text_x = (128 - strlen(form->text) * 6) / 2;
    gui_draw_rectangle(0, 0, 128, 16, FORM_TITLE_COLOR, Filled);
    gui_printf(text_x, 2, C_BLACK, FORM_TITLE_COLOR, "%s", form->text);
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