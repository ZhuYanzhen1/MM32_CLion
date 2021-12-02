/**************************************************************************/ /**
    \file     fft.c
    \brief    FFT function Source File
    \author   Lao·Zhu
    \version  V1.0.1
    \date     17. November 2021
 ******************************************************************************/

#include "fft.h"
#include "qfplib.h"

static void math_fft_butterfly(complex_t *a, complex_t *b, const complex_t *c) {
    complex_t bc;
    bc.r = b->r * c->r - b->i * c->i;
    bc.i = b->r * c->i + b->i * c->r;
    b->r = a->r - bc.r;
    b->i = a->i - bc.i;
    a->r += bc.r;
    a->i += bc.i;
}

static unsigned int math_fft_bits_reverse(unsigned int index, unsigned int bits) {
    unsigned int left, right;
    left = index << 16;
    right = index >> 16;
    index = left | right;
    left = (index << 8) & 0xff00ff00;
    right = (index >> 8) & 0x00ff00ff;
    index = left | right;
    left = (index << 4) & 0xf0f0f0f0;
    right = (index >> 4) & 0x0f0f0f0f;
    index = left | right;
    left = (index << 2) & 0xc3c3c3c3;
    right = (index >> 2) & 0x3c3c3c3c;
    index = left | right;
    left = (index << 1) & 0xa5a5a5a5;
    right = (index >> 1) & 0x5a5a5a5a;
    index = left | right;
    return index >> (32 - bits);
}

static void math_fft_k(complex_t *dat, const complex_t *w, unsigned int k, unsigned int k_all) {
    unsigned int i, j;
    complex_t *dat1;
    k_all = 1 << (k_all - k - 1);
    k = 1 << k;
    dat1 = dat + k;
    for (i = 0; i < k_all; i++) {
        for (j = 0; j < k; j++)
            math_fft_butterfly(dat++, dat1++, w + j * k_all);
        dat += k;
        dat1 += k;
    }
}

void math_fft_init(complex_t *w, unsigned int k) {
    float beta = 0.0f, dbeta = 3.1415926535f / (float) k;
    for (; k; k--) {
        w->r = qfp_fcos(beta);
        w->i = qfp_fsin(beta);
        beta += dbeta;
        w++;
    }
}

void math_fft(complex_t *dat, const complex_t *w, unsigned int k) {
    unsigned int i, j, n;
    complex_t temp;
    n = 1 << k;
    for (i = 0; i < n; i++) {
        j = math_fft_bits_reverse(i, k);
        if (i <= j)
            continue;
        temp = dat[i];
        dat[i] = dat[j];
        dat[j] = temp;
    }
    for (i = 0; i < k; i++)
        math_fft_k(dat, w, i, k);
}
