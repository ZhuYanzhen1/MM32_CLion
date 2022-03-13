/**************************************************************************/ /**
    \file     adis16470.h
    \brief    ADIS16470 function Header File
    \author   ZGL
    \version  V1.3.2
    \date     06. March 2021
******************************************************************************/

#ifndef MM32F3277_DEVICE_ADIS16470_H_
#define MM32F3277_DEVICE_ADIS16470_H_

#define DR_HIGH     GPIO_ReadInputDataBit(IMU_DR_PORT, IMU_DR_PIN)

typedef volatile struct {
    short diag_star;
    short x_gyro;
    short y_gyro;
    short z_gyro;
    short x_acll;
    short y_acll;
    short z_acll;
    short temp_out;
    short data_cntr;
    short checknum;
} adis16470_t;

extern adis16470_t imu;

short adis_read_uid();
short adis_read_register(unsigned int register_address);
void adis_burst_read();
void adis_point_wz(void);
void calibration_acll();

#endif  // MM32F3277_DEVICE_ADIS16470_H_
