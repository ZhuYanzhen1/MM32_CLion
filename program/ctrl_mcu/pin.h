/**************************************************************************/ /**
    \file     pin.h
    \brief    file that stores GPIO pin and port macros.
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

#ifndef MM32F3277_USER_PIN_H_
#define MM32F3277_USER_PIN_H_

#define LED1_PORT       GPIOA
#define LED1_PIN        GPIO_Pin_2

#define CH372_CS_PORT   GPIOC
#define CH372_CS_PIN    GPIO_Pin_0
#define CH372_A0_PORT   GPIOF
#define CH372_A0_PIN    GPIO_Pin_2
#define CH372_RD_PORT   GPIOF
#define CH372_RD_PIN    GPIO_Pin_1
#define CH372_WR_PORT   GPIOF
#define CH372_WR_PIN    GPIO_Pin_0

#endif // MM32F3277_USER_PIN_H_
