/**************************************************************************/ /**
    \file       kalman.c
    \brief      Kalman Fusion Source File
    \author     ZGL
    \version    V1.3.2
    \date       06. March 2022
******************************************************************************/

#include "kalman.h"
#include "hmc5883l.h"

/* Geomagnetometer after correction Calculation of the true north angle */
float true_north;

/* Kalman fusion true north angle
 * (true north angle from geomagnetometer + z-axis angular velocity from gyroscope) */
float true_north_final;
kalman_filter_float kalman_angle_north = {0};

/* Kalman fusion to obtain northward and eastward velocities
 * (GPS velocity + imu acceleration) */
float v_north;
float v_east;
kalman_filter_float kalman_v_north = {0};
kalman_filter_float kalman_v_east = {0};
float v_north_final;
float v_east_final;

/* Kalman fusion to obtain northward displacement and eastward displacement */
float distance_north;
float distance_east;
kalman_filter_float kalman_distance_north = {0};
kalman_filter_float kalman_distance_earth = {0};

void kalman_config() {
    float temp_angle_north = ture_north_begin();
    kalman_config_angle(&kalman_angle_north, temp_angle_north);
    kalman_config_v(&kalman_v_north);
    kalman_config_v(&kalman_v_east);
    kalman_config_distance(&kalman_distance_north, 384400);//得改     //337785    工一楼顶 //384400 西八楼顶
    kalman_config_distance(&kalman_distance_earth, 1487900);//得改    //377330            //1487900
}

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

/*
        iic_read_hmc5883l();
        hmc5883l_correction();
        adis_burst_read();
        true_north = qfp_fadd(qfp_fmul(qfp_fatan2(magnetometer_correction.y, magnetometer_correction.x),
                                       qfp_fdiv(180, PI)), 180);
        true_north_final = kalman_update(&kalman_angle_north, true_north,
                                         -(float) imu.z_gyro * 0.1f, 0.101f, 1);

        while (gps_rmc.status != 'A');  //用到GPS的时候开这个，不用的时候不开
        coordinate_system_transformation_neu(true_north_final);

        v_north = kalman_update(&kalman_v_north, neu.north_v, neu.north_acceleration,
                                0.101f, 0);
        v_east = kalman_update(&kalman_v_east, neu.east_v, neu.east_acceleration,
                               0.101f, 0);
        if (v_north < 1 && v_north > -1) v_north_final = neu.north_v;
        else v_north_final = v_north;
        if (v_east < 1 && v_east > -1) v_east_final = neu.east_v;
        else v_east_final = v_east;
        distance_north = kalman_update(&kalman_distance_north, neu.north_distance,
                                       v_north_final, 0.101f, 0);
        distance_east = kalman_update(&kalman_distance_earth, neu.east_distance,
                                      v_east_final, 0.101f, 0);
 * */
