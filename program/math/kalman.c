/**************************************************************************/ /**
    \file       kalman.c
    \brief      Kalman Fusion
                Coordinate system transformation
                Data Transformation
    \author     ZGL
    \version    V1.0.1
    \date       05. March 2022
******************************************************************************/

#include "kalman.h"
#include "qfplib.h"
#include "adis16470.h"
#include "gps_parser.h"

neu_infomation neu = {0};

void kalman_config_angle(kalman_filter_float *kalman, float pos_0) {
    kalman->QPos = 0.0065f;
    kalman->QVel = 0.01f;
    kalman->RPos = 0.56f;    //0.03f;

    kalman->pos = pos_0;  // Reset the pos
    kalman->vel = 0.0f;  // Reset speed
    kalman->bias = 0.0f; // Reset bias of velocity

    // Since we assume that the speed is 0 and we know the starting pos (use setpos),
    // the error covariance matrix is set like so
    kalman->P[0][0] = 0.0049525f;
    kalman->P[0][1] = 0.0032382f;
    kalman->P[1][0] = 0.0032382f;
    kalman->P[1][1] = 0.0048883f;
}

void kalman_config_v(kalman_filter_float *kalman) {
    kalman->QPos = 0.17f;
    kalman->QVel = 0.0123f;
    kalman->RPos = 0.05f;    //0.03f;

    kalman->pos = 0;  // Reset the pos
    kalman->vel = 0.0f;  // Reset speed
    kalman->bias = 0.0f; // Reset bias of velocity

    // Since we assume that the speed is 0 and we know the starting pos (use setpos),
    // the error covariance matrix is set like so
    kalman->P[0][0] = 0;
    kalman->P[0][1] = 0;
    kalman->P[1][0] = 0;
    kalman->P[1][1] = 0;
};

//TODO P的系数要改
void kalman_config_distance(kalman_filter_float *kalman, float pos_0) {
    kalman->QPos = 0.8f;
    kalman->QVel = 0.22f;
    kalman->RPos = 0.2f;    //0.03f;

    kalman->pos = pos_0;  // Reset the pos
    kalman->vel = 0.0f;  // Reset speed
    kalman->bias = 0.0f; // Reset bias of velocity

    // Since we assume that the speed is 0 and we know the starting pos (use setpos),
    // the error covariance matrix is set like so
    kalman->P[0][0] = 0.0526622f;//0.0526622012
    kalman->P[0][1] = -0.022637f;//-0.022636503
    kalman->P[1][0] = -0.022637f;//-0.022636503
    kalman->P[1][1] = 0.0232643f;//0.0232642889
}

// The pos should be in degrees and the rate should be in degrees per second and the delta time in seconds
float kalman_update(kalman_filter_float *kalman, float newpos, float newVel, float dt, unsigned char angle_flag) {

    // Discrete Kalman filter time update equations - Time Update ("Predict")
    // Update xhat - Project the state ahead
    /* Step 1 */
    kalman->vel = newVel - kalman->bias;
    kalman->pos += dt * kalman->vel;

    if (angle_flag) {
        kalman->pos =
            (kalman->pos < 0) ? kalman->pos + 360.0f : (kalman->pos > 360.0f) ? kalman->pos - 360.0f : kalman->pos;
    }
    // Update estimation error covariance - Project the error covariance ahead
    /* Step 2 */
    kalman->P[0][0] += dt * (dt * kalman->P[1][1] - kalman->P[0][1] - kalman->P[1][0] + kalman->QPos);
    kalman->P[0][1] -= dt * kalman->P[1][1];
    kalman->P[1][0] -= dt * kalman->P[1][1];
    kalman->P[1][1] += kalman->QVel * dt;

    // Calculate pos and velocity - Update estimate with measurement zk (newpos)
    /* Step 3 */
    float y = newpos - kalman->pos; // pos difference

    // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
    // Calculate Kalman gain - Compute the Kalman gain
    /* Step 4 */
    float S = kalman->P[0][0] + kalman->RPos; // Estimate error
    /* Step 5 */
    float K[2]; // Kalman gain - This is a 2x1 vector
    K[0] = kalman->P[0][0] / S;
    K[1] = kalman->P[1][0] / S;

    /* Step 6 */
    kalman->pos += K[0] * y;
    kalman->vel += K[1] * y;
    if (angle_flag)
        if (y >= 300 || y <= -300)
            kalman->pos = newpos;

    // Calculate estimation error covariance - Update the error covariance
    /* Step 7 */
    float P00_temp = kalman->P[0][0];
    float P01_temp = kalman->P[0][1];

    kalman->P[0][0] -= K[0] * P00_temp;
    kalman->P[0][1] -= K[0] * P01_temp;
    kalman->P[1][0] -= K[1] * P00_temp;
    kalman->P[1][1] -= K[1] * P01_temp;

    return kalman->pos;
}

// 经纬度的格式是最高2、3位为整数，其余为小数
float get_distance_m(float lon_or_lat) {
    const float earth_radius = 6371 * 1000.0f; // meters
    float geo_angle = GEO_ANGLE(lon_or_lat);

    float temp = qfp_fsin(geo_angle * 0.5f);
    float temp_quadratic = temp * temp;
    float a = temp_quadratic + qfp_fcos(geo_angle) * temp_quadratic;

    float c = 2 * qfp_fatan2(qfp_fsqrt(a), qfp_fsqrt(1.0f - a));
    return earth_radius * c;
}

float get_distance_m_lat(float lat) {
    float distance = GEO_ANGLE(lat);
    return EARTH_RADIUS * distance;
}

float get_distance_m_lon(float lon) {
    float distance = GEO_ANGLE(lon);
    return EARTH_RADIUS * distance;
}

void coordinate_system_transformation_neu(float delta) {
    float temp_delta = GEO_ANGLE(delta);

    float temp_latitude = unit_to_degree(gps_rmc.latitude, 4);
    neu.north_distance = get_distance_m_lat(temp_latitude - QRIGIN_LAT);
    float temp_lonitude = unit_to_degree(gps_rmc.longitude, 4);
    neu.east_distance = get_distance_m_lon(temp_lonitude - QRIGIN_LON);

    neu.north_acceleration = MG_TO_M_S_2
    ((float) imu.x_acll * FACTOR_ALLC * qfp_fcos(temp_delta)
         + (float) imu.y_acll * FACTOR_ALLC * qfp_fcos(temp_delta + PI / 2));
    neu.east_acceleration = MG_TO_M_S_2
    ((float) imu.x_acll * FACTOR_ALLC * qfp_fsin(temp_delta)
         + (float) imu.y_acll * FACTOR_ALLC * qfp_fsin(temp_delta + PI / 2));

/* 用GPS得到速度，转换单位后，再把坐标系转换为北东天 */
    float temp_v = (float) gps_rmc.speed_to_ground_section;
    int v_decimal = num_times_nth_power_of_10(1, gps_rmc.decimal_places_speed);
    temp_v = KNOT_TO_M_S(temp_v / v_decimal);
    neu.north_v = temp_v * qfp_fcos(temp_delta);
    neu.east_v = temp_v * qfp_fsin(temp_delta);
}
