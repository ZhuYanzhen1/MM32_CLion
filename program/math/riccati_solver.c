#include "riccati_solver.h"
#include "fast_math.h"
#include "test.h"

#ifndef RUNNING_UNIT_TEST
#include "printf.h"
#include "qfplib.h"

#include "kalman.h"
#include "sensor_decode.h"
#else

#include "stdio.h"
#include "math.h"
#include "test_data.h"

#endif

//  P = 96.7181   16.2375   -2.0601
//      16.2375   31.0814    8.2303
//      -2.0601    8.2303   11.3719


#define YAW_TO_ANGLE        (60.3111346f)     // 180/pi * (-50/47.5)
#define OUTPUT_DEBUG_INFO   0
#define ITERATION_ACCURACY  0.1f

#ifdef RUNNING_UNIT_TEST
float distance_north = 25.5f;
float distance_east = 5.7f;

float calculate_distance(int ind) {
    float distance = sqrtf(
        ((playground_index[ind][0] - distance_north) * (playground_index[ind][0] - distance_north)
         + (playground_index[ind][1] - distance_east) * (playground_index[ind][1] - distance_east)));
    return distance;
}


void lqr_control_test(unsigned short index, int angle_ctrl, float angle_yaw, float distance_n, float distance_e) {

    float v_r = 2, dt = 0.02f, L = 0.3f;

    // 求位置、航向角的误差
    float yaw_temp = (angle_yaw < 180) ? angle_yaw : (180 - angle_yaw);
    yaw_temp *= (0.0174533f);
    float x_error = distance_n - test_point[index][0];
    float y_error = distance_e - test_point[index][1];
    float yaw_error = yaw_temp - test_point[index][2];

    // 计算横向误差
//    float lateral_error = y_error * qfp_fcos(reference_point[index][2]) - x_error * qfp_fsin(reference_point[index][2]);

    // 由状态方程矩阵系数，计算K
    float a[3][3] = {{1, 0, -v_r * dt * sinf(reference_point[index][2])},
                     {0, 1, v_r * dt * cosf(reference_point[index][2])},
                     {0, 0, 1}};
    float b[3][2] = {{cosf(reference_point[index][2]) * dt, 0},
                     {sinf(reference_point[index][2]) * dt, 0},
                     {tanf(yaw_error) * dt / L,             v_r * dt /
                                                            (L * cosf(yaw_error) * cosf(yaw_error))}};

    // 获得速度误差量、前轮转角误差量两个控制量
    float x[3][1] = {{x_error},
                     {y_error},
                     {yaw_error}};
    float p[3][3] = {0};
    float control_val[2][1] = {0};
    float r = 1;
    float q = 1;

    solve_riccati_equation(a, b, q, r, p);
    solve_feedback_value(p, a, b, x, r, control_val);
//    speed = speed +control_val[0][0];
    short delta_angle = (short) (control_val[1][0] * YAW_TO_ANGLE);
    if (delta_angle > 162 || delta_angle < -162)
        return;
    angle_ctrl = angle_ctrl + delta_angle;
    if (angle_ctrl > 200)
        angle_ctrl = 200;
    else if (angle_ctrl < 100)
        angle_ctrl = 100;
}

#else

extern volatile unsigned short speed;
extern volatile short angle;

#ifdef IS_PROCESS_MCU
extern kalman_data_t kalman_data;
float calculate_distance(int ind) {
//    proc_data.distance_north = kalman_data.distance_north;
//    proc_data.distance_east = kalman_data.distance_east;

    float distance = (qfp_fsqrt
        ((reference_point[ind][0] - kalman_data.distance_north)
             * (reference_point[ind][0] - kalman_data.distance_north)
             + (reference_point[ind][1] - kalman_data.distance_east) *
                 (reference_point[ind][1] - kalman_data.distance_east)));
    return distance;
}
#else
float calculate_distance(int ind) {
    /* 国防生 */
    //    float distance = (qfp_fsqrt
    //        ((reference_point[ind][0] - proc_data.distance_north)
    //             * (reference_point[ind][0] - proc_data.distance_north)
    //             + (reference_point[ind][1] - proc_data.distance_east) *
    //                 (reference_point[ind][1] - proc_data.distance_east)));
    //    return distance;
    /* 工一楼顶 */
    float distance = (qfp_fsqrt
        ((test_point[ind][0] - proc_data.distance_north)
             * (test_point[ind][0] - proc_data.distance_north)
             + (test_point[ind][1] - proc_data.distance_east) *
                 (test_point[ind][1] - proc_data.distance_east)));
    return distance;
}

