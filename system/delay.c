#include "delay.h"

extern unsigned int SystemCoreClock;
static volatile unsigned int TimingDelay;

void delay_config(void) {
    if (SysTick_Config(SystemCoreClock / 1000))
        while (1);
    NVIC_SetPriority(SysTick_IRQn, 0x0);
}

static void TimingDelayDecrement(void) {
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}

void SysTick_Handler(void) {
    TimingDelayDecrement();
}

void delayms(unsigned int xms) {
    TimingDelay = xms;
    while (TimingDelay != 0);
}
