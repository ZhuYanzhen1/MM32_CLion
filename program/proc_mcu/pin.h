/**************************************************************************/ /**
    \file     pin.h
    \brief    file that stores GPIO pin and port macros.
    \author   Lao·Zhu
    \version  V1.0.1
    \date     19. January 2022
 ******************************************************************************/

#ifndef MM32F3277_USER_PIN_H_
#define MM32F3277_USER_PIN_H_

#define LED1_PORT       GPIOA
#define LED1_PIN        GPIO_Pin_3

#define LCD_RES_PORT    GPIOF
#define LCD_RES_PIN     GPIO_Pin_0
#define LCD_A0_PORT     GPIOE
#define LCD_A0_PIN      GPIO_Pin_4

#define TOUCH_CS_PORT   GPIOE
#define TOUCH_CS_PIN    GPIO_Pin_0
#define TOUCH_PEN_PORT  GPIOE
#define TOUCH_PEN_PIN   GPIO_Pin_1

#define IIC_SDA_PORT    GPIOD
#define IIC_SDA_PIN     GPIO_Pin_11
#define IIC_SCL_PORT    GPIOD
#define IIC_SCL_PIN     GPIO_Pin_10

#define SPI3_SCK_PIN    GPIO_Pin_4
#define SPI3_SCK_PORT   GPIOD
#define SPI3_MISO_PIN   GPIO_Pin_5
#define SPI3_MISO_PORT  GPIOD
#define SPI3_MOSI_PIN   GPIO_Pin_6
#define SPI3_MOSI_PORT  GPIOD
#define IMU_DR_PIN      GPIO_Pin_10
#define IMU_DR_PORT     GPIOG

#endif // MM32F3277_USER_PIN_H_
