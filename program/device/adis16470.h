/**************************************************************************/ /**
    \file     adis16470.h
    \brief    ADIS16470 function Header File
    \author   ZGL
    \version  V1.2.2
    \date     23. November 2021
******************************************************************************/

#ifndef MM32F3277_DEVICE_ADIS16470_H_
#define MM32F3277_DEVICE_ADIS16470_H_

typedef volatile struct {
    short diag_star;
    short x_gyro;
    short y_gyro;
    short z_gyro;
    short x_acll;
    short y_acll;
    short z_acll;
    short temp;
    short data_cntr;
    short checknum;
} adis16470_t;

extern adis16470_t imu;

unsigned short adis_read_prod_id(void);
void adis16470_read_register(const unsigned char *addr_register, unsigned int *rx_point, unsigned char register_num);
void adis16470_write_register(unsigned char addr, unsigned char value);
void adis_burst_read();

#endif  // MM32F3277_DEVICE_ADIS16470_H_
