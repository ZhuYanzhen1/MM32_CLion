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

void gpio3_config() {
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void xpt2046_gpio_config(void) {
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
