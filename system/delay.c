#include "delay.h"

extern unsigned int SystemCoreClock;
static volatile unsigned int delayms_counter = 0;

void delay_config(void) {
    if (SysTick_Config(SystemCoreClock / 1000))
        while (1);
    NVIC_SetPriority(SysTick_IRQn, 0x0);
}

static void TimingDelayDecrement(void) {
    if (delayms_counter != 0x00) {
        delayms_counter--;
    }
}

void SysTick_Handler(void) {
    TimingDelayDecrement();
}

void delayms(unsigned int xms) {
    delayms_counter = xms;
    while (delayms_counter != 0);
}
