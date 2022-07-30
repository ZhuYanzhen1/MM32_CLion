/**************************************************************************/ /**
    \file     led.h
    \brief    LED function Header File
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

#ifndef MM32F3277_HARDWARE_LED_H_
#define MM32F3277_HARDWARE_LED_H_

#define LED1_ON()       GPIO_ResetBits(LED1_PORT, LED1_PIN)
#define LED1_OFF()      GPIO_SetBits(LED1_PORT, LED1_PIN)
#define LED1_TOGGLE()   (GPIO_ReadOutputDataBit(LED1_PORT, LED1_PIN)) \
                            ? (GPIO_ResetBits(LED1_PORT, LED1_PIN)) : (GPIO_SetBits(LED1_PORT, LED1_PIN))

void led_config(void);

#endif  // MM32F3277_HARDWARE_LED_H_
