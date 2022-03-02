//
// Created by 16625 on 2022-02-27.
//

#include "kalman.h"
#include "qfplib.h"
#include "adis16470.h"
#include "gps_parser.h"

neu_infomation neu;

void kalman_config_ture_north(kalman_filter_float *kalman) {
    kalman->QPos = 0.001f;
    kalman->QVel = 0.003f;
    kalman->RPos = 1;    //0.03f;

    kalman->pos = 0.0f;  // Reset the pos
    kalman->vel = 0.0f;  // Reset speed
    kalman->bias = 0.0f; // Reset bias of velocity

    // Since we assume that the speed is 0 and we know the starting pos (use setpos),
    // the error covariance matrix is set like so
    kalman->P[0][0] = 0.0049525f;
    kalman->P[0][1] = 0.0032382f;
    kalman->P[1][0] = 0.0032382f;
    kalman->P[1][1] = 0.0048883f;
};

// The pos should be in degrees and the rate should be in degrees per second and the delta time in seconds
float kalman_update_ture_north(kalman_filter_float *kalman, float newpos, float newVel, float dt) {

    // Discrete Kalman filter time update equations - Time Update ("Predict")
    // Update xhat - Project the state ahead
    /* Step 1 */
    kalman->vel = newVel - kalman->bias;
    kalman->pos += dt * kalman->vel;

    kalman->pos =
        (kalman->pos < 0) ? kalman->pos + 360.0f : (kalman->pos > 360.0f) ? kalman->pos - 360.0f : kalman->pos;

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
    float S = kalman->P[0][0] + kalman->RVel; // Estimate error
    /* Step 5 */
    float K[2]; // Kalman gain - This is a 2x1 vector
    K[0] = kalman->P[0][0] / S;
    K[1] = kalman->P[1][0] / S;

    /* Step 6 */
    kalman->pos += K[0] * y;
    kalman->vel += K[1] * y;   // ?

    // Calculate estimation error covariance - Update the error covariance
    /* Step 7 */
    float P00_temp = kalman->P[0][0];
    float P01_temp = kalman->P[0][1];

    kalman->P[0][0] -= K[0] * P00_temp;
    kalman->P[0][1] -= K[0] * P01_temp;
    kalman->P[1][0] -= K[1] * P00_temp;
    kalman->P[1][1] -= K[1] * P01_temp;

    return kalman->pos;
};

void kalman_config_distance(kalman_filter_float *kalman) {
    kalman->QPos = 0;
    kalman->QVel = 0;
    kalman->RPos = 0.5f;   // gps的噪声是0.5m
    kalman->RVel = 0.013f;      // 陀螺仪的噪声是 13μg  m/s^2

    kalman->pos = 0.0f;  // Reset the pos
    kalman->vel = 0.0f;  // Reset speed
    kalman->bias = 0.0f; // Reset bias of velocity

    // Since we assume that the speed is 0 and we know the starting pos (use setpos),
    // the error covariance matrix is set like so
    kalman->P[0][0] = 0;
    kalman->P[0][1] = 0;
    kalman->P[1][0] = 0;
    kalman->P[1][1] = 0;
}

