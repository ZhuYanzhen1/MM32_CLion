/**************************************************************************/ /**
    \file       gpio.h
    \brief      GPIO function Header File
    \author     LaoZhu
    \version    V1.2.2
    \date       19. January 2022
******************************************************************************/

#ifndef MM32F3277_HARDWARE_GPIO_H_
#define MM32F3277_HARDWARE_GPIO_H_

void lcd_gpio_config(void);
void xpt2046_gpio_config(void);
void ch372c_gpio_input(void);
void ch372c_gpio_output(void);
void ch372c_gpio_config(void);
void tim3_pwm_gpio_config(void);

#endif // MM32F3277_HARDWARE_GPIO_H_
