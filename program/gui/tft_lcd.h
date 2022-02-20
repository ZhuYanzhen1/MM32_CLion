/**************************************************************************/ /**
    \file     tft_lcd.h
    \brief    lcd access function Header File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     19. January 2022
 ******************************************************************************/

#ifndef MM32F3277_GUI_LCD_H_
#define MM32F3277_GUI_LCD_H_

void lcd_config();
void lcd_set_direction(unsigned char mode);
void lcd_set_address(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

#endif  // MM32F3277_GUI_LCD_H_
