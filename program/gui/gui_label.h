/**************************************************************************/ /**
    \file     gui_label.h
    \brief    gui from features function Header File
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

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
    unsigned int update_flag;
    char text[24];
    struct LABEL_TYPE_T *next_label;
} label_struct_t;

void gui_label_init(label_struct_t *label,
                    unsigned char colum,
                    unsigned short color,
                    unsigned int align,
                    const char *name);
void gui_label_settext(label_struct_t *label, const char *fmt, ...);
void gui_label_update(label_struct_t *label);

#endif  // MM32F3277_GUI_LABLE_H_
