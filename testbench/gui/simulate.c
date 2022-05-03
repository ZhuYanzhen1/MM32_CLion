//
// Created by 16625 on 2022-05-03.
//

#include "simulate.h"
#include "math.h"
#include "stdlib.h"
#include "simulate.h"
#include "test_data.h"
#include "riccati_solver.h"

#define dt  0.02f
#define pi  3.1415926f
#define L   0.28f
#define refSpeed    3
#define ANGLE_TO_RADIAN         (0.0174533f)
#define YAW_TO_ANGLE            (-63.66203f)     // 180/pi * (-50/47.5)

void
lqr_update(float refDelta, float Delta_delta, float v_delta, float *x, float *y, float *yaw, float *v, float *Delta,
           float angle) {
//    float delta_static_error = 0.1f;                    // 舵机打角的静态误差
//    float v_delta_max = 4 * dt;                         // 最大加速度
//    float Delta_delta_max = 132.86f * pi / 180 * dt;    // 舵机打角最大加速度
//    float expect_delta = refDelta + Delta_delta;        // 舵机期望打角
//    float max_expect_delta = 95 * pi / 180;             // 舵机最大打角
//
//    // 考虑可能出现的打滑现象
//    float offset = 0, a = 0, F_centripetaL = 0, m = 1, f_max_static = 5;
//    float r = L / fabsf(tanf((*Delta)));
//    if (r != 0) {
//        F_centripetaL = m * (*v) * (*v) / r;
//        a = f_max_static - F_centripetaL;
//        if (a < 0)
//            offset = -a * dt * dt / 2;
//        else
//            offset = 0;
//    }

    // 考虑车模难以走直线的问题，给舵机打角一个静态的偏差值
//    expect_delta = expect_delta + delta_static_error;
//    if (expect_delta > pi)                  // 取余
//        expect_delta = expect_delta - 2 * pi;
//    if (expect_delta > max_expect_delta)    // 限制舵机期望打角范围
//        expect_delta = max_expect_delta;
//    else if (expect_delta < -max_expect_delta)
//        expect_delta = -max_expect_delta;
//
//    // 得到舵机最终打角
//    if (fabsf(expect_delta - (*Delta)) > Delta_delta_max) {
//        if (expect_delta > (*Delta))
//            (*Delta) = (*Delta) + Delta_delta_max;
//        else
//            (*Delta) = (*Delta) - Delta_delta_max;
//    } else
//        (*Delta) = expect_delta;
//
//    // 考虑智能车的最大加速度
//    if (v_delta > v_delta_max)
//        v_delta = v_delta_max;
//    if ((*v) > refSpeed)
//        (*v) = refSpeed + v_delta;
//    else
//        (*v) = (*v) + v_delta;

//    if ((*v) > 2)
//        (*v) = (*v) + rand() % 2 - 2;

//    (*Delta) = refDelta + Delta_delta;
    (*Delta) = angle > 150 ? (200 - angle) : (angle - 100);
    (*Delta) /= YAW_TO_ANGLE;
    (*x) = (*x) + (*v) * cosf((*yaw)) * dt;
    (*y) = (*y) + (*v) * sinf((*yaw)) * dt;
    (*yaw) = (*yaw) + (*v) / L * tanf((*Delta)) * dt;
    (*v) = refSpeed + v_delta;
    // 考虑可能出现的打滑现象
//    if ((*Delta) > 0) {
//        (*x) = (*x) + sinf((*yaw)) * offset;
//        (*y) = (*y) - cosf((*yaw)) * offset;
//    } else {
//        (*x) = (*x) - sinf((*yaw)) * offset;
//        (*y) = (*y) + cosf((*yaw)) * offset;
//    }
}

float calculate_distance(int ind, float x, float y) {
    float distance = (sqrtf
            ((test_point_1[ind][0] - x)
             * (test_point_1[ind][0] - x)
             + (test_point_1[ind][1] - y) *
               (test_point_1[ind][1] - y)));
    return distance;
}

float pos[100000][2] = {0};
float angle;
unsigned int max_pos = 0;

void simulate_lqr_control(void) {
    unsigned short index = 0;
    float dn = test_point_1[0][0] - 0.5f, de = test_point_1[0][1] + 0.5f,
            ay = test_point_1[0][2] - 0.02f, speed = 0, delta = 0;
    float *distance_n = &dn, *distance_e = &de, *angle_yaw = &ay, *v = &speed, *Delta = &delta;
    float v_r = refSpeed;


//    for (int time = 0; time < 1000; ++time) {
    unsigned int time = 0;
    while (index < INDEX_NUM - 1) {
        if (index < INDEX_NUM)
            index = dichotomy(((index - 2) <= 0) ? 0 : (index - 2),
                              (index + INDEX_OFFSET > INDEX_NUM) ?
                              INDEX_NUM : (index + INDEX_OFFSET), *distance_n, *distance_e);

        // 求位置、航向角的误差
//        float yaw_temp = ((*angle_yaw) < 360) ? (*angle_yaw) : ((*angle_yaw) - 360);
//        yaw_temp *= ANGLE_TO_RADIAN;

        float x_error = (*distance_n) - test_point_1[index][0];
        float y_error = (*distance_e) - test_point_1[index][1];
        float yaw_error = (*angle_yaw) - test_point_1[index][2];

        // 计算横向误差
        float lateral_error = y_error * cosf(test_point_1[index][2]) - x_error * sinf(test_point_1[index][2]);
        if (abs((int) lateral_error) > 1)
            break;

        // 由状态方程矩阵系数，计算K
        float a[3][3] = {{1, 0, -v_r * dt * sinf(test_point_1[index][2])},
                         {0, 1, v_r * dt * cosf(test_point_1[index][2])},
                         {0, 0, 1}};
        float b[3][2] = {{cosf(test_point_1[index][2]) * dt,     0},
                         {sinf(test_point_1[index][2]) * dt,     0},
                         {tanf(test_point_1[index][3]) * dt / L, v_r * dt /
                                                                 (L * cosf(test_point_1[index][3]) *
                                                                  cosf(test_point_1[index][3]))}};

        // 获得速度误差量、前轮转角误差量两个控制量
        float X[3][1] = {{x_error},
                         {y_error},
                         {yaw_error}};
        float p[3][3] = {0};
        float control_val[2][1] = {0};
        float q = 1;
        float r = 1;

        solve_riccati_equation(a, b, q, r, p);
        solve_feedback_value(p, a, b, X, r, control_val);
        float v_delta = control_val[0][0];
        float Delta_delta = control_val[1][0];
        angle = (short) (150 + (control_val[1][0] + test_point_1[index][3]) * YAW_TO_ANGLE);

        lqr_update(test_point_1[index][3], Delta_delta, v_delta, distance_n, distance_e, angle_yaw, v, Delta, angle);
        pos[time][0] = *distance_n;
        pos[time][1] = *distance_e;
        time++;
        max_pos++;
        if (time > 6000)
            break;
        if (index > INDEX_NUM - 1)
            break;
    }
}
