/**************************************************************************/ /**
    \file     led.c
    \brief    LED function Source File
    \author   Lao·Zhu
    \version  V1.2.2
    \date     14. November 2021
 ******************************************************************************/

#include "led.h"
#include "hal_conf.h"
#include "mm32_device.h"
#include "pin.h"

void led_config(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = LED1_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED1_PORT, &GPIO_InitStruct);

    LED1_OFF();
}
