//
// Created by Lao·Zhu on 2021/2/26.
//

#include "tft_lcd.h"
#include "gui_base.h"
#include "string.h"
#include "stdarg.h"
#include "font.h"
#include "printf.h"

void gui_config(void) {
    lcd_config();
    lcd_set_direction(4);
    gui_clear_screan(C_WHITE);
}

void gui_putchar(unsigned char x,
                 unsigned char line,
                 unsigned char value,
                 unsigned int dcolor,
                 unsigned int bgcolor) {
    unsigned char i, j;
    unsigned char *temp = (unsigned char *) &Font_6_12[0];
    temp += (value - 32) * 12;
    lcd_set_address(x, line, x + 5, line + 11);
    for (j = 0; j < 12; j++) {
        for (i = 0; i < 6; i++) {
            if ((*temp & (1 << (7 - i))) != 0)
                lcd_write_data(dcolor);
            else
                lcd_write_data(bgcolor);
        }
        temp++;
    }
}

void gui_printf(unsigned char row,
                unsigned char column,
                unsigned int dcolor,
                unsigned int bgcolor,
                const char *fmt,
                ...) {
    char lcd_tmp_buffer[32] = {0};
    unsigned char n = 0;
    va_list ap;
    memset(lcd_tmp_buffer, '\0', sizeof(lcd_tmp_buffer));
    va_start(ap, fmt);
    vsnprintf(lcd_tmp_buffer, sizeof(lcd_tmp_buffer), fmt, ap);
    va_end(ap);
    while (lcd_tmp_buffer[n] != '\0') {
        gui_putchar(row, column, lcd_tmp_buffer[n], dcolor, bgcolor);
        row += 6;
        n++;
    }
}

void gui_clear_screan(unsigned short color) {
    unsigned char i, j;
    lcd_set_address(0, 0, 127, 159);
    for (i = 0; i < 128; i++) {
        for (j = 0; j < 160; j++)
            lcd_write_data(color);
    }
}

void gui_draw_point(unsigned short x, unsigned short y, unsigned short color) {
    lcd_set_address(x, y, x, y);
    lcd_write_data(color);
}

void gui_draw_hline(unsigned char x1, unsigned char y1, unsigned char width, unsigned short color) {
    unsigned char temp;
    lcd_set_address(x1, y1, x1 + width - 1, y1);
    for (temp = 0; temp < width; temp++)
        lcd_write_data(color);
}

void gui_draw_vline(unsigned char x1, unsigned char y1, unsigned char height, unsigned short color) {
    unsigned char temp;
    lcd_set_address(x1, y1, x1, y1 + height - 1);
    for (temp = 0; temp < height; temp++)
        lcd_write_data(color);
}

void gui_draw_rectangle(unsigned short sx,
                        unsigned short sy,
                        unsigned short width,
                        unsigned short height,
                        unsigned short color,
                        Filled_Status_e filled) {
    if (filled == Filled) {
        unsigned short temp, temp1;
        lcd_set_address(sx, sy, sx + width - 1, sy + height - 1);
        for (temp = 0; temp < width; temp++)
            for (temp1 = 0; temp1 < height; temp1++)
                lcd_write_data(color);
    } else {
        gui_draw_hline(sx, sy, width, color);
        gui_draw_vline(sx, sy, height, color);
        gui_draw_hline(sx, sy + height - 1, width, color);
        gui_draw_vline(sx + width - 1, sy, height, color);
    }
}
