/**************************************************************************/ /**
    \file     hmc5883l.c
    \brief    hmc5883l function Source File
    \author   ZGL
    \version  V1.0.1
    \date     22. February 2022
******************************************************************************/

#include "hmc5883l.h"
#include "iic.h"
#include "delay.h"

hmc5883l Magnetometer = {0};

void hmc5883l_config() {
    iic_start();
    iic_send_byte(0x3c);
    iic_send_byte(0x00);
    iic_send_byte(0x14);  //输出速率75hz,MAX 0x78
//    iic_stop();
    delayms(2);
    iic_start();
    iic_send_byte(0x3c); //写指令
    iic_send_byte(0x01);
    iic_send_byte(0x00); //测量范围
//    iic_stop();
    delayms(2);
    iic_start();
    iic_send_byte(0x3c); //写指令
    iic_send_byte(0x02);
    iic_send_byte(0x00); //连续测量模式
    iic_stop();
}

hmc5883l magnetometer = {0};

void iic_read_hmc5883l() {
    short xyz_data[6] = {0};

    iic_start();
    iic_send_byte(0x3c);// 0x3c是写信号 ；ox3d是读信号
    iic_send_byte(0x03);// 地址位
//    iic_stop();

    delayus(100);

    iic_start();
    iic_send_byte(0x3d);
    for (unsigned char i = 0; i < 6; i++) {
        xyz_data[i] = iic_read_byte();
        if (i == 5) {
            iic_not_ack();
        } else {
            iic_ack();
        }

    }
    iic_stop();

    /* The read order is xzy */
    magnetometer.x = (short) ((xyz_data[0] << 8) | xyz_data[1]);

    magnetometer.y = (short) ((xyz_data[4] << 8) | xyz_data[5]);
    magnetometer.z = (short) ((xyz_data[2] << 8) | xyz_data[3]);
}
