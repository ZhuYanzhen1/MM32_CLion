/**************************************************************************/ /**
    \file     delay.h
    \brief    delay function Header File
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

#ifndef MM32F3277_SYSTEM_DELAY_H_
#define MM32F3277_SYSTEM_DELAY_H_

void delay_decrease(void);
void delay_config(void);
void delayms(unsigned int xms);
void delayus(unsigned int xus);

extern unsigned int global_time_stamp;

#endif  // MM32F3277_SYSTEM_DELAY_H_
