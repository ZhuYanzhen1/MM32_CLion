/**************************************************************************/ /**
    \file     delay.h
    \brief    delay function Header File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     14. November 2021
 ******************************************************************************/

#ifndef MM32F3277_SYSTEM_USER_H_
#define MM32F3277_SYSTEM_USER_H_

void delay_decrease(void);
void delay_config(void);
void delayms(unsigned int xms);
void delayus(unsigned int xus);

#endif  // MM32F3277_SYSTEM_USER_H_
