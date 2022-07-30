/**************************************************************************/ /**
    \file     mag_calibrate.h
    \brief    mag_calibrate features function Header File
    \author   Lao·Zhu
    \version  V1.6.1
    \date     24. July 2022
 ******************************************************************************/

#ifndef MM32F3277_MATH_MAG_CALIBRATE_H_
#define MM32F3277_MATH_MAG_CALIBRATE_H_

typedef struct {
    unsigned char vflg;
    float offset[3];  //bias error
    float diag[3];    //scale-factor error
    float offdiag[3]; //cross-axis sensitivity
    float radius;     //sphere radius
} calpara_t;

#define COMPASS_CAL_NUM_SAMPLES 400

char CompassCal(float sample[3]);

#endif  // MM32F3277_MATH_MAG_CALIBRATE_H_
