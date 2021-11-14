//
// Created by Lao·Zhu on 2021/11/14.
//

#ifndef MM32F3277_MATH_FFT_H_
#define MM32F3277_MATH_FFT_H_

typedef struct {
    float r;
    float i;
} complex_t;

void fft_init(complex_t *w, unsigned int k);
void fft(complex_t *dat, const complex_t *w, unsigned int k);

#endif //MM32F3277_MATH_FFT_H_
