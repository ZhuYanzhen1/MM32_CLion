/**************************************************************************/ /**
    \file       kalman.h
    \brief      Kalman Fusion
                Coordinate system transformation
                Data Transformation
    \author     ZGL
    \version    V1.3.2
    \date       06. March 2022
******************************************************************************/

#ifndef MM32F3277_MATH_KALMAN_H_
#define MM32F3277_MATH_KALMAN_H_

// TODO 当到了其他地方比赛之前，得先测试当地的经纬度，并更改此处的宏，否则会导致浮点运算精度丢失的问题
#define QRIGIN_LAT      20  /* The starting point is set as(20°N,110°E) */
#define QRIGIN_LON      110
#define PI              3.1415926f
#define EARTH_RADIUS    6371000
#define FACTOR_ALLC     1.25f
#define GEO_ANGLE(x)    ((x) * PI / 180.0)
#define KNOT_TO_M_S(x)  ((x)* 0.514444)
#define MG_TO_M_S_2(x)  ((x)/102.1627f)

typedef struct {
    float q_pos; // Process noise variance for the position
    float q_vel; // Process noise variance for the velocity
    float r_pos; // Measurement noise variance - this is actually the variance of the measurement noise

    float pos;   // The pos calculated by the Kalman filter - part of the 2x1 state vector
    float vel;   // The gyro velocity calculated by the Kalman filter - part of the 2x1 state vector
    float bias;  // The bias of velocity

    float P[2][2]; // Error covariance 2x2 matrix
} kalman_filter_float;

typedef volatile struct {
    float north_acceleration;
    float east_acceleration;
    float north_v;
    float east_v;
    float north_distance;
    float east_distance;
} neu_infomation;

extern neu_infomation neu;

void kalman_config_angle(kalman_filter_float *kalman, float pos_0);
void kalman_config_v(kalman_filter_float *kalman);
void kalman_config_distance(kalman_filter_float *kalman, float pos_0);
float kalman_update(kalman_filter_float *kalman, float new_pos, float newVel, float dt, unsigned char angle_flag);
void coordinate_system_transformation_neu(float delta);
float get_distance_m_lat(float lat);
float get_distance_m_lon(float lon);

#endif //MAIN_C_MATH_KALMAN_H_
