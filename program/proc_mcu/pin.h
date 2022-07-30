/**************************************************************************/ /**
    \file     pin.h
    \brief    file that stores GPIO pin and port macros.
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

#ifndef MM32F3277_USER_PIN_H_
#define MM32F3277_USER_PIN_H_

/*! \brief The port where LED1 located */
#define LED1_PORT       GPIOA
/*! \brief The pin where LED1 located */
#define LED1_PIN        GPIO_Pin_4

/*! \brief The port where LCD reset pin located */
#define LCD_RES_PORT    GPIOE
/*! \brief The pin where LCD reset pin located */
#define LCD_RES_PIN     GPIO_Pin_0
/*! \brief The port where LCD reset pin located */
#define LCD_A0_PORT     GPIOE
/*! \brief The pin where LCD command pin located */
#define LCD_A0_PIN      GPIO_Pin_1

/*! \brief The port where touch sensor cs pin located */
#define TOUCH_CS_PORT   GPIOE
/*! \brief The pin where touch sensor cs pin located */
#define TOUCH_CS_PIN    GPIO_Pin_3
/*! \brief The port where touch sensor interrupt pin located */
#define TOUCH_PEN_PORT  GPIOE
/*! \brief The pin where touch sensor interrupt pin located */
#define TOUCH_PEN_PIN   GPIO_Pin_4

#endif // MM32F3277_USER_PIN_H_
