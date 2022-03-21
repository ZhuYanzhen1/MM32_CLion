//
// Created by Lao·Zhu on 2021/5/1.
//

#ifndef MM32F3277_GUI_LABLE_H_
#define MM32F3277_GUI_LABLE_H_

typedef enum {
    label_align_left = 0,
    label_align_middle = 1,
    label_align_right = 2
} label_align_e;

typedef struct LABEL_TYPE_T {
    unsigned char colum;
    unsigned char align;
    unsigned short color;
    char text[24];
    struct LABEL_TYPE_T *next_label;
} label_struct_t;

void gui_label_init(label_struct_t *label);
void gui_label_settext(label_struct_t *label, const char *fmt, ...);
void gui_label_update(label_struct_t *label);

#endif // MM32F3277_GUI_LABLE_H_
