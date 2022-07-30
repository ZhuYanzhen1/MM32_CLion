/**************************************************************************/ /**
    \file       filter.c
    \brief      Includes some filtering algorithms
    \author     Lao·Zhu & ZGL
    \version    V1.6.1
    \date       24. July 2022
******************************************************************************/

#include "filter.h"
#include "qfplib.h"
#include "fast_math.h"

#define M_PI    3.1415926f

void create_che_low_pass_filter(float epsilon, float sampling_frequency,
                                float half_power_frequency, CHELowPass *filter) {
    filter->m = 4 / 2;

    for (int counter = 0; counter < 2; ++counter) {
        filter->w0[counter] = 0;
        filter->w1[counter] = 0;
        filter->w2[counter] = 0;
    }

    float a = qfp_ftan(M_PI * half_power_frequency / sampling_frequency);
    float a2 = a * a;

    float u = qfp_fln((1.0f + qfp_fsqrt(1.0f + epsilon * epsilon)) / epsilon);
    float su = my_sinh(u / 4.0f);
    float cu = my_cosh(u / 4.0f);
    float b, c;

    int i;
    for (i = 0; i < filter->m; ++i) {
        b = qfp_fsin(M_PI * (2.0 * i + 1.0) / (2.0 * 4.0f)) * su;
        c = qfp_fcos(M_PI * (2.0 * i + 1.0) / (2.0 * 4.0f)) * cu;
        c = b * b + c * c;
        sampling_frequency = a2 * c + 2.0f * a * b + 1.0f;
        filter->A[i] = a2 / (4.0f * sampling_frequency);
        filter->d1[i] = 2.0f * (1.0f - a2 * c) / sampling_frequency;
        filter->d2[i] = -(a2 * c - 2.0f * a * b + 1.0f) / sampling_frequency;
    }
    filter->ep = 2.0f / epsilon;
}

float che_low_pass(CHELowPass *filter, float x) {
    for (unsigned char i = 0; i < filter->m; ++i) {
        filter->w0[i] = filter->d1[i] * filter->w1[i] + filter->d2[i] * filter->w2[i] + x;
        x = filter->A[i] * (filter->w0[i] + 2.0f * filter->w1[i] + filter->w2[i]);
        filter->w2[i] = filter->w1[i];
        filter->w1[i] = filter->w0[i];
    }
    return x * filter->ep;
}

float rc_low_pass(float input, float last_output, float a) {
    return a * input + (1 - a) * last_output;
}
