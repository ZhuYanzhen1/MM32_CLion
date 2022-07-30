/**************************************************************************/ /**
    \file     gui_label.c
    \brief    gui from features function Source File
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

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
            gui_draw_rectangle(text_length,
                               label->colum * 12 + 16,
                               128 - text_length,
                               12,
                               C_WHITE,
                               Filled);
            gui_printf(0, label->colum * 12 + 16, label->color,
                       C_WHITE, "%s", label->text);
            break;
        case label_align_middle:
            gui_draw_rectangle(0,
                               label->colum * 12 + 16,
                               (128 - text_length) / 2,
                               12,
                               C_WHITE,
                               Filled);
            gui_draw_rectangle((128 - text_length) / 2 + text_length,
                               label->colum * 12 + 16,
                               (128 - text_length) / 2,
                               12,
                               C_WHITE,
                               Filled);
            gui_printf((128 - text_length) / 2, label->colum * 12 + 16, label->color,
                       C_WHITE, "%s", label->text);
            break;
        case label_align_right:
            gui_draw_rectangle(0,
                               label->colum * 12 + 16,
                               128 - text_length,
                               12,
                               C_WHITE,
                               Filled);
            gui_printf(128 - text_length, label->colum * 12 + 16, label->color,
                       C_WHITE, "%s", label->text);
            break;
    }
}
