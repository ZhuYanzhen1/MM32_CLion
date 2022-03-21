//
// Created by Lao·Zhu on 2021/5/1.
//

#ifndef PROGRAM_GUI_GUI_LABLE_H_
#define PROGRAM_GUI_GUI_LABLE_H_

typedef struct {
    unsigned char x_pos;
    unsigned char y_pos;
    unsigned short color;
    const char *text;
} lable_struct_t;

void gui_lable_init(lable_struct_t *lable);
void gui_lable_settext(lable_struct_t *lable, const char *fmt, ...);

#endif //PROGRAM_GUI_GUI_LABLE_H_
