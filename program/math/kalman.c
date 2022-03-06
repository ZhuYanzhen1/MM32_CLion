/**************************************************************************/ /**
    \file       kalman.c
    \brief      Kalman Fusion
                Coordinate system transformation
                Data Transformation
    \author     ZGL
    \version    V1.3.2
    \date       06. March 2022
******************************************************************************/

#include "kalman.h"
#include "qfplib.h"
#include "adis16470.h"
#include "gps_parser.h"

neu_infomation neu = {0};

void kalman_config_angle(kalman_filter_float *kalman, float pos_0) {
    kalman->q_pos = 0.0065f;
    kalman->q_vel = 0.01f;
    kalman->r_pos = 0.56f;

    kalman->pos = pos_0;
    kalman->vel = 0.0f;
    kalman->bias = 0.0f;

    kalman->P[0][0] = 0.0049525f;
    kalman->P[0][1] = 0.0032382f;
    kalman->P[1][0] = 0.0032382f;
    kalman->P[1][1] = 0.0048883f;
}

void kalman_config_v(kalman_filter_float *kalman) {
    kalman->q_pos = 0.17f;
    kalman->q_vel = 0.0123f;
    kalman->r_pos = 0.05f;

    kalman->pos = 0;
    kalman->vel = 0.0f;
    kalman->bias = 0.0f;

    kalman->P[0][0] = 0;
    kalman->P[0][1] = 0;
    kalman->P[1][0] = 0;
    kalman->P[1][1] = 0;
};

//TODO P的系数要改
void kalman_config_distance(kalman_filter_float *kalman, float pos_0) {
    kalman->q_pos = 0.8f;
    kalman->q_vel = 0.22f;
    kalman->r_pos = 0.2f;

    kalman->pos = pos_0;
    kalman->vel = 0.0f;
    kalman->bias = 0.0f;

    kalman->P[0][0] = 0.0526622f;
    kalman->P[0][1] = -0.022637f;
    kalman->P[1][0] = -0.022637f;
    kalman->P[1][1] = 0.0232643f;
}

/*!
    \brief      Kalman Fusion
    \param[in]  kalman:Kalman Structures
    \param[in]  new_pos
    \param[in]  new_vel
    \param[in]  dt:Timestamp between two fusions
    \param[in]  angle_flag:Timestamp between two fusions
    \retval     Position after fusion
    \note       The pos should be in degrees and the rate should be in degrees per second
                and the delta time in seconds
*/
float kalman_update(kalman_filter_float *kalman, float new_pos, float new_vel, float dt, unsigned char angle_flag) {

    /* Discrete Kalman filter time update equations - Time Update ("Predict") */
    /* Update xhat - Project the state ahead */
    /* Step 1 */
    kalman->vel = new_vel - kalman->bias;
    kalman->pos += dt * kalman->vel;

    /* When using geomagnetometer and gyroscope to fuse out the true north angle, flag == 1 */
    /* Keep the angle in the range of 0~360° */
    if (angle_flag) {
        kalman->pos =
            (kalman->pos < 0) ? kalman->pos + 360.0f : (kalman->pos > 360.0f) ? kalman->pos - 360.0f : kalman->pos;
    }

    /* Update estimation error covariance - Project the error covariance ahead */
    /* Step 2 */
    kalman->P[0][0] += dt * (dt * kalman->P[1][1] - kalman->P[0][1] - kalman->P[1][0] + kalman->q_pos);
    kalman->P[0][1] -= dt * kalman->P[1][1];
    kalman->P[1][0] -= dt * kalman->P[1][1];
    kalman->P[1][1] += kalman->q_vel * dt;

    /* Calculate pos and velocity - Update estimate with measurement zk (new_pos) */
    /* Step 3 */
    /* pos difference */
    float y = new_pos - kalman->pos;

    /* Discrete Kalman filter measurement update equations - Measurement Update ("Correct") */
    /* Calculate Kalman gain - Compute the Kalman gain */
    /* Step 4 */
    /* Estimate error */
    float S = kalman->P[0][0] + kalman->r_pos;

    /* Step 5 */
    float K[2];
    K[0] = kalman->P[0][0] / S;
    K[1] = kalman->P[1][0] / S;

    /* Step 6 */
    kalman->pos += K[0] * y;
    kalman->vel += K[1] * y;
    /* Overcome the transition from 360° to 0° */
    if (angle_flag)
        if (y >= 300 || y <= -300)
            kalman->pos = new_pos;

    /* Calculate estimation error covariance - Update the error covariance */
    /* Step 7 */
    float P00_temp = kalman->P[0][0];
    float P01_temp = kalman->P[0][1];

    kalman->P[0][0] -= K[0] * P00_temp;
    kalman->P[0][1] -= K[0] * P01_temp;
    kalman->P[1][0] -= K[1] * P00_temp;
    kalman->P[1][1] -= K[1] * P01_temp;

    return kalman->pos;
}

/*!
    \brief      Convert latitude to displacement in ENU coordinate system
    \param[in]  latitude(°)
    \retval     Converted data
*/
float get_distance_m_lat(float lat) {
    float distance = GEO_ANGLE(lat);
    return EARTH_RADIUS * distance;
}

/*!
    \brief      Convert longitude to displacement in ENU coordinate system
    \param[in]  longitude(°)
    \retval     Converted data
*/
float get_distance_m_lon(float lon) {
    float distance = GEO_ANGLE(lon);
    return EARTH_RADIUS * distance;
}

/*!
    \brief      Preceding steps of Kalman Fusion
                Convert the acceleration of imu and velocity of gps to NEU coordinate system
                Convert the data units read from the sensor to m/s^2, m/s, m
    \param[in]  True North Angle
*/
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

    /* Use GPS to get the velocity, convert the units,
     * and then convert the coordinate system to the NEU */
    float temp_v = (float) gps_rmc.speed_to_ground_section;
    int v_decimal = num_times_nth_power_of_10(1, gps_rmc.decimal_places_speed);
    temp_v = KNOT_TO_M_S(temp_v / v_decimal);
    neu.north_v = temp_v * qfp_fcos(temp_delta);
    neu.east_v = temp_v * qfp_fsin(temp_delta);
}
