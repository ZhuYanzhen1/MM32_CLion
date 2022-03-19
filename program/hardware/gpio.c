/**************************************************************************/ /**
    \file       gpio.c
    \brief      GPIO function Source File
    \author     LaoZhu
    \version    V1.2.2
    \date       19. January 2022
******************************************************************************/

#include "gpio.h"
#include "config.h"
#include "hal_conf.h"
#include "mm32_device.h"
#include "pin.h"

#ifdef IS_PROCESS_MCU
void lcd_gpio_config(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOE, ENABLE);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = LCD_A0_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_A0_PORT, &GPIO_InitStruct);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = LCD_RES_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_RES_PORT, &GPIO_InitStruct);
}

void xpt2046_gpio_config(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

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

    GPIO_SetBits(TOUCH_PEN_PORT, TOUCH_PEN_PIN);
    GPIO_SetBits(TOUCH_CS_PORT, TOUCH_CS_PIN);
}
#endif
