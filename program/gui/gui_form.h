/**************************************************************************/ /**
    \file     gui_from.h
    \brief    gui from features function Header File
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

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
    void (*callback)(void *object);
} form_struct_t;

void gui_form_init(form_struct_t *form, const char *name, void (*callback)(void *object));
void gui_form_display(form_struct_t *form);
void gui_form_update(unsigned char x_pos, unsigned char y_pos);
void gui_form_bind_label(form_struct_t *form, void *label);
void gui_form_bind_button(form_struct_t *form, void *button);

#endif  // MM32F3277_GUI_FORM_H_
