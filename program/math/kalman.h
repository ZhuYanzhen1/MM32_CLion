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

void kalman_config(kalman_filter_float *kalman);
float kalman_update(kalman_filter_float *kalman, float newPos, float newVel, float dt);

#endif //MAIN_C_MATH_KALMAN_H_
