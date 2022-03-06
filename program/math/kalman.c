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
