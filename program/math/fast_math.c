//
// Created by LaoZhu on 2022/2/28.
//

#include "fast_math.h"

#define M_PI_2        1.57079632f

float LUT[102] = {
    0, 0.0099996664f, 0.019997334f, 0.029991005f, 0.039978687f,
    0.049958397f, 0.059928156f, 0.069885999f, 0.079829983f, 0.089758173f,
    0.099668652f, 0.10955953f, 0.11942893f, 0.12927501f, 0.13909595f,
    0.14888994f, 0.15865526f, 0.16839015f, 0.17809294f, 0.18776195f,
    0.19739556f, 0.20699219f, 0.21655031f, 0.22606839f, 0.23554498f,
    0.24497867f, 0.25436807f, 0.26371184f, 0.27300870f, 0.28225741f,
    0.29145679f, 0.30060568f, 0.30970293f, 0.31874755f, 0.32773849f,
    0.33667481f, 0.34555557f, 0.35437992f, 0.36314702f, 0.37185606f,
    0.38050637f, 0.38909724f, 0.39762798f, 0.40609807f, 0.41450688f,
    0.42285392f, 0.43113875f, 0.43936089f, 0.44751999f, 0.45561564f,
    0.46364760f, 0.47161558f, 0.47951928f, 0.48735857f, 0.49513325f,
    0.50284320f, 0.51048833f, 0.51806855f, 0.52558380f, 0.53303409f,
    0.54041952f, 0.54774004f, 0.55499572f, 0.56218672f, 0.56931317f,
    0.57637525f, 0.58337301f, 0.59030676f, 0.59717667f, 0.60398299f,
    0.61072594f, 0.61740589f, 0.62402308f, 0.63057774f, 0.63707036f,
    0.64350110f, 0.64987046f, 0.65617871f, 0.66242629f, 0.66861355f,
    0.67474097f, 0.68080884f, 0.68681765f, 0.69276786f, 0.69865984f,
    0.70449406f, 0.71027100f, 0.71599114f, 0.72165483f, 0.72726268f,
    0.73281509f, 0.73831260f, 0.74375558f, 0.74914461f, 0.75448018f,
    0.75976276f, 0.76499283f, 0.77017093f, 0.77529752f, 0.78037310f,
    0.78539819f, 0.79037325f};

float atan_fast(float x) {
    /*
    Linear interpolation is used for higher accuracy
    */
    if (x >= 0) {
        if (x <= 1) {
            int index = (int) (x * 100);
            return (LUT[index] + (x * 100 - (float) index) * (LUT[index + 1] - LUT[index]));
        } else {
            float re_x = 1 / x;
            int index = (int) (re_x * 100);
            return (M_PI_2 - (LUT[index] + (re_x * 100.0f - (float) index) * (LUT[index + 1] - LUT[index])));
            // No recursive is better here
        }
    } else {
        if (x >= -1) {
            float abs_x = -x;
            int index = (int) (abs_x * 100);
            return -(LUT[index] + (abs_x * 100.0f - (float) index) * (LUT[index + 1] - LUT[index]));
        } else {
            float re_x = 1 / (-x);
            int index = (int) (re_x * 100);
            return (LUT[index] + (re_x * 100.0f - (float) index) * (LUT[index + 1] - LUT[index])) - M_PI_2;
        }
    }
}

float fast_sqrt(float value) {
    float xhalf = 0.5f * value;
    int i = *(int *) &value; // get bits for floating value
    i = 0x5f3759df - (i >> 1); // gives initial guess
    value = *(float *) &i; // convert bits back to float
    value = value * (1.5f - xhalf * value * value); // Newton step
    value = 1.0f / value;
    return value;
}
