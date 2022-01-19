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
#include "config.h"

void timer2_config() {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    NVIC_InitTypeDef NVIC_StructInit;
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2, ENABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);

    /* 50Hz */
    TIM_TimeBaseStruct.TIM_Period = 199;
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
