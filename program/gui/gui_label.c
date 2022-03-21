//
// Created by Lao·Zhu on 2021/5/1.
//

#include "gui_label.h"
#include "string.h"
#include "stdarg.h"
#include "printf.h"

void gui_label_init(label_struct_t *label) {
    label->next_label = NULL;
}

void gui_label_settext(label_struct_t *label, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(label->text, sizeof(label->text), fmt, ap);
    va_end(ap);
}
