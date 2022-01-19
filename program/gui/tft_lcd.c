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

void LCD_WriteReg(unsigned int Com) {
    GPIO_WriteBit(LCD_A0_PORT, LCD_A0_PIN, Bit_RESET);
    GPIO_WriteBit(LCD_CSB_PORT, LCD_CSB_PIN, Bit_RESET);
    spi2_readwrite_byte(Com);
    GPIO_WriteBit(LCD_CSB_PORT, LCD_CSB_PIN, Bit_SET);
}

void LCD_WriteData(unsigned int dat) {
    GPIO_WriteBit(LCD_A0_PORT, LCD_A0_PIN, Bit_SET);
    GPIO_WriteBit(LCD_CSB_PORT, LCD_CSB_PIN, Bit_RESET);
    spi2_readwrite_byte(dat);
    GPIO_WriteBit(LCD_CSB_PORT, LCD_CSB_PIN, Bit_SET);
}

void lcd_write_data(unsigned int dat16) {
    LCD_WriteData(dat16 >> 8UL);
    LCD_WriteData(dat16);
}

void lcd_set_address(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    x1 = x1 + 2;
    x2 = x2 + 2;
    y1 = y1 + 1;
    y2 = y2 + 1;
    LCD_WriteReg(0X2A);
    LCD_WriteData(x1 >> 8UL);
    LCD_WriteData(x1 & 0XFFUL);
    LCD_WriteData(x2 >> 8UL);
    LCD_WriteData(x2 & 0XFFUL);
    LCD_WriteReg(0X2BUL);
    LCD_WriteData(y1 >> 8UL);
    LCD_WriteData(y1 & 0XFFUL);
    LCD_WriteData(y2 >> 8UL);
    LCD_WriteData(y2 & 0XFFUL);
    LCD_WriteReg(0X2CUL);
}

void lcd_set_direction(unsigned char mode) {
    switch (mode) {
        case 1: LCD_WriteReg(0x36);
            LCD_WriteData(0x00);
            LCD_WriteReg(0x2C);
            break;
        case 2: LCD_WriteReg(0x36);
            LCD_WriteData(0x80);
            LCD_WriteReg(0x2C);
            break;
        case 3: LCD_WriteReg(0x36);
            LCD_WriteData(0x40);
            LCD_WriteReg(0X2C);
            break;
        case 4: LCD_WriteReg(0x36);
            LCD_WriteData(0xC0);
            LCD_WriteReg(0x2C);
            break;
        case 5: LCD_WriteReg(0x36);
            LCD_WriteData(0x20);
            LCD_WriteReg(0x2C);
            break;
        case 6: LCD_WriteReg(0x36);
            LCD_WriteData(0x60);
            LCD_WriteReg(0x2C);
            break;
        case 7: LCD_WriteReg(0x36);
            LCD_WriteData(0xA0);
            LCD_WriteReg(0x2C);
            break;
        case 8: LCD_WriteReg(0x36);
            LCD_WriteData(0xE0);
            LCD_WriteReg(0x2C);
            break;
        default:break;
    }
}

void lcd_config() {
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOE, ENABLE);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = LCD_A0_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_A0_PORT, &GPIO_InitStruct);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = LCD_CSB_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_CSB_PORT, &GPIO_InitStruct);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = LCD_RES_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_RES_PORT, &GPIO_InitStruct);

    GPIO_WriteBit(LCD_RES_PORT, LCD_RES_PIN, Bit_SET);
    delayus(1000);
    GPIO_WriteBit(LCD_RES_PORT, LCD_RES_PIN, Bit_RESET);
    delayus(1000);
    GPIO_WriteBit(LCD_RES_PORT, LCD_RES_PIN, Bit_SET);
    delayus(120 * 1000);
    LCD_WriteReg(0x11);
    delayus(120 * 1000);

    LCD_WriteReg(0xB1);
    LCD_WriteData(0x05);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x3C);
    LCD_WriteReg(0xB2);
    LCD_WriteData(0x05);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x3C);
    LCD_WriteReg(0xB3);
    LCD_WriteData(0x05);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x05);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x3C);
    LCD_WriteReg(0xB4);
    LCD_WriteData(0x03);
    LCD_WriteReg(0xC0);
    LCD_WriteData(0x28);
    LCD_WriteData(0x08);
    LCD_WriteData(0x04);
    LCD_WriteReg(0xC1);
    LCD_WriteData(0XC0);
    LCD_WriteReg(0xC2);
    LCD_WriteData(0x0D);
    LCD_WriteData(0x00);
    LCD_WriteReg(0xC3);
    LCD_WriteData(0x8D);
    LCD_WriteData(0x2A);
    LCD_WriteReg(0xC4);
    LCD_WriteData(0x8D);
    LCD_WriteData(0xEE);
    LCD_WriteReg(0xC5);
    LCD_WriteData(0x1A);
    LCD_WriteReg(0x36);
    LCD_WriteData(0xC0);
    LCD_WriteReg(0xE0);
    LCD_WriteData(0x04);
    LCD_WriteData(0x22);
    LCD_WriteData(0x07);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x2E);
    LCD_WriteData(0x30);
    LCD_WriteData(0x25);
    LCD_WriteData(0x2A);
    LCD_WriteData(0x28);
    LCD_WriteData(0x26);
    LCD_WriteData(0x2E);
    LCD_WriteData(0x3A);
    LCD_WriteData(0x00);
    LCD_WriteData(0x01);
    LCD_WriteData(0x03);
    LCD_WriteData(0x13);
    LCD_WriteReg(0xE1);
    LCD_WriteData(0x04);
    LCD_WriteData(0x16);
    LCD_WriteData(0x06);
    LCD_WriteData(0x0D);
    LCD_WriteData(0x2D);
    LCD_WriteData(0x26);
    LCD_WriteData(0x23);
    LCD_WriteData(0x27);
    LCD_WriteData(0x27);
    LCD_WriteData(0x25);
    LCD_WriteData(0x2D);
    LCD_WriteData(0x3B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x01);
    LCD_WriteData(0x04);
    LCD_WriteData(0x13);
    LCD_WriteReg(0x3A);
    LCD_WriteData(0x05);
    LCD_WriteReg(0x29);
}
