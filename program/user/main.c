/**************************************************************************//**
  \file     main.c
  \brief    main function Source File
  \author   Lao·Zhu
  \version  V1.0.1
  \date     14. November 2021
 ******************************************************************************/

#include "main.h"

int main(void) {
    unsigned char counter = 0;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    delay_config();
    led_config();
    uart1_config(115200);
    while (1) {
        counter++;
        printf("Hello MM32 with printf! counter: %d\r\n", counter);
        LED1_TOGGLE();
        delayms(500);
    }
}