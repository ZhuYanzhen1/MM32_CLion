/**************************************************************************/ /**
    \file       riccati_solver.c
    \brief      Processing LQR-related algorithms
    \author     Lao·Zhu & ZGL
    \version    V1.6.1
    \date       24. July 2022
 ******************************************************************************/

#include "riccati_solver.h"
#include "path.h"

#ifndef RUNNING_UNIT_TEST
#include "qfplib.h"
#include "math.h"
#include "sensor_decode.h"
#include "delay.h"
#include "uart.h"
#else

#include "stdio.h"
#include "math.h"

#endif

#ifndef RUNNING_UNIT_TEST

/* \brief The last obtained angle control amount */
unsigned short last_angle = SERVO_MID_POINT;

/* \brief Timestamp at this time */
static unsigned int last_global_time_stamp = 0;

/*!
    \brief      Calculate the distance between the current coordinates and the target point
    \param[in]  ind: The nth point
    \retval     the distance between two points
*/
float calculate_distance(int ind) {
    float distance = (qfp_fsqrt
        ((test_point[ind][0] - proc_data.distance_north)
             * (test_point[ind][0] - proc_data.distance_north)
             + (test_point[ind][1] - proc_data.distance_east) *
                 (test_point[ind][1] - proc_data.distance_east)));
    return distance;
}

/*!
    \brief      Limits on the amount of control of the servo
    \param[in]  index: The nth point
    \param[in]  control_value: Servo control volume
    \retval     Corrected servo control amount
    \note       DIVIDING_POINT_1 ~ DIVIDING_POINT_4 is the dividing point between curves and straights
                BUFFER_BELT_1 ~ BUFFER_BELT_4 is the decay area where the control volume changes slowly
*/
float arranging_transition_process(float control_value, unsigned short index) {
    float attenuation_rate;
    float attenuation;
    unsigned short attenuation_index;
    if ((index > DIVIDING_POINT_3 && index < DIVIDING_POINT_4)
        || (index > DIVIDING_POINT_1 && index < DIVIDING_POINT_2)) {
        control_value /= CURVES_ATTENUATION_RATE;
    } else if (index < DIVIDING_POINT_1) {
        attenuation_rate =
            (CURVES_ATTENUATION_RATE - STRAIGHT_ATTENUATION_RATE) / TRANSITION_SECTION;
        if (index > BUFFER_BELT_2) {
            attenuation_index = index - BUFFER_BELT_2;
            attenuation = (float) attenuation_index * attenuation_rate;
            control_value /= STRAIGHT_ATTENUATION_RATE + attenuation;
        } else
            control_value /= STRAIGHT_ATTENUATION_RATE;
    } else if ((index > DIVIDING_POINT_2 && index < DIVIDING_POINT_3)) {
        attenuation_rate = (CURVES_ATTENUATION_RATE - STRAIGHT_ATTENUATION_RATE) / TRANSITION_SECTION;
        if (index < BUFFER_BELT_3) {
            attenuation_index = index - DIVIDING_POINT_2;
            attenuation = (float) attenuation_index * attenuation_rate;
            control_value /= CURVES_ATTENUATION_RATE - attenuation;
        } else if (index > BUFFER_BELT_4) {
            attenuation_index = index - BUFFER_BELT_4;
            attenuation = (float) attenuation_index * attenuation_rate;
            control_value /= STRAIGHT_ATTENUATION_RATE + attenuation;
        } else
            control_value /= STRAIGHT_ATTENUATION_RATE;
    } else {
        attenuation_rate =
            (CURVES_ATTENUATION_RATE - STRAIGHT_ATTENUATION_RATE) / TRANSITION_SECTION;     // 衰减率
        if (index < BUFFER_BELT_5) {
            attenuation_index = index - DIVIDING_POINT_4;                                   // 点数
            attenuation = (float) attenuation_index * attenuation_rate;                     // 衰减量
            control_value /= CURVES_ATTENUATION_RATE - attenuation;
        } else
            control_value /= STRAIGHT_ATTENUATION_RATE;
    }
    return control_value;
}

