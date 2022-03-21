//
// Created by Lao·Zhu on 2021/5/1.
//

#include "gui_label.h"
#include "gui_base.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"

void gui_label_init(label_struct_t *label) {
    char n = 0, row = label->x_pos;
    label->next_label = NULL;
    while (label->text[n] != '\0') {
        gui_putchar(row, label->y_pos, label->text[n], label->color, C_WHITE);
        row += 6;
        n++;
    }
}

void gui_label_settext(label_struct_t *label, const char *fmt, ...) {
    char LCD_BUF[64], n = 0, row = label->x_pos;
    va_list ap;
    memset(LCD_BUF, '\0', sizeof(LCD_BUF));
    va_start(ap, fmt);
    vsprintf(LCD_BUF, fmt, ap);
    va_end(ap);
    while (LCD_BUF[n] != '\0') {
        gui_putchar(row, label->y_pos, LCD_BUF[n], label->color, C_WHITE);
        row += 6;
        n++;
    }
}
