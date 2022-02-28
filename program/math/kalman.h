//
// Created by 16625 on 2022-02-27.
//

#ifndef MAIN_C_MATH_KALMAN_H_
#define MAIN_C_MATH_KALMAN_H_

typedef struct {
    /* Kalman filter variables */
    float QPos; // Process noise variance for the position
    float QVel; // Process noise variance for the velocity
    float R;    // Measurement noise variance - this is actually the variance of the measurement noise

    float pos;  // The pos calculated by the Kalman filter - part of the 2x1 state vector
    float vel;  // The gyro velocity calculated by the Kalman filter - part of the 2x1 state vector
    float bias; // The bias of velocity

    float P[2][2]; // Error covariance 2x2 matrix
} kalman_filter_float;

typedef struct { //整数运算：扩大1000倍，即取3位浮点数
    /* Kalman filter variables */
    int QPos; // Process noise variance for the position
    int QVel; // Process noise variance for the velocity
    int R;    // Measurement noise variance - this is actually the variance of the measurement noise

    int pos;  // The pos calculated by the Kalman filter - part of the 2x1 state vector
    int vel;  // The gyro velocity calculated by the Kalman filter - part of the 2x1 state vector
    int bias; // The bias of velocity

    int P[2][2]; // Error covariance 2x2 matrix
} kalman_filter_int;

void kalman_config(kalman_filter_float *kalman);

float kalman_update(kalman_filter_float *kalman, float newPos, float newVel, float dt);

void kalman_config_int(kalman_filter_int *kalman);

int kalman_update_int(kalman_filter_int *kalman, int newpos, int newVel, int dt);

#endif //MAIN_C_MATH_KALMAN_H_
