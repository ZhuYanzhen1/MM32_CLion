#include "main.h"
#include "delay.h"
#include "led.h"
#include "uart.h"

int main(void) {
    delay_config();
    led_config();
    UART1_NVIC_Init(115200);
    while (1) {
        LED1_TOGGLE();
        delayms(500);
    }
}
