//
// Created by 16625 on 2022-02-27.
//

#include "kalman.h"

void kalman_config(kalman_filter_float *kalman) {
    kalman->QPos = 0.001f;
    kalman->QVel = 0.003f;
    kalman->R = 0.03f;

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
float kalman_update(kalman_filter_float *kalman, float newpos, float newVel, float dt) {

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
    float S = kalman->P[0][0] + kalman->R; // Estimate error
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
