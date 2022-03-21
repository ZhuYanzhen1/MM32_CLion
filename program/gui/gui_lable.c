//
// Created by Lao·Zhu on 2021/5/1.
//

#include "gui_lable.h"
#include "gui_base.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"

void gui_lable_init(lable_struct_t *lable) {
    char n = 0, row = lable->x_pos;
    while (lable->text[n] != '\0') {
        gui_putchar(row, lable->y_pos, lable->text[n], lable->color, C_WHITE);
        row += 6;
        n++;
    }
}

void gui_lable_settext(lable_struct_t *lable, const char *fmt, ...) {
    char LCD_BUF[64], n = 0, row = lable->x_pos;
    va_list ap;
    memset(LCD_BUF, '\0', sizeof(LCD_BUF));
    va_start(ap, fmt);
    vsprintf(LCD_BUF, fmt, ap);
    va_end(ap);
    while (LCD_BUF[n] != '\0') {
        gui_putchar(row, lable->y_pos, LCD_BUF[n], lable->color, C_WHITE);
        row += 6;
        n++;
    }
}