/*!
    \brief      LQR control algorithm
    \param[in]  index: The nth point
    \param[in]  status: Current state: including coordinates and heading angle
    \retval     The amount of control of the final servo
*/
unsigned char lqr_control(unsigned short index, basic_status_t status) {
    const double v_r = 12.5, L = 0.28;
    const float q = 1, r = 2;

    if (last_global_time_stamp == 0)
        last_global_time_stamp = global_time_stamp - 20;
    double dt = (double) (global_time_stamp - last_global_time_stamp) * 0.001;
    last_global_time_stamp = global_time_stamp;

    /* Find the error of position and heading angle */
    float yaw_temp = (status.angle < 180) ? status.angle : (status.angle - 360);
    yaw_temp *= ANGLE_TO_RADIAN;

    float x_error = status.pos_n - test_point[index][0];
    float y_error = status.pos_e - test_point[index][1];
    float yaw_error = yaw_temp - test_point[index][2];
    if (yaw_error > 3.1415926)
        yaw_error -= _2PI_;
    else if (yaw_error < -3.1415926)
        yaw_error += _2PI_;

    x_error = 0.2f * x_error;
    y_error = 0.2f * y_error;

    /* From the state equation matrix coefficients, calculate K */
    double a[3][3] = {{1, 0, -v_r * dt * sin((double) test_point[index][2])},
                      {0, 1, v_r * dt * cos((double) test_point[index][2])},
                      {0, 0, 1}};
    double b[3][2] = {{cos((double) test_point[index][2]) * dt, 0},
                      {sin((double) test_point[index][2]) * dt, 0},
                      {tan((double) test_point[index][3]) * dt / L, v_r * dt /
                          (L * cos((double) test_point[index][3]) * cos((double) test_point[index][3]))}};

    /* Obtain two control quantities: speed error and front wheel angle error */
    double x[3][1] = {{x_error},
                      {y_error},
                      {yaw_error}};
    float p[3][3] = {0};
    float control_val[2][1] = {0};

    solve_riccati_equation(a, b, q, r, p);
    solve_feedback_value(p, a, b, x, r, control_val);

    control_val[1][0] = arranging_transition_process(control_val[1][0], index);
    float output_angle = control_val[1][0] + test_point[index][3];
    unsigned char angle = (short) (SERVO_MID_POINT + output_angle * YAW_TO_ANGLE);

    /* Limit */
    if (angle > SERVO_MID_POINT + MAX_DECLINATION_ANGLE)
        angle = SERVO_MID_POINT + MAX_DECLINATION_ANGLE;
    else if (angle < SERVO_MID_POINT - MAX_DECLINATION_ANGLE)
        angle = SERVO_MID_POINT - MAX_DECLINATION_ANGLE;

    /* Servo angular acceleration limit */
    if ((last_angle > SERVO_MID_POINT)) {
        if (angle > last_angle)
            angle = (short) (
                ((angle - last_angle) > DELTA_ANGLE) ? (last_angle + DELTA_ANGLE) : (angle));
        else if (angle < SERVO_MID_POINT)
            angle = SERVO_MID_POINT;
    }
    if ((last_angle < SERVO_MID_POINT)) {
        if (angle < last_angle)
            angle = (short) ((last_angle - angle) > DELTA_ANGLE) ? (last_angle - DELTA_ANGLE) : angle;
        else if (angle > SERVO_MID_POINT)
            angle = SERVO_MID_POINT;
    }

    return angle;
}

/*!
    \brief      Addressing the nearest point using a sliding window
    \param[in]  last_index: Last found nearest point
    \retval     The point closest to its own coordinates
*/
int find_index(int last_index) {
    int nearest_index = last_index;
    for (int i = last_index; i < last_index + 10; i++) {
        if (calculate_distance(i) < calculate_distance(nearest_index)) {
            nearest_index = i;
        }
    }
    return nearest_index;
}

/*!
    \brief      Finding the nearest point using dichotomy with a sliding window
    \param[in]  ind_start: Start point of the sliding window
    \param[in]  ind_end: End point of the sliding window
    \retval     The point closest to its own coordinates
*/
int dichotomy(int ind_start, int ind_end) {
    int ind_middle, ind_middle_add, ind_middle_sub;
    for (unsigned char i = 0; i < 10; i++) {
        ind_middle = (ind_start + ind_end) / 2;
        ind_middle_add = (ind_middle + ind_end) / 2;
        ind_middle_sub = (ind_start + ind_middle) / 2;
        if (calculate_distance(ind_middle) > calculate_distance(ind_middle_add)) {
            ind_start = ind_middle;
        } else if (calculate_distance(ind_middle) > calculate_distance(ind_middle_sub)) {
            ind_end = ind_middle;
        } else
            break;
    }
    return ind_middle;
}
#else

