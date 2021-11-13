#include "main.h"
#include "delay.h"
#include "led.h"
#include "uart_txrx_interrupt.h"

s32 main(void) {
    DELAY_Init();
    LED_Init();
    UART1_NVIC_Init(115200);
    while (1) {
        LED1_ON();
        LED2_OFF();
        DELAY_Ms(100);
        LED1_OFF();
        LED2_ON();
        DELAY_Ms(100);
        //UART1_RxTx_Transceiving();
    }
}
