//
// Created by 16625 on 2022-02-27.
//

#ifndef MAIN_C_MATH_KALMAN_H_
#define MAIN_C_MATH_KALMAN_H_

#define GEO_ANGLE(x)    ((x) * PI / 180.0)
#define PI              3.1415926f
#define EARTH_RADIUS    6371000
#define FACTOR_ALLC     1.25f
#define QRIGIN_LAT      20  // 起点设为(20°N,100°E)
#define QRIGIN_LON      100
#define KNOT_TO_M_S(x)  ((x)* 0.514444)
#define MG_TO_M_S_2(x)  ((x)/102.1627f)

typedef struct {
    /* Kalman filter variables */
    float QPos; // Process noise variance for the position
    float QVel; // Process noise variance for the velocity
    float RPos;    // Measurement noise variance - this is actually the variance of the measurement noise

    float pos;  // The pos calculated by the Kalman filter - part of the 2x1 state vector
    float vel;  // The gyro velocity calculated by the Kalman filter - part of the 2x1 state vector
    float bias; // The bias of velocity

    float P[2][2]; // Error covariance 2x2 matrix
} kalman_filter_float;

typedef struct {
    /* Kalman filter variables */
    float QPos; // Process noise variance for the position
    float QVel; // Process noise variance for the velocity
    float QAcc;
    float RPos;    // Measurement noise variance - this is actually the variance of the measurement noise
    float RVel;    // Measurement noise variance - this is actually the variance of the measurement noise
    float RAcc;

    float pos;  // The pos calculated by the Kalman filter - part of the 2x1 state vector
    float vel;  // The gyro velocity calculated by the Kalman filter - part of the 2x1 state vector
    float acc;
    float bias; // The bias of velocity

    float P[3][3]; // Error covariance 2x2 matrix
} kalman_filter_distance;

typedef volatile struct {
    float north_acceleration;
    float east_acceleration;
    float north_v;
    float east_v;
    float north_distance;
    float east_distance;
} neu_infomation;

extern neu_infomation neu;

void kalman_config_angle(kalman_filter_float *kalman);
void kalman_config_distance(kalman_filter_float *kalman, float pos_0);
float kalman_update(kalman_filter_float *kalman, float newpos, float newVel, float dt, unsigned char angle_flag);
void coordinate_system_transformation_neu(float delta, float dt);
float get_distance_m_lat(float lat);
float get_distance_m_lon(float lon);

#endif //MAIN_C_MATH_KALMAN_H_
