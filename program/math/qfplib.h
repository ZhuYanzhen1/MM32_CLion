/**************************************************************************/ /**
    \file     qfplib.h
    \brief    quick float point math function Source File
    \author   Mark Owen
    \version  V1.0.1
    \date     14. November 2016
 ******************************************************************************/

#ifndef MM32F3277_MATH_QFPLIB_H_
#define MM32F3277_MATH_QFPLIB_H_

extern float qfp_fadd(float x, float y);
extern float qfp_fsub(float x, float y);
extern float qfp_fmul(float x, float y);
extern float qfp_fdiv(float x, float y);
extern float qfp_fsqrt(float x);
extern float qfp_fexp(float x);
extern float qfp_fln(float x);
extern float qfp_fsin(float x);
extern float qfp_fcos(float x);
extern float qfp_ftan(float x);
extern float qfp_fatan2(float y, float x);

#endif  // MM32F3277_MATH_QFPLIB_H_
