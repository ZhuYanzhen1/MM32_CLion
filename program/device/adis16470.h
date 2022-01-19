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
    int diag_star;
    int x_gyro;
    int y_gyro;
    int z_gyro;
    int x_acll;
    int y_acll;
    int z_acll;
    int temp;
    int data_cntr;
    int checknum;
} adis16470_t;

typedef struct {
    float angular_speed_x;  //角速度
    float angular_speed_y;
    float angular_speed_z;
    float acceleration_x;  //加速度
    float acceleration_y;
    float acceleration_z;
    float angle_x;  //角度
    float angle_y;
    float angle_z;
} gyroscope_t;
extern adis16470_t imu;
extern gyroscope_t gyroscope_data;

void adis16470_read_register(const unsigned char *addr_register, unsigned int *rx_point, unsigned char register_num);
void adis16470_write_register(unsigned char addr, unsigned char value);
void adis16470_single_handle(void);

#endif  // MM32F3277_DEVICE_ADIS16470_H_