// 加速度计的单位是m/s^2 ,读取到的单位是mg
float kalman_update_distance(kalman_filter_float *kalman, float new_pows, float new_v, float acc_enu, float dt) {
    kalman->pos += kalman->vel * dt + acc_enu * dt * dt / 2;
    kalman->vel += acc_enu * dt;

    kalman->P[0][0] += kalman->P[1][0] * dt + dt * (kalman->P[0][1] + kalman->P[1][1]) + kalman->QPos;
    kalman->P[0][1] += kalman->P[1][1] * dt + kalman->QPos;
    kalman->P[0][1] += kalman->P[1][1] * dt + kalman->QVel;
    kalman->P[1][1] += kalman->QVel;

    float y[2];
    y[0] = new_pows - kalman->pos;
    y[1] = new_v - kalman->vel;

    float S[2][2];
    S[0][0] = kalman->P[0][0] + kalman->RPos; // Estimate error
    S[0][1] = kalman->P[0][1]; // +0还是+RPos
    S[1][0] = kalman->P[1][0]; // +0还是+RVel
    S[1][1] = kalman->P[1][1] + kalman->RVel; // Estimate error

    float P_00 = kalman->P[0][0];
    float P_01 = kalman->P[0][1];
    float P_10 = kalman->P[1][0];
    float P_11 = kalman->P[1][1];

    float K[2][2];
    K[0][0] = (P_00 * P_11 - P_01 * P_10 + P_00 * kalman->RVel)
        / (P_00 * P_11 - P_01 * P_10 + P_00 * kalman->RVel + P_11 * kalman->RPos + kalman->RPos * kalman->RVel);
    K[0][1] = (P_01 * kalman->RPos)
        / (P_00 * P_11 - P_01 * P_10 + P_00 * kalman->RVel + P_11 * kalman->RPos + kalman->RPos * kalman->RVel);
    K[1][0] = (P_10 * kalman->RVel)
        / (P_00 * P_11 - P_01 * P_10 + P_00 * kalman->RVel + P_11 * kalman->RPos + kalman->RPos * kalman->RVel);
    K[1][1] = (P_00 * P_11 - P_01 * P_10 + P_11 * kalman->RPos)
        / (P_00 * P_11 - P_01 * P_10 + P_00 * kalman->RVel + P_11 * kalman->RPos + kalman->RPos * kalman->RVel);

    kalman->pos += K[0][0] * y[0] + K[0][1] * y[1];
    kalman->vel += K[1][0] * y[0] + K[1][1] * y[1];

    kalman->P[0][0] = -(P_00 * (K[0][0] - 1) + K[0][1] * P_10);//- P_00*(K[0][0] - 1) - K[0][1]*P_10
    kalman->P[0][1] = -(P_01 * (K[0][0] - 1) + K[0][1] * P_11);//- P_01*(K[0][0] - 1) - K[0][1]*P_11
    kalman->P[1][0] = -(P_10 * (K[1][1] - 1) + K[1][0] * P_00);//- P_10*(K[1][1] - 1) - K[1][0]*P_00
    kalman->P[1][1] = -(P_11 * (K[1][1] - 1) + K[1][0] * P_01);//- P_11*(K[1][1] - 1) - K[1][0]*P_01

    return kalman->pos;
}

#include "math.h"

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

/* 将载体坐标系转换为北东天 */
// 参数是计算出来的真北角
// 将要转换的数据提前变化为合适的单位
// 精度可能不够，建议加多一位，不能多加，因为这样下去经度要超过int的范围了
void coordinate_system_transformation_neu(float delta) {
    unsigned char lat_decimal_place = 0, lon_decimal_place = 0;
    int lat_decimal = 1, lon_decimal = 1, gps_v_decimal = 1;
    lat_decimal_place = change_latitude_longitude_format(&gps_rmc.latitude, gps_rmc.decimal_places_latitude);
    lon_decimal_place = change_latitude_longitude_format(&gps_rmc.longitude, gps_rmc.decimal_places_longitude);
    lat_decimal = num_times_nth_power_of_10(lat_decimal, lat_decimal_place);
    lon_decimal = num_times_nth_power_of_10(lon_decimal, lon_decimal_place);
    gps_v_decimal = num_times_nth_power_of_10(gps_v_decimal, gps_rmc.decimal_places_speed);

    /* Convert latitude and longitude to units of° */
    gps_rmc.speed_to_ground_section =
        KNOT_TO_M_S(gps_rmc.speed_to_ground_section); // Converting the units of speed to m/s
    //    neu.north_acceleration =
    (float) imu.x_acll * FACTOR_ALLC * cos((double) delta) + (float) imu.y_acll * FACTOR_ALLC * cos((double) delta);
    neu.north_acceleration = MG_TO_M_S_2
    ((float) imu.x_acll * FACTOR_ALLC * qfp_fcos(delta) + (float) imu.y_acll * FACTOR_ALLC * qfp_fcos(delta));
    neu.earth_acceleration = MG_TO_M_S_2
    ((float) imu.x_acll * FACTOR_ALLC * qfp_fsin(delta) + (float) imu.y_acll * FACTOR_ALLC * qfp_fsin(delta));
    neu.north_v = (float) gps_rmc.speed_to_ground_section / (float) gps_v_decimal * qfp_fcos(delta);
    neu.earth_v = (float) gps_rmc.speed_to_ground_section / (float) gps_v_decimal * qfp_fsin(delta);
    neu.north_distance = get_distance_m((float) gps_rmc.latitude / (float) lat_decimal);
    neu.earth_distance = get_distance_m((float) gps_rmc.longitude / (float) lon_decimal);
}