/* 寻找点迹 */
int dichotomy(int ind_start, int ind_end, float x, float y) {
    int ind_middle, ind_middle_add, ind_middle_sub;
    for (unsigned char i = 0; i < 5; i++) {
        ind_middle = (ind_start + ind_end) / 2;
        ind_middle_add = (ind_middle + ind_end) / 2;
        ind_middle_sub = (ind_start + ind_middle) / 2;
        if (calculate_distance(ind_middle, x, y) > calculate_distance(ind_middle_add, x, y)) {
            ind_start = ind_middle;
        } else if (calculate_distance(ind_middle, x, y) > calculate_distance(ind_middle_sub, x, y)) {
            ind_end = ind_middle;
        } else
            break;
    }
    return ind_middle;
}

void track_prediction(basic_status_t *current, float v, float t, float servo_angle) {
    float l = 0.28f;
    float r = l / tanf(servo_angle);
    float theta = v * t / r;
    float s = 2 * r * sinf(0.5f * theta);
    float delta = theta / 2 + current->angle;
    current->pos_n = s * cosf(delta) + current->pos_n;
    current->pos_e = s * sinf(delta) + current->pos_e;
    current->angle = theta + current->angle;

    current->angle = (current->angle > _2PI_) ? (current->angle - _2PI_) : (
        (current->angle < 0) ? (current->angle + _2PI_) : current->angle);
}

#endif  // RUNNING_UNIT_TEST

float uabs(float value) {
    if (value < 0)
        return -1 * value;
    else
        return value;
}

void solve_feedback_value(float p[3][3], double a[3][3], double b[3][2], double x[3][1], float r, float fb[2][1]) {
    double bt_pn_b[2][2] = {{b[0][0] * (b[0][0] * p[0][0] + b[1][0] * p[1][0] + b[2][0] * p[2][0]) +
        b[1][0] * (b[0][0] * p[0][1] + b[1][0] * p[1][1] + b[2][0] * p[2][1]) +
        b[2][0] * (b[0][0] * p[0][2] + b[1][0] * p[1][2] + b[2][0] * p[2][2]),
                             b[2][1] * (b[0][0] * p[0][2] + b[1][0] * p[1][2] + b[2][0] * p[2][2])},
                            {b[0][0] * b[2][1] * p[2][0] + b[1][0] * b[2][1] * p[2][1] + b[2][0] * b[2][1] * p[2][2],
                             b[2][1] * b[2][1] * p[2][2]}};
    double bt_pn_a[2][3] = {{b[0][0] * p[0][0] + b[1][0] * p[1][0] + b[2][0] * p[2][0],
                             b[0][0] * p[0][1] + b[1][0] * p[1][1] + b[2][0] * p[2][1],
                             b[0][0] * p[0][2] + b[1][0] * p[1][2]
                                 + a[0][2] * (b[0][0] * p[0][0] + b[1][0] * p[1][0] + b[2][0] * p[2][0]) +
                                 a[1][2] * (b[0][0] * p[0][1] + b[1][0] * p[1][1] + b[2][0] * p[2][1])
                                 + b[2][0] * p[2][2]},
                            {b[2][1] * p[2][0], b[2][1] * p[2][1],
                             a[0][2] * b[2][1] * p[2][0] + a[1][2] * b[2][1] * p[2][1] + b[2][1] * p[2][2]}};
    double bt_pn_b_inv_m[2][2] = {{(bt_pn_b[1][1] + r)
                                       / (-(bt_pn_b[0][1] * bt_pn_b[1][0]) + bt_pn_b[0][0] * bt_pn_b[1][1]
                                           + bt_pn_b[0][0] * r + bt_pn_b[1][1] * r + r * r),
                                   -(bt_pn_b[0][1]
                                       / (-(bt_pn_b[0][1] * bt_pn_b[1][0]) + bt_pn_b[0][0] * bt_pn_b[1][1]
                                           + bt_pn_b[0][0] * r + bt_pn_b[1][1] * r + r * r))},
                                  {-(bt_pn_b[1][0]
                                      / (-(bt_pn_b[0][1] * bt_pn_b[1][0]) + bt_pn_b[0][0] * bt_pn_b[1][1]
                                          + bt_pn_b[0][0] * r + bt_pn_b[1][1] * r + r * r)),
                                   (bt_pn_b[0][0] + r)
                                       / (-(bt_pn_b[0][1] * bt_pn_b[1][0]) + bt_pn_b[0][0] * bt_pn_b[1][1]
                                           + bt_pn_b[0][0] * r + bt_pn_b[1][1] * r + r * r)}};
    double matrix_k[2][3] = {{bt_pn_a[0][0] * bt_pn_b_inv_m[0][0] + bt_pn_a[1][0] * bt_pn_b_inv_m[0][1],
                              bt_pn_a[0][1] * bt_pn_b_inv_m[0][0] + bt_pn_a[1][1] * bt_pn_b_inv_m[0][1],
                              bt_pn_a[0][2] * bt_pn_b_inv_m[0][0] + bt_pn_a[1][2] * bt_pn_b_inv_m[0][1]},
                             {bt_pn_a[0][0] * bt_pn_b_inv_m[1][0] + bt_pn_a[1][0] * bt_pn_b_inv_m[1][1],
                              bt_pn_a[0][1] * bt_pn_b_inv_m[1][0] + bt_pn_a[1][1] * bt_pn_b_inv_m[1][1],
                              bt_pn_a[0][2] * bt_pn_b_inv_m[1][0] + bt_pn_a[1][2] * bt_pn_b_inv_m[1][1]}};
    fb[0][0] = (float) (x[0][0] * matrix_k[0][0] + x[1][0] * matrix_k[0][1] + x[2][0] * matrix_k[0][2]);
    fb[1][0] = (float) (x[0][0] * matrix_k[1][0] + x[1][0] * matrix_k[1][1] + x[2][0] * matrix_k[1][2]);
    fb[0][0] = -1 * fb[0][0];
    fb[1][0] = -1 * fb[1][0];
}