void lqr_control(unsigned short index) {

    float v_r = 2, dt = 0.02f, L = 0.3f;

    // 求位置、航向角的误差
    float yaw_temp = (proc_data.north_angle < 180) ? proc_data.north_angle : (180 - proc_data.north_angle);
    yaw_temp *= (0.0174533f);

    /* 国防生 */
//    float x_error = proc_data.distance_north - reference_point[index][0];
//    float y_error = proc_data.distance_east - reference_point[index][1];
//    float yaw_error = yaw_temp - reference_point[index][2];

    /* 工一楼顶 */
    float x_error = proc_data.distance_north - test_point[index][0];
    float y_error = proc_data.distance_east - test_point[index][1];
    float yaw_error = yaw_temp - test_point[index][2];

    // 计算横向误差
    //    float lateral_error = y_error * qfp_fcos(reference_point[index][2]) - x_error * qfp_fsin(reference_point[index][2]);

    // 由状态方程矩阵系数，计算K
    float a[3][3] = {{1, 0, -v_r * dt * qfp_fsin(reference_point[index][2])},
                     {0, 1, v_r * dt * qfp_fcos(reference_point[index][2])},
                     {0, 0, 1}};
    float b[3][2] = {{qfp_fcos(reference_point[index][2]) * dt, 0},
                     {qfp_fsin(reference_point[index][2]) * dt, 0},
                     {qfp_ftan(yaw_error) * dt / L, v_r * dt /
                         (L * qfp_fcos(yaw_error) * qfp_fcos(yaw_error))}};

    // 获得速度误差量、前轮转角误差量两个控制量
    float x[3][1] = {{x_error},
                     {y_error},
                     {yaw_error}};
    float p[3][3] = {0};
    float control_val[2][1] = {0};
    float r = 1;
    float q = 1;

    solve_riccati_equation(a, b, q, r, p);
    solve_feedback_value(p, a, b, x, r, control_val);
    //    speed = speed +control_val[0][0];
    short delta_angle = (short) (control_val[1][0] * YAW_TO_ANGLE);
//    if (delta_angle > 162 || delta_angle < -162)
//        return;
    angle = angle + delta_angle;
    if (angle > 200)
        angle = 200;
    else if (angle < 100)
        angle = 100;
}

#endif  // IS_PROCESS_MCU

#endif  // RUNNING_UNIT_TEST

float low_pass_filter_angle(float input, float last_output) {
    float a = 0.1f;
    return (a * input + (1 - a) * last_output);
}

/* 对轨迹进行滤波，使其更加平滑 */
void low_pass_filter(unsigned short length) {
    float a = 0.1f;
    for (unsigned char i = 0; i < 2; i++) {
        for (unsigned short j = 1; j < length; j++) {
            reference_point[i][j] = a * reference_point[i][j] + (1 - a) * reference_point[i][j - 1];
        }
    }
}

