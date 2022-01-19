/**************************************************************************/ /**
    \file     pin.h
    \brief    file that stores GPIO pin and port macros.
    \author   Lao·Zhu
    \version  V1.0.1
    \date     19. January 2022
 ******************************************************************************/

#ifndef MM32F3277_USER_PIN_H_
#define MM32F3277_USER_PIN_H_

#define LED1_PORT       GPIOF
#define LED1_PIN        GPIO_Pin_11
#define LED2_PORT       GPIOF
#define LED2_PIN        GPIO_Pin_12

#define LCD_RES_PORT    GPIOE
#define LCD_RES_PIN     GPIO_Pin_3
#define LCD_A0_PORT     GPIOE
#define LCD_A0_PIN      GPIO_Pin_4
#define LCD_CSB_PORT    GPIOE
#define LCD_CSB_PIN     GPIO_Pin_9

#define TOUCH_CS_PORT   GPIOE
#define TOUCH_CS_PIN    GPIO_Pin_1
#define TOUCH_PEN_PORT  GPIOE
#define TOUCH_PEN_PIN   GPIO_Pin_0

#endif // MM32F3277_USER_PIN_H_