void solve_riccati_equation(double a[3][3], double b[3][2], float q, float r, float p[3][3]) {
    float A02 = (float) a[0][2], A12 = (float) a[1][2];
    float B00 = (float) b[0][0], B10 = (float) b[1][0], B20 = (float) b[2][0], B21 = (float) b[2][1];
    float p_next[3][3] = {0};
    unsigned int counter;
    p[0][0] = q;
    p[0][1] = 0;
    p[0][2] = 0;
    p[1][0] = 0;
    p[1][1] = q;
    p[1][2] = 0;
    p[2][0] = 0;
    p[2][1] = 0;
    p[2][2] = q;

    for (counter = 0; counter < 500; ++counter) {
        float at_pn_a[3][3] = {{p[0][0], p[0][1],
                                A02 * p[0][0] + A12 * p[0][1] +
                                    p[0][2]},
                               {p[1][0], p[1][1], A02 * p[1][0] + A12 * p[1][1] +
                                   p[1][2]},
                               {A02 * p[0][0] + A12 * p[1][0] + p[2][0], A02 * p[0][1] + A12 * p[1][1] + p[2][1],
                                A02 * p[0][2] + A12 * p[1][2] + A02 *
                                    (A02 *
                                        p[0][0] +
                                        A12 *
                                            p[1][0] +
                                        p[2][0])
                                    + A12 *
                                        (A02 * p[0][1] + A12 * p[1][1] +
                                            p[2][1]) + p[2][2]}};
        float at_pn_b[3][2] = {{B00 * p[0][0] + B10 * p[0][1] + B20 * p[0][2], B21 * p[0][2]},
                               {B00 * p[1][0] + B10 * p[1][1] + B20 * p[1][2], B21 * p[1][2]},
                               {B00 * (A02 * p[0][0] + A12 * p[1][0] + p[2][0]) +
                                   B10 * (A02 * p[0][1] + A12 * p[1][1] + p[2][1])
                                    + B20 * (A02 * p[0][2] + A12 * p[1][2] + p[2][2]),
                                B21 * (A02 * p[0][2] + A12 * p[1][2] +
                                    p[2][2])}};
        float r_bt_pn_b[2][2] = {{B00 * (B00 * p[0][0] + B10 * p[1][0] + B20 * p[2][0]) +
            B10 * (B00 * p[0][1] + B10 * p[1][1] + B20 * p[2][1])
                                      + B20 * (B00 * p[0][2] + B10 * p[1][2] + B20 * p[2][2]) + r,
                                  B21 * (B00 * p[0][2] + B10 * p[1][2] + B20 * p[2][2])},
                                 {B00 * B21 * p[2][0] + B10 * B21 * p[2][1] + B20 * B21 * p[2][2],
                                  B21 * B21 * p[2][2] + r}};
        float bt_pn_a[2][3] =
            {{B00 * p[0][0] + B10 * p[1][0] + B20 * p[2][0], B00 * p[0][1] + B10 * p[1][1] + B20 * p[2][1],
              B00 * p[0][2] + B10 * p[1][2] + A02 *
                  (B00 *
                      p[0][0] +
                      B10 *
                          p[1][0] +
                      B20 *
                          p[2][0])
                  + A12 * (B00 * p[0][1] + B10 * p[1][1] +
                      B20 * p[2][1]) +
                  B20 * p[2][2]},
             {B21 * p[2][0], B21 * p[2][1], A02 * B21 * p[2][0] +
                 A12 * B21 * p[2][1] + B21 * p[2][2]}};

        float r_bt_pn_b_diag = r_bt_pn_b[0][0] * r_bt_pn_b[1][1] - r_bt_pn_b[0][1] * r_bt_pn_b[1][0];

        float r_bt_pn_b_inv[2][2] = {{r_bt_pn_b[1][1] / r_bt_pn_b_diag, -(r_bt_pn_b[0][1] / r_bt_pn_b_diag)},
                                     {-(r_bt_pn_b[1][0] / (-r_bt_pn_b_diag)), r_bt_pn_b[0][0] / r_bt_pn_b_diag}};

        float big_matrix[3][3] =
            {{bt_pn_a[0][0] * (at_pn_b[0][0] * r_bt_pn_b_inv[0][0] + at_pn_b[0][1] * r_bt_pn_b_inv[1][0]) +
                bt_pn_a[1][0] * (at_pn_b[0][0] * r_bt_pn_b_inv[0][1] + at_pn_b[0][1] * r_bt_pn_b_inv[1][1]),
              bt_pn_a[0][1] *
                  (at_pn_b[0][0] * r_bt_pn_b_inv[0][0] + at_pn_b[0][1] * r_bt_pn_b_inv[1][0]) +
                  bt_pn_a[1][1] *
                      (at_pn_b[0][0] * r_bt_pn_b_inv[0][1] + at_pn_b[0][1] * r_bt_pn_b_inv[1][1]),
              bt_pn_a[0][2] *
                  (at_pn_b[0][0] * r_bt_pn_b_inv[0][0] + at_pn_b[0][1] * r_bt_pn_b_inv[1][0]) +
                  bt_pn_a[1][2] *
                      (at_pn_b[0][0] * r_bt_pn_b_inv[0][1] + at_pn_b[0][1] * r_bt_pn_b_inv[1][1])},
             {bt_pn_a[0][0] * (at_pn_b[1][0] * r_bt_pn_b_inv[0][0] + at_pn_b[1][1] * r_bt_pn_b_inv[1][0]) +
                 bt_pn_a[1][0] * (at_pn_b[1][0] * r_bt_pn_b_inv[0][1] + at_pn_b[1][1] * r_bt_pn_b_inv[1][1]),
              bt_pn_a[0][1] *
                  (at_pn_b[1][0] * r_bt_pn_b_inv[0][0] + at_pn_b[1][1] * r_bt_pn_b_inv[1][0]) +
                  bt_pn_a[1][1] *
                      (at_pn_b[1][0] * r_bt_pn_b_inv[0][1] + at_pn_b[1][1] * r_bt_pn_b_inv[1][1]),
              bt_pn_a[0][2] *
                  (at_pn_b[1][0] * r_bt_pn_b_inv[0][0] + at_pn_b[1][1] * r_bt_pn_b_inv[1][0]) +
                  bt_pn_a[1][2] *
                      (at_pn_b[1][0] * r_bt_pn_b_inv[0][1] + at_pn_b[1][1] * r_bt_pn_b_inv[1][1])},
             {bt_pn_a[0][0] * (at_pn_b[2][0] * r_bt_pn_b_inv[0][0] + at_pn_b[2][1] * r_bt_pn_b_inv[1][0]) +
                 bt_pn_a[1][0] * (at_pn_b[2][0] * r_bt_pn_b_inv[0][1] + at_pn_b[2][1] * r_bt_pn_b_inv[1][1]),
              bt_pn_a[0][1] *
                  (at_pn_b[2][0] * r_bt_pn_b_inv[0][0] + at_pn_b[2][1] * r_bt_pn_b_inv[1][0]) +
                  bt_pn_a[1][1] *
                      (at_pn_b[2][0] * r_bt_pn_b_inv[0][1] + at_pn_b[2][1] * r_bt_pn_b_inv[1][1]),
              bt_pn_a[0][2] *
                  (at_pn_b[2][0] * r_bt_pn_b_inv[0][0] + at_pn_b[2][1] * r_bt_pn_b_inv[1][0]) +
                  bt_pn_a[1][2] *
                      (at_pn_b[2][0] * r_bt_pn_b_inv[0][1] + at_pn_b[2][1] * r_bt_pn_b_inv[1][1])}};

        p_next[0][0] = at_pn_a[0][0] - big_matrix[0][0] + q;
        p_next[0][1] = at_pn_a[0][1] - big_matrix[0][1];
        p_next[0][2] = at_pn_a[0][2] - big_matrix[0][2];
        p_next[1][0] = at_pn_a[1][0] - big_matrix[1][0];
        p_next[1][1] = at_pn_a[1][1] - big_matrix[1][1] + q;
        p_next[1][2] = at_pn_a[1][2] - big_matrix[1][2];
        p_next[2][0] = at_pn_a[2][0] - big_matrix[2][0];
        p_next[2][1] = at_pn_a[2][1] - big_matrix[2][1];
        p_next[2][2] = at_pn_a[2][2] - big_matrix[2][2] + q;

        float p_next_max_coefficient = uabs(p_next[0][0]);
        for (unsigned char counter1 = 0; counter1 < 3; ++counter1)
            for (unsigned char counter2 = 0; counter2 < 3; ++counter2)
                if (p_next_max_coefficient < uabs(p_next[counter1][counter2]))
                    p_next_max_coefficient = uabs(p_next[counter1][counter2]);

        float p_max_coefficient = uabs(p[0][0]);
        for (unsigned char counter1 = 0; counter1 < 3; ++counter1)
            for (unsigned char counter2 = 0; counter2 < 3; ++counter2)
                if (p_max_coefficient < uabs(p[counter1][counter2]))
                    p_max_coefficient = uabs(p[counter1][counter2]);

        for (unsigned char counter1 = 0; counter1 < 3; ++counter1)
            for (unsigned char counter2 = 0; counter2 < 3; ++counter2)
                p[counter1][counter2] = p_next[counter1][counter2];

//        printf("accuracy:%f\r\n", uabs(p_max_coefficient - p_next_max_coefficient));

        if (uabs(p_max_coefficient - p_next_max_coefficient) < ITERATION_ACCURACY) {

#ifndef RUNNING_UNIT_TEST
#if OUTPUT_DEBUG_INFO == 1
            printf("\r\niteration counter:%d\r\n", counter);
            _fflush();
#endif
#else
#if OUTPUT_DEBUG_INFO == 1
            printf("\r\niteration counter:%d\r\n", counter);
#endif
#endif
            break;
        }
    }
#if OUTPUT_DEBUG_INFO == 1
#ifndef RUNNING_UNIT_TEST
    if (counter > 400) {
        printf("------------Matrix a------------\r\n");
        for (int counter1 = 0; counter1 < 3; ++counter1) {
            for (int counter2 = 0; counter2 < 3; ++counter2)
                printf("%.4f   ", a[counter1][counter2]);
            printf("\r\n");
            _fflush();
        }
        delayms(40);
        printf("------------Matrix b------------\r\n");
        for (int counter1 = 0; counter1 < 3; ++counter1) {
            for (int counter2 = 0; counter2 < 2; ++counter2)
                printf("%.4f   ", b[counter1][counter2]);
            printf("\r\n");
            _fflush();
        }
        delayms(40);
    }

#else
    if (counter > 400) {
        printf("------------Matrix a------------\r\n");
        for (int counter1 = 0; counter1 < 3; ++counter1) {
            for (int counter2 = 0; counter2 < 3; ++counter2)
                printf("%.4f   ", a[counter1][counter2]);
            printf("\r\n");
        }
        printf("------------Matrix b------------\r\n");
        for (int counter1 = 0; counter1 < 3; ++counter1) {
            for (int counter2 = 0; counter2 < 2; ++counter2)
                printf("%.4f   ", b[counter1][counter2]);
            printf("\r\n");
        }
    }
#endif
#endif
#if OUTPUT_DEBUG_INFO == 1
    printf("------------Matrix P------------\r\n");
    for (int counter1 = 0; counter1 < 3; ++counter1) {
        for (int counter2 = 0; counter2 < 3; ++counter2)
            printf("%f   ", P[counter1][counter2]);
        printf("\r\n");
    }
#endif
}

