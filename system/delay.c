#include "delay.h"

extern u32 SystemCoreClock;
static __IO u32 TimingDelay;

void DELAY_Init(void) {
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while (1);
    }
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

void DELAY_Ms(__IO u32 nTime) {
    TimingDelay = nTime;
    while (TimingDelay != 0);
}
