#include "delay.h"
#include "config.h"

extern unsigned int SystemCoreClock;
static volatile unsigned int delayms_counter = 0;
static unsigned int delay_us_factor = 0;
static unsigned int delay_ms_factor = 0;

void delay_config(void) {
    delay_us_factor = SystemCoreClock / 1000000;
    delay_ms_factor = 1000 / configTICK_RATE_HZ;
    if (SysTick_Config(SystemCoreClock / configTICK_RATE_HZ))
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

void delayus(unsigned int xus) {
    unsigned int Ticks, Time_Old, Time_Now, Time_Count = 0;
    unsigned int Reload = SysTick->LOAD;
    Ticks = xus * delay_us_factor;
    Time_Old = SysTick->VAL;
    while (1) {
        Time_Now = SysTick->VAL;
        if (Time_Now != Time_Old) {
            if (Time_Now < Time_Old)
                Time_Count += Time_Old - Time_Now;
            else
                Time_Count += Reload - Time_Now + Time_Old;
            Time_Old = Time_Now;
            if (Time_Count >= Ticks)
                break;
        }
    }
}

void delayms(unsigned int xms) {
#if USE_FREERTOS == 1
    if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) {
        if (xms >= delay_ms_factor) {
            vTaskDelay(xms / delay_ms_factor);
        } else {
            xms %= delay_ms_factor;
            delayus(xms * 1000);
        }
    } else
        delayus(xms * 1000);
#else
    delayms_counter = xms;
    while (delayms_counter != 0);
#endif
}
