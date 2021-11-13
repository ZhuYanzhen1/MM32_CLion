/**************************************************************************//**
  \file     main.c
  \brief    main function Source File
  \author   Lao·Zhu
  \version  V1.0.1
  \date     14. November 2021
 ******************************************************************************/

#include "main.h"

int main(void) {
    delay_config();
    led_config();
    UART1_NVIC_Init(115200);
    while (1) {
        LED1_TOGGLE();
        delayms(500);
    }
}
