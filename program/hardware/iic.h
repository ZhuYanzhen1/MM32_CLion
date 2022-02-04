//
// Created by Lao·Zhu on 2022/2/4.
//

#ifndef MAIN_C_HARDWARE_IIC_H_
#define MAIN_C_HARDWARE_IIC_H_

void iic1_writebyte(unsigned char dat);
void iic1_wait_for_stop(void);
void iic1_set_slave_addr(unsigned char deviceaddr);
void iic1_config(unsigned int iic_speed);

#endif //MAIN_C_HARDWARE_IIC_H_
