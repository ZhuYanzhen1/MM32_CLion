/**************************************************************************/ /**
    \file     gui_base.c
    \brief    gui basic features function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     19. January 2022
 ******************************************************************************/

#include "tft_lcd.h"
#include "gui_base.h"
#include "stdarg.h"
#include "font.h"
#include "printf.h"
#include "dma.h"

volatile unsigned char lcd_buffer[128 * 160 * 2] = {0};

void gui_config(void) {
    lcd_config();
    lcd_set_direction(4);
    for (unsigned short i = 0; i < 128 * 160; i++)
        ((unsigned short *) lcd_buffer)[i] = C_WHITE;
    lcd_set_address(0, 0, 127, 159);
    spi2_dma_sent_config((unsigned int *) lcd_buffer, 128 * 160 * 2);
    spi2_dma_set_transmit_buffer((unsigned int *) lcd_buffer, 128 * 160 * 2);
}

void gui_flush(void) {
    lcd_set_address(0, 0, 127, 159);
    spi2_dma_set_transmit_buffer((unsigned int *) lcd_buffer, 128 * 160 * 2);
}

void gui_putchar(unsigned char x,
                 unsigned char line,
                 unsigned char value,
                 unsigned int dcolor,
                 unsigned int bgcolor) {
    unsigned char i, j;
    unsigned char *temp = (unsigned char *) &Font_6_12[0];
    temp += (value - 32) * 12;
    for (j = 0; j < 12; j++) {
        for (i = 0; i < 6; i++) {
            if ((*temp & (1 << (7 - i))) != 0)
                ((unsigned short *) lcd_buffer)[(line + j) * 128 + x + i] = dcolor;
            else
                ((unsigned short *) lcd_buffer)[(line + j) * 128 + x + i] = bgcolor;
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
    for (unsigned short i = 0; i < 128 * 160; i++)
        ((unsigned short *) lcd_buffer)[i] = color;
    lcd_set_address(0, 0, 127, 159);
    spi2_dma_set_transmit_buffer((unsigned int *) lcd_buffer, 128 * 160 * 2);
}

void gui_draw_hline(unsigned char x1, unsigned char y1, unsigned char width, unsigned short color) {
    for (unsigned char temp = 0; temp < width; temp++)
        ((unsigned short *) lcd_buffer)[y1 * 128 + x1 + temp] = color;
}

void gui_draw_vline(unsigned char x1, unsigned char y1, unsigned char height, unsigned short color) {
    for (unsigned char temp = 0; temp < height; temp++)
        ((unsigned short *) lcd_buffer)[(y1 + temp) * 128 + x1] = color;
}

void gui_draw_rectangle(unsigned short sx,
                        unsigned short sy,
                        unsigned short width,
                        unsigned short height,
                        unsigned short color,
                        Filled_Status_e filled) {
    if (filled == Filled) {
//        unsigned short temp, temp1;
//        lcd_set_address(sx, sy, sx + width - 1, sy + height - 1);
//        for (temp = 0; temp < width; temp++)
//            for (temp1 = 0; temp1 < height; temp1++)
//                lcd_write_data(color);
    } else {
        gui_draw_hline(sx, sy, width, color);
        gui_draw_vline(sx, sy, height, color);
        gui_draw_hline(sx, sy + height - 1, width, color);
        gui_draw_vline(sx + width - 1, sy, height, color);
    }
}
