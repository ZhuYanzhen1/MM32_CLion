/**************************************************************************/ /**
    \file     mag_calibrate.c
    \brief    mag_calibrate features function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     21. March 2022
 ******************************************************************************/

typedef struct {
    unsigned char vflg;
    float offset[3];  //bias error
    float diag[3];    //scale-factor error
    float offdiag[3]; //cross-axis sensitivity
    float radius;     //sphere radius
} calpara_t;

#define COMPASS_CAL_NUM_SAMPLES 500   //总采样数

char CompassCal(float sample[3]);
