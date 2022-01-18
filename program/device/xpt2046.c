//
// Created by Lao·Zhu on 2021/12/22.
//

#include "xpt2046.h"
#include "hal_conf.h"
#include "mm32_device.h"
#include "pin.h"
#include "spi.h"
#include "delay.h"
#include "gui_base.h"
#include "qfplib.h"
#include "config.h"

static short xoffset = 0, yoffset = 0;
static float xfactor = 0, yfactor = 0;

unsigned short xpt2046_read(unsigned short cmd) {
    unsigned char i, j;
    unsigned short tmp, value[TOUCH_READ_TIMES] = {0};
    unsigned long total_value = 0;

    for (i = 0; i < TOUCH_READ_TIMES; i++) {
        GPIO_WriteBit(TOUCH_CS_PORT, TOUCH_CS_PIN, Bit_RESET);
        spi2_readwrite_byte(cmd);
        value[i] = spi2_readwrite_byte(TOUCH_Continue_Read) << 8;
        value[i] |= spi2_readwrite_byte(TOUCH_Continue_Read);
        value[i] >>= 3;
        GPIO_WriteBit(TOUCH_CS_PORT, TOUCH_CS_PIN, Bit_SET);
    }

    for (i = 0; i < TOUCH_READ_TIMES; i++) {
        for (j = i + 1; j < TOUCH_READ_TIMES; j++) {
            if (value[i] < value[j]) {
                tmp = value[i];
                value[i] = value[j];
                value[j] = tmp;
            }
        }
    }

    for (i = 1; i < TOUCH_READ_TIMES - 1; i++)
        total_value += value[i];
    total_value /= (TOUCH_READ_TIMES - 2);

    return total_value;
}

unsigned char xpt2046_readxy(unsigned short *x, unsigned short *y) {
    unsigned short valueX1, valueY1, valueX2, valueY2;

    MODIFY_REG(SPI2->BRR, BRR_Mask, SPI_BaudRatePrescaler_256);
    delayms(1);
    valueX1 = xpt2046_read(TOUCH_X_CMD);
    valueY1 = xpt2046_read(TOUCH_Y_CMD);
    valueX2 = xpt2046_read(TOUCH_X_CMD);
    valueY2 = xpt2046_read(TOUCH_Y_CMD);
    delayms(1);
    MODIFY_REG(SPI2->BRR, BRR_Mask, SPI_BaudRatePrescaler_16);

    *x = valueX1 > valueX2 ? (valueX1 - valueX2) : (valueX2 - valueX1);
    *y = valueY1 > valueY2 ? (valueY1 - valueY2) : (valueY2 - valueY1);
    if ((*x > TOUCH_ERR_RANGE) || (*y > TOUCH_ERR_RANGE))
        return 0xFF;

    *x = (valueX1 + valueX2) / 2;
    *y = (valueY1 + valueY2) / 2;

    if ((*x < TOUCH_X_MIN || *x > TOUCH_X_MAX) ||
        (*y < TOUCH_Y_MIN || *y > TOUCH_Y_MAX))
        return 0xFF;
    return 0;
}

void gui_draw_sign_for_calibrate(unsigned char x, unsigned char y) {
    gui_draw_hline(x - 5, y - 1, 10, C_RED);
    gui_draw_hline(x - 5, y, 10, C_RED);
    gui_draw_hline(x - 5, y + 1, 10, C_RED);
    gui_draw_vline(x - 1, y - 5, 10, C_RED);
    gui_draw_vline(x, y - 5, 10, C_RED);
    gui_draw_vline(x + 1, y - 5, 10, C_RED);
}

void xpt2046_calibrate_single_point(unsigned short x,
                                    unsigned short y,
                                    unsigned short *valueX,
                                    unsigned short *valueY) {
    unsigned char i = 0;
    delayms(1);
    gui_clear_screan(C_BLACK);
    gui_draw_sign_for_calibrate(x, y);
    delayms(1);
    while (1) {
        if (xpt2046_readxy(valueX, valueY) != 0xFF) {
            i++;
            if (i > 10)
                break;
        }
    }
}

void xpt2046_calibrate(void) {
    unsigned short px[2], py[2], valueX[4], valueY[4];

    xpt2046_calibrate_single_point(LCD_CALx_MIN, LCD_CALy_MIN, &valueX[0], &valueY[0]);
    delayms(500);
    xpt2046_calibrate_single_point(LCD_CALx_MIN, LCD_CALy_MAX, &valueX[1], &valueY[1]);
    delayms(500);
    xpt2046_calibrate_single_point(LCD_CALx_MAX, LCD_CALy_MIN, &valueX[2], &valueY[2]);
    delayms(500);
    xpt2046_calibrate_single_point(LCD_CALx_MAX, LCD_CALy_MAX, &valueX[3], &valueY[3]);
    delayms(500);

    px[0] = (valueX[0] + valueX[1]) / 2;
    py[0] = (valueY[0] + valueY[2]) / 2;
    px[1] = (valueX[3] + valueX[2]) / 2;
    py[1] = (valueY[3] + valueY[1]) / 2;

    xfactor = (float) LCD_CAL_X / (float) (px[1] - px[0]);
    yfactor = (float) LCD_CAL_Y / (float) (py[1] - py[0]);
    xoffset = (short) ((float) LCD_CALx_MAX - ((float) px[1] * xfactor));
    yoffset = (short) ((float) LCD_CALy_MAX - ((float) py[1] * yfactor));

    delayms(500);
    gui_clear_screan(C_WHITE);
    gui_printf(5, 10, C_BLACK, C_WHITE, "xoffset %d", xoffset);
    gui_printf(5, 20, C_BLACK, C_WHITE, "yoffset %d", yoffset);
    gui_printf(5, 30, C_BLACK, C_WHITE, "xFactor %.5f", xfactor);
    gui_printf(5, 40, C_BLACK, C_WHITE, "yFactor %.5f", yfactor);
}

unsigned char xpt2046_scan(unsigned char *x_pos, unsigned char *y_pos) {
    unsigned short valueX, valueY;
    if (xpt2046_readxy(&valueX, &valueY) == 0xFF)
        return 0xFF;
    *x_pos = (char) qfp_fmul(valueX, xfactor) + xoffset;
    *y_pos = (char) qfp_fmul(valueY, yfactor) + xoffset;
    if ((*x_pos > 128) || (*y_pos > 160))
        return 0xFE;
    return 0;
}

void xpt2046_config(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFG, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOE, ENABLE);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = TOUCH_CS_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(TOUCH_CS_PORT, &GPIO_InitStruct);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = TOUCH_PEN_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(TOUCH_PEN_PORT, &GPIO_InitStruct);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = EXTI0_PRIORITY;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    GPIO_SetBits(TOUCH_CS_PORT, TOUCH_CS_PIN);
    GPIO_SetBits(TOUCH_PEN_PORT, TOUCH_PEN_PIN);
}
