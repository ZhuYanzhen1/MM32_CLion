//
// Created by Lao·Zhu on 2021/5/1.
//

#include "gui_label.h"
#include "gui_base.h"
#include "stdarg.h"
#include "printf.h"
#include "string.h"

void gui_label_init(label_struct_t *label,
                    unsigned char colum,
                    unsigned short color,
                    unsigned int align,
                    const char *name) {
    label->colum = colum;
    label->color = color;
    label->align = align;
    label->next_label = NULL;
    sprintf(label->text, "%s", name);
    if (label->text[21] != 0x00)
        label->text[21] = 0x00;
    if (label->colum > 12)
        label->colum = 12;
}

void gui_label_settext(label_struct_t *label, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(label->text, sizeof(label->text), fmt, ap);
    va_end(ap);
    label->update_flag = 1;
}

void gui_label_update(label_struct_t *label) {
    unsigned char text_length = strlen(label->text) * 6;
    switch (label->align) {
        default:
        case label_align_left:
            gui_printf(0, label->colum * 12 + 16, label->color,
                       C_WHITE, "%s", label->text);
            break;
        case label_align_middle:
            gui_printf((128 - text_length) / 2, label->colum * 12 + 16, label->color,
                       C_WHITE, "%s", label->text);
            break;
        case label_align_right:
            gui_printf(128 - text_length, label->colum * 12 + 16, label->color,
                       C_WHITE, "%s", label->text);
            break;
    }
}
