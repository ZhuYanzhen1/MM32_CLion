/**************************************************************************/ /**
    \file       fast_math.c
    \brief      fast math features function Header File
    \author     Lao·Zhu & ZGL
    \version    V1.6.1
    \date       24. July 2022
******************************************************************************/

#ifndef MM32F3277_MATH_FAST_MATH_H_
#define MM32F3277_MATH_FAST_MATH_H_

/* Take absolute value */
#define fast_absf(x) ((x < 0) ? -1.0f * x : x)

float atan_fast(float x);
float fast_sqrt(float value);
float my_acos(float x);
float my_asin(float x);
float my_pow(float x, int n);
float my_abs(float x);
float my_sinh(float x);
float my_cosh(float x);

#endif // MM32F3277_MATH_FAST_MATH_H_
