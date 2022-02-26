//
// Created by 16625 on 2022-02-25.
//

#ifndef MM32F3277_MATH_AHRS_H_
#define MM32F3277_MATH_AHRS_H_

#define ARC_TO_ANGLE 57.3f

void MadgwickAHRSupdate();
void MadgwickAHRSupdateIMU();
float invSqrt(float x);
float my_asinf(float x);

#endif //MAIN_C_MATH_AHRS_H_


