//
// Created by Lao·Zhu on 2021/5/1.
//

#ifndef MM32F3277_GUI_LABLE_H_
#define MM32F3277_GUI_LABLE_H_

typedef struct LABEL_TYPE_T {
    unsigned char x_pos;
    unsigned char y_pos;
    unsigned short color;
    char text[24];
    struct LABEL_TYPE_T *next_label;
} label_struct_t;

void gui_label_init(label_struct_t *label);
void gui_label_settext(label_struct_t *label, const char *fmt, ...);

#endif // MM32F3277_GUI_LABLE_H_
