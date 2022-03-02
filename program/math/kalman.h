//
// Created by 16625 on 2022-02-27.
//

#ifndef MAIN_C_MATH_KALMAN_H_
#define MAIN_C_MATH_KALMAN_H_

#define GEO_ANGLE(x)    ((x) * PI / 180.0)
#define PI              3.1415926f
#define FACTOR_ALLC     1.25f
#define KNOT_TO_M_S(x)  ((x)* 0.514444)
#define MG_TO_M_S_2(x)  ((x)/102.1627f)

typedef struct {
    /* Kalman filter variables */
    float QPos; // Process noise variance for the position
    float QVel; // Process noise variance for the velocity
    float RPos;    // Measurement noise variance - this is actually the variance of the measurement noise
    float RVel;    // Measurement noise variance - this is actually the variance of the measurement noise

    float pos;  // The pos calculated by the Kalman filter - part of the 2x1 state vector
    float vel;  // The gyro velocity calculated by the Kalman filter - part of the 2x1 state vector
    float bias; // The bias of velocity

    float P[2][2]; // Error covariance 2x2 matrix
} kalman_filter_float;

typedef volatile struct {
    float north_acceleration;
    float earth_acceleration;
    float north_v;
    float earth_v;
    float north_distance;
    float earth_distance;
} neu_infomation;

extern neu_infomation neu;

void kalman_config_ture_north(kalman_filter_float *kalman);
float kalman_update_ture_north(kalman_filter_float *kalman, float newpos, float newVel, float dt);
void kalman_config_distance(kalman_filter_float *kalman);
float kalman_update_distance(kalman_filter_float *kalman, float new_pows, float new_v, float acc_enu, float dt);
void coordinate_system_transformation_neu(float delta);

#endif //MAIN_C_MATH_KALMAN_H_
