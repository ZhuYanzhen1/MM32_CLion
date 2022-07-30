/**************************************************************************/ /**
    \file       kalman.c
    \brief      Kalman Fusion Source File
    \author     ZGL
    \version    V1.6.1
    \date       24. July 2022
******************************************************************************/

#include "kalman.h"

void kalman_config_v(kalman_filter_t *kalman) {
    kalman->q_pos = V_Q_POS;
    kalman->q_vel = 0.1f;
    kalman->r_pos = 0.2f;
    kalman->r_old_pos = kalman->r_pos;
    kalman->gps_valid_flag = 1;

    kalman->pos = 0;
    kalman->vel = 0.0f;
    kalman->bias = 0.0f;

    kalman->P[0][0] = 0;
    kalman->P[0][1] = 0;
    kalman->P[1][0] = 0;
    kalman->P[1][1] = 0;
};

void kalman_config_distance(kalman_filter_t *kalman, float pos_0) {
    kalman->q_pos = DIS_Q_POS;
    kalman->q_vel = 0.22f;
    kalman->r_pos = 3.0f;
    kalman->r_old_pos = kalman->r_pos;

    kalman->pos = pos_0;
    kalman->vel = 0.0f;
    kalman->bias = 0.0f;

    kalman->P[0][0] = 0;
    kalman->P[0][1] = 0;
    kalman->P[1][0] = 0;
    kalman->P[1][1] = 0;
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
float kalman_update(kalman_filter_t *kalman, float new_pos, float new_vel, float dt) {
    if (kalman->gps_valid_flag == 1)
        kalman->r_pos = kalman->r_old_pos;
    else if (kalman->gps_valid_flag == 2)
        kalman->r_pos = 20;
    else if (kalman->gps_valid_flag == 0)
        kalman->r_pos = 100000;

    if (kalman->q_pos == V_Q_POS)
        kalman->gps_valid_flag = 2;
    else if (kalman->q_pos == DIS_Q_POS)
        kalman->gps_valid_flag = 0;

    /* Discrete Kalman filter time update equations - Time Update ("Predict") */
    /* Update xhat - Project the state ahead */
    /* Step 1 */
    kalman->vel = new_vel - kalman->bias;
    kalman->pos += dt * kalman->vel;

    /* Update estimation error covariance - Project the error covariance ahead */
    /* Step 2 */
//    kalman->P[0][0] += dt * (dt * kalman->P[1][1] - kalman->P[0][1] - kalman->P[1][0] + kalman->q_pos);
//    kalman->P[0][1] -= dt * kalman->P[1][1];
//    kalman->P[1][0] -= dt * kalman->P[1][1];
//    kalman->P[1][1] += kalman->q_vel * dt;

    kalman->P[0][0] += dt * (dt * kalman->P[1][1] + kalman->P[0][1] + kalman->P[1][0]) + kalman->q_pos;
    kalman->P[0][1] += dt * kalman->P[1][1] + kalman->q_pos;
    kalman->P[1][0] += dt * kalman->P[1][1] + kalman->q_vel;
    kalman->P[1][1] += kalman->q_vel;

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


