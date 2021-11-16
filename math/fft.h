/**************************************************************************//**
  \file     fft.h
  \brief    fft function Header File
  \author   Lao·Zhu
  \version  V1.0.1
  \date     17. November 2021
 ******************************************************************************/

#ifndef MM32F3277_MATH_FFT_H_
#define MM32F3277_MATH_FFT_H_

typedef struct {
    float r;
    float i;
} complex_t;

void math_fft_init(complex_t *w, unsigned int k);
void math_fft(complex_t *dat, const complex_t *w, unsigned int k);

#endif //MM32F3277_MATH_FFT_H_