/* 寻找点迹 */
int dichotomy(int ind_start, int ind_end) {
    int ind_middle, ind_middle_add, ind_middle_sub;
    for (unsigned char i = 0; i < 5; i++) {
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

float uabs(float value) {
    if (value < 0)
        return -1 * value;
    else
        return value;
}

void solve_feedback_value(float p[3][3], float a[3][3], float b[3][2], float x[3][1], float r, float fb[2][1]) {
    float bt_pn_b[2][2] = {{b[0][0] * (b[0][0] * p[0][0] + b[1][0] * p[1][0] + b[2][0] * p[2][0]) +
                            b[1][0] * (b[0][0] * p[0][1] + b[1][0] * p[1][1] + b[2][0] * p[2][1]) +
                            b[2][0] * (b[0][0] * p[0][2] + b[1][0] * p[1][2] + b[2][0] * p[2][2]),
                                                                                                   b[2][1] *
                                                                                                   (b[0][0] *
                                                                                                    p[0][2] +
                                                                                                    b[1][0] *
                                                                                                    p[1][2] +
                                                                                                    b[2][0] *
                                                                                                    p[2][2])},
                           {b[0][0] * b[2][1] * p[2][0] + b[1][0] * b[2][1]
                                                          * p[2][1] + b[2][0] * b[2][1] * p[2][2], b[2][1] *
                                                                                                   b[2][1] *
                                                                                                   p[2][2]}};
    float bt_pn_a[2][3] = {{b[0][0] * p[0][0] + b[1][0] * p[1][0] + b[2][0] * p[2][0],
                                               b[0][0] * p[0][1] + b[1][0] * p[1][1] + b[2][0] * p[2][1],
                               b[0][0] * p[0][2] + b[1][0] * p[1][2]
                               + a[0][2] * (b[0][0] * p[0][0] + b[1][0] * p[1][0] + b[2][0] * p[2][0]) +
                               a[1][2] * (b[0][0] * p[0][1] + b[1][0] * p[1][1] + b[2][0] * p[2][1])
                               + b[2][0] * p[2][2]},
                           {b[2][1] * p[2][0], b[2][1] * p[2][1],
                               a[0][2] * b[2][1] * p[2][0] + a[1][2] * b[2][1] * p[2][1] + b[2][1] * p[2][2]}};
    float bt_pn_b_diag = -(bt_pn_b[0][1] * bt_pn_b[1][0]) + bt_pn_b[0][0] * bt_pn_b[1][1] +
                         bt_pn_b[0][0] * r + bt_pn_b[1][1] * r + r * r;
    float bt_pn_b_inv_m[2][2] = {{-((bt_pn_b[1][1] + r) / (bt_pn_b_diag)), bt_pn_b[0][1] / (bt_pn_b_diag)},
                                 {bt_pn_b[1][0] / (bt_pn_b_diag),          -((bt_pn_b[0][0] + r) /
                                                                             (bt_pn_b_diag))}};
    float matrix_k[2][3] = {{bt_pn_a[0][0] * bt_pn_b_inv_m[0][0] + bt_pn_a[1][0] * bt_pn_b_inv_m[0][1],
                                bt_pn_a[0][1] * bt_pn_b_inv_m[0][0] + bt_pn_a[1][1] * bt_pn_b_inv_m[0][1],
                                bt_pn_a[0][2] * bt_pn_b_inv_m[0][0] + bt_pn_a[1][2] * bt_pn_b_inv_m[0][1]},
                            {bt_pn_a[0][0] * bt_pn_b_inv_m[1][0] + bt_pn_a[1][0] * bt_pn_b_inv_m[1][1],
                                bt_pn_a[0][1] * bt_pn_b_inv_m[1][0] + bt_pn_a[1][1] * bt_pn_b_inv_m[1][1],
                                bt_pn_a[0][2] * bt_pn_b_inv_m[1][0] + bt_pn_a[1][2] * bt_pn_b_inv_m[1][1]}};
    fb[0][0] = x[0][0] * matrix_k[0][0] + x[1][0] * matrix_k[0][1] + x[2][0] * matrix_k[0][2];
    fb[1][0] = x[0][0] * matrix_k[1][0] + x[1][0] * matrix_k[1][1] + x[2][0] * matrix_k[1][2];
}

void solve_riccati_equation(float a[3][3], float b[3][2], float q, float r, float p[3][3]) {
    float A02 = a[0][2], A12 = a[1][2], B00 = b[0][0], B10 = b[1][0], B20 = b[2][0], B21 = b[2][1];
    float p_next[3][3] = {0};
    p[0][0] = q;
    p[0][1] = 0;
    p[0][2] = 0;
    p[1][0] = 0;
    p[1][1] = q;
    p[1][2] = 0;
    p[2][0] = 0;
    p[2][1] = 0;
    p[2][2] = q;

    for (unsigned int counter = 0; counter < 500; ++counter) {
        float at_pn_a[3][3] = {{p[0][0],                                 p[0][1],
                                                                                  A02 * p[0][0] + A12 * p[0][1] +
                                                                                  p[0][2]},
                               {p[1][0],                                 p[1][1], A02 * p[1][0] + A12 * p[1][1] +
                                                                                  p[1][2]},
                               {A02 * p[0][0] + A12 * p[1][0] + p[2][0], A02 * p[0][1] + A12 * p[1][1] + p[2][1],
                                                                                  A02 * p[0][2] + A12 * p[1][2] +
                                                                                  A02 *
                                                                                  (A02 *
                                                                                   p[0][0] +
                                                                                   A12 *
                                                                                   p[1][0] +
                                                                                   p[2][0]) +
                                                                                  A12 *
                                                                                  (A02 * p[0][1] + A12 * p[1][1] +
                                                                                   p[2][1]) + p[2][2]}};
        float at_pn_b[3][2] = {{B00 * p[0][0] + B10 * p[0][1] + B20 * p[0][2], B21 * p[0][2]},
                               {B00 * p[1][0] + B10 * p[1][1] + B20 * p[1][2], B21 * p[1][2]},
                               {B00 * (A02 * p[0][0] + A12 * p[1][0] + p[2][0]) +
                                B10 * (A02 * p[0][1] + A12 * p[1][1] + p[2][1]) +
                                B20 * (A02 * p[0][2] + A12 * p[1][2] + p[2][2]),
                                                                               B21 *
                                                                               (A02 * p[0][2] + A12 * p[1][2] +
                                                                                p[2][2])}};
        float r_bt_pn_b[2][2] = {{B00 * (B00 * p[0][0] + B10 * p[1][0] + B20 * p[2][0]) +
                                  B10 * (B00 * p[0][1] + B10 * p[1][1] + B20 * p[2][1])
                                  + B20 * (B00 * p[0][2] + B10 * p[1][2] + B20 * p[2][2]) + r,
                                     B21 * (B00 * p[0][2] + B10 * p[1][2] + B20 * p[2][2])},
                                 {B00 * B21 * p[2][0] + B10 * B21 * p[2][1] + B20 * B21 * p[2][2],
                                     B21 * B21 * p[2][2] + r}};
        float bt_pn_a[2][3] =
            {{B00 * p[0][0] + B10 * p[1][0] + B20 * p[2][0], B00 * p[0][1] + B10 * p[1][1] + B20 * p[2][1],
                                                                            B00 * p[0][2] + B10 * p[1][2] +
                                                                            A02 *
                                                                            (B00 *
                                                                             p[0][0] +
                                                                             B10 *
                                                                             p[1][0] +
                                                                             B20 *
                                                                             p[2][0]) +
                                                                            A12 *
                                                                            (B00 * p[0][1] + B10 * p[1][1] +
                                                                             B20 * p[2][1]) + B20 * p[2][2]},
             {B21 * p[2][0],                                 B21 * p[2][1], A02 * B21 * p[2][0] +
                                                                            A12 * B21 * p[2][1] +
                                                                            B21 * p[2][2]}};

        float r_bt_pn_b_diag = r_bt_pn_b[0][0] * r_bt_pn_b[1][1] - r_bt_pn_b[0][1] * r_bt_pn_b[1][0];

        float r_bt_pn_b_inv[2][2] = {{r_bt_pn_b[1][1] / r_bt_pn_b_diag,       -(r_bt_pn_b[0][1] / r_bt_pn_b_diag)},
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

        if (uabs(p_max_coefficient - p_next_max_coefficient) < ITERATION_ACCURACY) {
#if OUTPUT_DEBUG_INFO == 1
            printf("\r\niteration counter:%d\r\n", counter);
#endif
            printf("\r\niteration counter:%d\r\n", counter);
            _fflush();
            break;
        }

        for (unsigned char counter1 = 0; counter1 < 3; ++counter1)
            for (unsigned char counter2 = 0; counter2 < 3; ++counter2)
                p[counter1][counter2] = p_next[counter1][counter2];
    }
#if OUTPUT_DEBUG_INFO == 1
    printf("------------Matrix P------------\r\n");
    for (int counter1 = 0; counter1 < 3; ++counter1) {
        for (int counter2 = 0; counter2 < 3; ++counter2)
            printf("%f   ", P[counter1][counter2]);
        printf("\r\n");
    }
#endif
}

