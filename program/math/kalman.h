/**************************************************************************/ /**
    \file       kalman.h
    \brief      Kalman Fusion Header File
    \author     ZGL
    \version    V1.3.2
    \date       06. March 2022
******************************************************************************/

#ifndef MM32F3277_MATH_KALMAN_H_
#define MM32F3277_MATH_KALMAN_H_

typedef struct {
    float q_pos; // Process noise variance for the position
    float q_vel; // Process noise variance for the velocity
    float r_pos; // Measurement noise variance - this is actually the variance of the measurement noise

    float pos;   // The pos calculated by the Kalman filter - part of the 2x1 state vector
    float vel;   // The gyro velocity calculated by the Kalman filter - part of the 2x1 state vector
    float bias;  // The bias of velocity

    float P[2][2]; // Error covariance 2x2 matrix
} kalman_filter_float;

void kalman_config_angle(kalman_filter_float *kalman, float pos_0);
void kalman_config_v(kalman_filter_float *kalman);
void kalman_config_distance(kalman_filter_float *kalman, float pos_0);
float kalman_update(kalman_filter_float *kalman, float new_pos, float newVel, float dt, unsigned char angle_flag);
void kalman_config();

#endif //MM32F3277_MATH_KALMAN_H_
