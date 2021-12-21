/**************************************************************************/ /**
    \file     led.h
    \brief    LED function Header File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#ifndef MM32F3277_HARDWARE_LED_H_
#define MM32F3277_HARDWARE_LED_H_

#define LED1_Port GPIOF
#define LED1_Pin GPIO_Pin_11
#define LED2_Port GPIOF
#define LED2_Pin GPIO_Pin_12

#define LED1_ON() GPIO_ResetBits(LED1_Port, LED1_Pin)
#define LED1_OFF() GPIO_SetBits(LED1_Port, LED1_Pin)
#define LED1_TOGGLE() (GPIO_ReadOutputDataBit(LED1_Port, LED1_Pin)) ? (GPIO_ResetBits(LED1_Port, LED1_Pin)) : (GPIO_SetBits(LED1_Port, LED1_Pin))

#define LED2_ON() GPIO_ResetBits(LED2_Port, LED2_Pin)
#define LED2_OFF() GPIO_SetBits(LED2_Port, LED2_Pin)
#define LED2_TOGGLE() (GPIO_ReadOutputDataBit(LED2_Port, LED2_Pin)) ? (GPIO_ResetBits(LED2_Port, LED2_Pin)) : (GPIO_SetBits(LED2_Port, LED2_Pin))

void led_config(void);

#endif  // MM32F3277_HARDWARE_LED_H_
