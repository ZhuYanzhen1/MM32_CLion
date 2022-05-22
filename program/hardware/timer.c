/**************************************************************************/ /**
    \file     timer.c
    \brief    TIM function Source File
    \author   Lao·Zhu
    \version  V1.2.2
    \date     19. January 2022
******************************************************************************/

#include "timer.h"
#include "hal_conf.h"
#include "mm32_device.h"
#ifdef IS_CONTROL_MCU
#include "../ctrl_mcu/config.h"
#else
#include "../proc_mcu/config.h"
#endif  // IS_CONTROL_MCU

void timer3_config(void) {  // PB4
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_2);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 1999;    // 100k / 2000 = 50Hz
    TIM_TimeBaseStructure.TIM_Prescaler = 1199;    // 120M / 1200 = 100kHz
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

    TIM_OCInitStructure.TIM_Pulse = 150;        // 100 = 1ms
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_CtrlPWMOutputs(TIM3, ENABLE);

    TIM_Cmd(TIM3, ENABLE);
    WRITE_REG(TIM3->CCR1, 150);
}

void timer2_config() {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    NVIC_InitTypeDef NVIC_StructInit;
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2, ENABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);

    /* 25Hz */
    TIM_TimeBaseStruct.TIM_Period = 399;            // 400 - 1
    TIM_TimeBaseStruct.TIM_Prescaler = 11999;
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

    NVIC_StructInit.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_StructInit.NVIC_IRQChannelPreemptionPriority = TIM2_PRIORITY;
    NVIC_StructInit.NVIC_IRQChannelSubPriority = 0;
    NVIC_StructInit.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_StructInit);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

#ifdef IS_CONTROL_MCU

void timer4_config() {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    NVIC_InitTypeDef NVIC_StructInit;
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM4, ENABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);

    /* 50Hz */
    TIM_TimeBaseStruct.TIM_Period = 399;            // 200 - 1
    TIM_TimeBaseStruct.TIM_Prescaler = 11999;
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct);

    NVIC_StructInit.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_StructInit.NVIC_IRQChannelPreemptionPriority = TIM4_PRIORITY;
    NVIC_StructInit.NVIC_IRQChannelSubPriority = 0;
    NVIC_StructInit.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_StructInit);

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
}

#endif
