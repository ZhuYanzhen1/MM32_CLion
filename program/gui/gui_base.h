//
// Created by Lao·Zhu on 2021/2/26.
//

#ifndef MM32F3277_GUI_GUI_BASE_H_
#define MM32F3277_GUI_GUI_BASE_H_

#include "color.h"

typedef enum {
    Filled = 0,
    UnFilled = 1,
} Filled_Status_e;

void gui_config(void);
void gui_clear_screan(unsigned short color);
void gui_draw_point(unsigned short x, unsigned short y, unsigned short color);
void gui_draw_rectangle(unsigned short sx,
                        unsigned short sy,
                        unsigned short width,
                        unsigned short height,
                        unsigned short color,
                        Filled_Status_e filled);
void gui_draw_vline(unsigned char x1, unsigned char y1, unsigned char height, unsigned short color);
void gui_draw_hline(unsigned char x1, unsigned char y1, unsigned char height, unsigned short color);
void lcd_putchar(unsigned char x,
                 unsigned char line,
                 unsigned char value,
                 unsigned int dcolor,
                 unsigned int bgcolor);
void gui_printf(unsigned char row,
                unsigned char column,
                unsigned int dcolor,
                unsigned int bgcolor,
                const char *fmt,
                ...);

#endif  // MM32F3277_GUI_GUI_BASE_H_
