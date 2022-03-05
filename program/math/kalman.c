//
// Created by 16625 on 2022-02-27.
//

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
    kalman->QPos = 0;
    kalman->QVel = 0;
    kalman->RPos = 0;    //0.03f;

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

/* 将载体坐标系转换为北东天 */
// 参数是计算出来的真北角
// 将要转换的数据提前变化为合适的单位

void coordinate_system_transformation_neu(float delta, float dt) {
    int lat_decimal = 1, lon_decimal = 1;
    int temp_lat = gps_rmc.latitude, temp_lon = gps_rmc.longitude;
    unsigned char lat_decimal_place =
        change_latitude_longitude_format((unsigned int *) &temp_lat, gps_rmc.decimal_places_latitude);
    unsigned char lon_decimal_place =
        change_latitude_longitude_format((unsigned int *) &temp_lon, gps_rmc.decimal_places_longitude);
    lat_decimal = num_times_nth_power_of_10(lat_decimal, lat_decimal_place);
    lon_decimal = num_times_nth_power_of_10(lon_decimal, lon_decimal_place);
    neu.north_distance = get_distance_m_lat((float) temp_lat / (float) lat_decimal);
    neu.east_distance = get_distance_m_lon((float) temp_lon / (float) lon_decimal);

    neu.north_acceleration = MG_TO_M_S_2
    ((float) imu.x_acll * FACTOR_ALLC * qfp_fcos(delta) + (float) imu.y_acll * FACTOR_ALLC * qfp_fcos(delta + 90));
    neu.east_acceleration = MG_TO_M_S_2
    ((float) imu.x_acll * FACTOR_ALLC * qfp_fsin(delta) + (float) imu.y_acll * FACTOR_ALLC * qfp_fsin(delta + 90));

/* 用GPS得到速度，转换单位后，再把坐标系转换为北东天 */
    float temp_v = (float) gps_rmc.speed_to_ground_section;
    int v_decimal = num_times_nth_power_of_10(1, gps_rmc.decimal_places_speed);
    temp_v = KNOT_TO_M_S(temp_v / v_decimal);
    neu.north_v = temp_v * qfp_fcos(delta);
    neu.east_v = temp_v * qfp_fsin(delta);

//    int temp_v = KNOT_TO_M_S(gps_rmc.speed_to_ground_section); // Converting the units of speed to m/s
//    gps_v_decimal = num_times_nth_power_of_10(gps_v_decimal, gps_rmc.decimal_places_speed);
//    neu.north_v = (float) gps_rmc.speed_to_ground_section / (float) gps_v_decimal * qfp_fcos(delta);
//    neu.east_v = (float) gps_rmc.speed_to_ground_section / (float) gps_v_decimal * qfp_fsin(delta);

//    if (neu.north_acceleration >= 1 || neu.north_acceleration <= -1) {
//        neu.north_v += neu.north_acceleration * dt;
//    } else {
//        if (neu.north_v <= 0.5 || neu.north_v >= -0.5) { neu.north_v = 0; }
//    }
//    if (neu.east_acceleration >= 1 || neu.east_acceleration <= -1) {
//        neu.east_v += neu.east_acceleration * dt;
//    } else {
//        if (neu.east_v <= 0.5 || neu.east_v >= -0.5) { neu.east_v = 0; }
//    }



}
