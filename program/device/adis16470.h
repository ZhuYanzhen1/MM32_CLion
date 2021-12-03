/**************************************************************************/ /**
    \file     adis16470.h
    \brief    ADIS16470 function Header File
    \author   ZGL
    \version  V1.0.1
    \date     23. November 2021
******************************************************************************/

#ifndef MM32F3277_DEVICE_ADIS16470_H
#define MM32F3277_DEVICE_ADIS16470_H

#include "mm32_device.h"

typedef struct
{
    volatile int16_t DIAG_STAT;
    volatile int16_t X_GYRO;
    volatile int16_t Y_GYRO;
    volatile int16_t Z_GYRO;
    volatile int16_t X_ACCL;
    volatile int16_t Y_ACCL;
    volatile int16_t Z_ACCL;
    volatile int16_t TEMP;
    volatile int16_t DATA_CNTR;
    volatile int16_t Checknum;
} adis_t;
typedef struct
{
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
extern adis_t imu;
extern gyroscope_t gyroscope_data;

void adis_read_register(const uint8_t *addr_register, uint16_t *rx_point, uint8_t register_num);
void adis_write_register(uint8_t addr, uint8_t value);
void adis_single_handle(void);
void Self_Calibration(void);

#endif  // MM32F3277_DEVICE_ADIS16470_H
