/**************************************************************************/ /**
    \file     led.h
    \brief    LED function Header File
    \author   Lao·Zhu
    \version  V1.2.2
    \date     14. November 2021
 ******************************************************************************/

#ifndef MM32F3277_HARDWARE_LED_H_
#define MM32F3277_HARDWARE_LED_H_

#define LED1_ON() GPIO_ResetBits(LED1_PORT, LED1_PIN)
#define LED1_OFF() GPIO_SetBits(LED1_PORT, LED1_PIN)
#define LED1_TOGGLE() (GPIO_ReadOutputDataBit(LED1_PORT, LED1_PIN)) \
                            ? (GPIO_ResetBits(LED1_PORT, LED1_PIN)) : (GPIO_SetBits(LED1_PORT, LED1_PIN))

#define LED2_ON() GPIO_ResetBits(LED2_PORT, LED2_PIN)
#define LED2_OFF() GPIO_SetBits(LED2_PORT, LED2_PIN)
#define LED2_TOGGLE() (GPIO_ReadOutputDataBit(LED2_PORT, LED2_PIN)) \
                            ? (GPIO_ResetBits(LED2_PORT, LED2_PIN)) : (GPIO_SetBits(LED2_PORT, LED2_PIN))

void led_config(void);

#endif  // MM32F3277_HARDWARE_LED_H_
