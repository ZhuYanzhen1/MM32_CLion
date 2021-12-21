//
// Created by Lao·Zhu on 2021/4/7.
//

#ifndef _LCD_H_
#define _LCD_H_

void lcd_config();
void lcd_set_direction(unsigned char mode);
void lcd_write_data(unsigned int dat16);
void lcd_set_address(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

#endif
