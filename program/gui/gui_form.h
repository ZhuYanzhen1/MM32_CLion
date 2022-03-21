//
// Created by LaoZhu on 2022/3/21.
//

#ifndef MM32F3277_GUI_FORM_H_
#define MM32F3277_GUI_FORM_H_

#define FORM_TITLE_COLOR    0xFCC6

typedef struct FORM_TYPE_T {
    unsigned char number;
    unsigned char label_num;
    unsigned char button_num;
    const char *text;
    void *first_label;
    void *first_button;
} form_struct_t;

void gui_form_init(form_struct_t *form);
void gui_form_display(form_struct_t *form);
void gui_update(form_struct_t *form);
void gui_form_bind_label(form_struct_t *form, void *label);
void gui_form_bind_button(form_struct_t *form, void *button);

#endif // MM32F3277_GUI_FORM_H_
