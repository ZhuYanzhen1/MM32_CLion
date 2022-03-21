//
// Created by Lao·Zhu on 2021/5/1.
//

#ifndef PROGRAM_GUI_GUI_LABLE_H_
#define PROGRAM_GUI_GUI_LABLE_H_

typedef struct LABEL_TYPE_T {
    unsigned char x_pos;
    unsigned char y_pos;
    unsigned short color;
    const char *text;
    struct LABEL_TYPE_T *next_label;
} label_struct_t;

void gui_label_init(label_struct_t *label);
void gui_label_settext(label_struct_t *label, const char *fmt, ...);

#endif //PROGRAM_GUI_GUI_LABLE_H_
