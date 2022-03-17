/**************************************************************************/ /**
    \file     tft_lcd.c
    \brief    lcd access function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     19. January 2022
 ******************************************************************************/

#include "tft_lcd.h"
#include "hal_conf.h"
#include "mm32_device.h"
#include "delay.h"
#include "pin.h"
#include "spi.h"
#include "gpio.h"

void lcd_write_command(unsigned int com) {
    GPIO_WriteBit(LCD_A0_PORT, LCD_A0_PIN, Bit_RESET);
    spi3_readwrite_byte(com);
}

void lcd_write_data(unsigned int dat) {
    GPIO_WriteBit(LCD_A0_PORT, LCD_A0_PIN, Bit_SET);
    spi3_readwrite_byte(dat);
}

void lcd_set_address(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    x1 = x1 + 2;
    x2 = x2 + 2;
    y1 = y1 + 1;
    y2 = y2 + 1;
    lcd_write_command(0X2A);
    lcd_write_data(x1 >> 8UL);
    lcd_write_data(x1 & 0XFFUL);
    lcd_write_data(x2 >> 8UL);
    lcd_write_data(x2 & 0XFFUL);
    lcd_write_command(0X2BUL);
    lcd_write_data(y1 >> 8UL);
    lcd_write_data(y1 & 0XFFUL);
    lcd_write_data(y2 >> 8UL);
    lcd_write_data(y2 & 0XFFUL);
    lcd_write_command(0X2CUL);

    GPIO_WriteBit(LCD_A0_PORT, LCD_A0_PIN, Bit_SET);
}

void lcd_set_direction(unsigned char mode) {
    switch (mode) {
        case 1: lcd_write_command(0x36);
            lcd_write_data(0x08);      // BGR Order. 0x00 for RGB Order
            lcd_write_command(0x2C);
            break;
        case 2: lcd_write_command(0x36);
            lcd_write_data(0x80);
            lcd_write_command(0x2C);
            break;
        case 3: lcd_write_command(0x36);
            lcd_write_data(0x40);
            lcd_write_command(0X2C);
            break;
        case 4: lcd_write_command(0x36);
            lcd_write_data(0xC8);   // BGR Order. 0xC0 for RGB Order
            lcd_write_command(0x2C);
            break;
        case 5: lcd_write_command(0x36);
            lcd_write_data(0x20);
            lcd_write_command(0x2C);
            break;
        case 6: lcd_write_command(0x36);
            lcd_write_data(0x60);
            lcd_write_command(0x2C);
            break;
        case 7: lcd_write_command(0x36);
            lcd_write_data(0xA0);
            lcd_write_command(0x2C);
            break;
        case 8: lcd_write_command(0x36);
            lcd_write_data(0xE0);
            lcd_write_command(0x2C);
            break;
        default:break;
    }
}

void lcd_config() {
    lcd_gpio_config();

    GPIO_WriteBit(LCD_RES_PORT, LCD_RES_PIN, Bit_SET);
    delayus(1000);
    GPIO_WriteBit(LCD_RES_PORT, LCD_RES_PIN, Bit_RESET);
    delayus(1000);
    GPIO_WriteBit(LCD_RES_PORT, LCD_RES_PIN, Bit_SET);
    delayus(120 * 1000);
    lcd_write_command(0x11);
    delayus(120 * 1000);

    lcd_write_command(0xB1);
    lcd_write_data(0x05);
    lcd_write_data(0x3C);
    lcd_write_data(0x3C);
    lcd_write_command(0xB2);
    lcd_write_data(0x05);
    lcd_write_data(0x3C);
    lcd_write_data(0x3C);
    lcd_write_command(0xB3);
    lcd_write_data(0x05);
    lcd_write_data(0x3C);
    lcd_write_data(0x3C);
    lcd_write_data(0x05);
    lcd_write_data(0x3C);
    lcd_write_data(0x3C);
    lcd_write_command(0xB4);
    lcd_write_data(0x03);
    lcd_write_command(0xC0);
    lcd_write_data(0x28);
    lcd_write_data(0x08);
    lcd_write_data(0x04);
    lcd_write_command(0xC1);
    lcd_write_data(0XC0);
    lcd_write_command(0xC2);
    lcd_write_data(0x0D);
    lcd_write_data(0x00);
    lcd_write_command(0xC3);
    lcd_write_data(0x8D);
    lcd_write_data(0x2A);
    lcd_write_command(0xC4);
    lcd_write_data(0x8D);
    lcd_write_data(0xEE);
    lcd_write_command(0xC5);
    lcd_write_data(0x1A);
    lcd_write_command(0x36);
    lcd_write_data(0xC8);       // BGR Order. 0xC0 for RGB Order
    lcd_write_command(0xE0);
    lcd_write_data(0x04);
    lcd_write_data(0x22);
    lcd_write_data(0x07);
    lcd_write_data(0x0A);
    lcd_write_data(0x2E);
    lcd_write_data(0x30);
    lcd_write_data(0x25);
    lcd_write_data(0x2A);
    lcd_write_data(0x28);
    lcd_write_data(0x26);
    lcd_write_data(0x2E);
    lcd_write_data(0x3A);
    lcd_write_data(0x00);
    lcd_write_data(0x01);
    lcd_write_data(0x03);
    lcd_write_data(0x13);
    lcd_write_command(0xE1);
    lcd_write_data(0x04);
    lcd_write_data(0x16);
    lcd_write_data(0x06);
    lcd_write_data(0x0D);
    lcd_write_data(0x2D);
    lcd_write_data(0x26);
    lcd_write_data(0x23);
    lcd_write_data(0x27);
    lcd_write_data(0x27);
    lcd_write_data(0x25);
    lcd_write_data(0x2D);
    lcd_write_data(0x3B);
    lcd_write_data(0x00);
    lcd_write_data(0x01);
    lcd_write_data(0x04);
    lcd_write_data(0x13);
    lcd_write_command(0x3A);
    lcd_write_data(0x05);
    lcd_write_command(0x29);
}
