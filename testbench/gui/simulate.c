//
// Created by 16625 on 2022-05-03.
//

#include "simulate.h"
#include "math.h"
#include "test_data.h"
#include "riccati_solver.h"

#define dt          0.02f
#define L           0.28f
#define refSpeed    3
#define ANGLE_TO_RADIAN         (0.0174533f)
#define YAW_TO_ANGLE            (-63.66203f)     // 180/pi * (-50/47.5)

void
lqr_update(float refDelta, float Delta_delta, float v_delta, float *x, float *y, float *yaw, float *v, float *Delta,
           float angle) {
    (*Delta) = angle > 150 ? (200 - angle) : (angle - 100);
    (*Delta) /= YAW_TO_ANGLE;
    (*x) = (*x) + (*v) * cosf((*yaw)) * dt;
    (*y) = (*y) + (*v) * sinf((*yaw)) * dt;
    (*yaw) = (*yaw) + (*v) / L * tanf((*Delta)) * dt;
    (*v) = refSpeed + v_delta;
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

    unsigned int time = 0;
    while (index < INDEX_NUM - 1) {
        if (index < INDEX_NUM)
            index = dichotomy(((index - 2) <= 0) ? 0 : (index - 2),
                              (index + INDEX_OFFSET > INDEX_NUM) ?
                              INDEX_NUM : (index + INDEX_OFFSET), *distance_n, *distance_e);

        float x_error = (*distance_n) - test_point_1[index][0];
        float y_error = (*distance_e) - test_point_1[index][1];
        float yaw_error = (*angle_yaw) - test_point_1[index][2];

        // 计算横向误差
        float lateral_error = y_error * cosf(test_point_1[index][2]) - x_error * sinf(test_point_1[index][2]);
        if (abs((int) lateral_error) > 1)
            break;

        // 由状态方程矩阵系数，计算K
        double a[3][3] = {{1, 0, -v_r * dt * sinf(test_point_1[index][2])},
                          {0, 1, v_r * dt * cosf(test_point_1[index][2])},
                          {0, 0, 1}};
        double b[3][2] = {{cosf(test_point_1[index][2]) * dt, 0},
                          {sinf(test_point_1[index][2]) * dt, 0},
                          {tanf(test_point_1[index][3]) * dt / L, v_r * dt /
                              (L * cosf(test_point_1[index][3]) *
                                  cosf(test_point_1[index][3]))}};

        // 获得速度误差量、前轮转角误差量两个控制量
        double x[3][1] = {{x_error},
                          {y_error},
                          {yaw_error}};
        float p[3][3] = {0};
        float control_val[2][1] = {0};
        float q = 1;
        float r = 1;

        solve_riccati_equation(a, b, q, r, p);
        solve_feedback_value(p, a, b, x, r, control_val);
        float v_delta = control_val[0][0];
        float Delta_delta = control_val[1][0];
        angle = (short) (150 + (control_val[1][0] + test_point_1[index][3]) * YAW_TO_ANGLE);

        lqr_update(test_point_1[index][3], Delta_delta, v_delta, distance_n, distance_e, angle_yaw, v, Delta, angle);
        pos[time][0] = *distance_n;
        pos[time][1] = *distance_e;
        time++;
        max_pos++;
        if (time > 8000)
            break;
        if (index > INDEX_NUM - 1)
            break;
    }
}
