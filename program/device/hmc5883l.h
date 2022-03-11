/**************************************************************************/ /**
    \file     hmc5883l.h
    \brief    hmc5883l function Header File
    \author   ZGL
    \version  V1.0.1
    \date     22. February 2022
******************************************************************************/

#ifndef MM32F3277_DEVICE_HMC5883L_H_
#define MM32F3277_DEVICE_HMC5883L_H_

#define AVERAGE_NUM_TURE_NORTH  20
#define FACTOR_MAGNETOMETER_MGS 0.73f

typedef volatile struct {
    short x;
    short y;
    short z;
} hmc5883l;

typedef volatile struct {
    float x;
    float y;
    float z;
} hmc_correction;

extern hmc5883l magnetometer;
extern hmc_correction magnetometer_correction;

void hmc5883l_config();
void iic_read_hmc5883l();
unsigned char iic_read_hmc5883l_verification();
void hmc5883l_correction();
float initial_angle_begin();

#endif // MAIN_C_DEVICE_HMC5883L_H_
