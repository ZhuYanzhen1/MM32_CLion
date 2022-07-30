/**************************************************************************/ /**
    \file       iic.h
    \brief      iic function Header File
    \author     ZGL & LaoZhu
    \version    V1.6.1
    \date       24. July 2022
******************************************************************************/

#ifndef MM32F3277_HARDWARE_IIC_H_
#define MM32F3277_HARDWARE_IIC_H_

void iic1_writebyte(unsigned char dat);
void iic1_wait_for_stop(void);
void iic1_set_slave_addr(unsigned char deviceaddr);
void iic1_config(void);

#endif //MM32F3277_HARDWARE_IIC_H_
