/**************************************************************************/ /**
    \file     hmc5883l.h
    \brief    hmc5883l function Header File
    \author   ZGL
    \version  V1.0.1
    \date     22. February 2022
******************************************************************************/

#ifndef MAIN_C_DEVICE_HMC5883L_H_
#define MAIN_C_DEVICE_HMC5883L_H_

typedef volatile struct {
    short x;
    short y;
    short z;
} hmc5883l;

void hmc5883l_config();
void iic_read_hmc5883l();
void iic_read_hmc5883l_verification();

#endif // MAIN_C_DEVICE_HMC5883L_H_
