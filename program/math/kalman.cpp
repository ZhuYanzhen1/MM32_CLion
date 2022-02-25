//
// Created by 16625 on 2022-02-25.
//

#include "kalman.h"

static float Q_angle;
static float Q_bias;
static float R_measure;
static float P[2][2];
static float f_bias;
static float f_angle;
static float rate;
float y, new_yaw;

float roll_kal, pitch_kal, yaw_kal;

void kalman_init(void) {
    Q_angle = 0.0001f;
    Q_bias = 0.0003f;
    R_measure = 0.001f;

    f_bias = 0.0f;
    f_angle = 0.0f;
    P[0][0] = 0.0f;
    P[0][1] = 0.0f;
    P[1][0] = 0.0f;
    P[1][1] = 0.0f;
}

float get_kalman_angle(float new_angle, float new_gyro, float dt) {
    //step 1
    rate = new_gyro - f_bias;
    f_angle += rate * dt;
    //step 2

    P[0][0] += dt * (dt * P[1][1] - P[0][1] - P[1][0] + Q_angle);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_bias * dt;
    //step 3
    y = new_angle - f_angle;
    //step 4
    float S = P[0][0] + R_measure;
    // step 5 : Kalman gain
    float K[2];
    K[0] = P[0][0] / S;
    K[1] = P[1][0] / S;
    // step 6
    f_angle += K[0] * y;
    f_bias += K[1] * y;
    // step 7
    float P00_temp = P[0][0];
    float P01_temp = P[0][1];

    P[0][0] -= K[0] * P00_temp;
    P[0][1] -= K[0] * P01_temp;
    P[1][0] -= K[1] * P00_temp;
    P[1][1] -= K[1] * P01_temp;

    return f_angle;
}
