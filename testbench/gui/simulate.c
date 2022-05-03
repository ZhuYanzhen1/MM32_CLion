//
// Created by 16625 on 2022-05-03.
//

#include "simulate.h"
#include "math.h"
#include "stdlib.h"

#define dt  0.02f
#define pi  3.1415926f
#define L   0.28f
#define refSpeed    3

void
lqr_update(float refDelta, float Delta_delta, float v_delta, float *x, float *y, float *yaw, float *v, float *Delta) {
    float delta_static_error = 0.1f;                    // 舵机打角的静态误差
    float v_delta_max = 4 * dt;                         // 最大加速度
    float Delta_delta_max = 132.86f * pi / 180 * dt;    // 舵机打角最大加速度
    float expect_delta = refDelta + Delta_delta;        // 舵机期望打角
    float max_expect_delta = 95 * pi / 180;             // 舵机最大打角

    // 考虑可能出现的打滑现象
    float offset = 0, r = L / fabsf(tanf((*Delta))), a = 0, F_centripetaL = 0, m = 1, f_max_static = 5;

    if (r != 0) {
        F_centripetaL = m * (*v) * (*v) / r;
        a = f_max_static - F_centripetaL;
        if (a < 0)
            offset = -a * dt * dt / 2;
        else
            offset = 0;
    }

    // 考虑车模难以走直线的问题，给舵机打角一个静态的偏差值
    expect_delta = expect_delta + delta_static_error;
    if (expect_delta > pi)                  // 取余
        expect_delta = expect_delta - 2 * pi;
    if (expect_delta > max_expect_delta)    // 限制舵机期望打角范围
        expect_delta = max_expect_delta;
    else if (expect_delta < -max_expect_delta)
        expect_delta = -max_expect_delta;

    // 得到舵机最终打角
    if (fabsf(expect_delta - (*Delta)) > Delta_delta_max) {
        if (expect_delta > (*Delta))
            (*Delta) = (*Delta) + Delta_delta_max;
        else
            (*Delta) = (*Delta) - Delta_delta_max;
    } else
        (*Delta) = expect_delta;

    // 考虑智能车的最大加速度
    if (v_delta > v_delta_max)
        v_delta = v_delta_max;
    if ((*v) > refSpeed)
        (*v) = refSpeed + v_delta;
    else
        (*v) = (*v) + v_delta;

    if ((*v) > 2)
        (*v) = (*v) + rand() % 2 - 2;

    (*x) = (*x) + (*v) * cosf((*yaw)) * dt;
    (*y) = (*y) + (*v) * sinf((*yaw)) * dt;
    (*yaw) = (*yaw) + (*v) / L * tanf((*Delta)) * dt;

    // 考虑可能出现的打滑现象
    if ((*Delta) > 0) {
        (*x) = (*x) + sinf((*yaw)) * offset;
        (*y) = (*y) - cosf((*yaw)) * offset;
    } else {
        (*x) = (*x) - sinf((*yaw)) * offset;
        (*y) = (*y) + cosf((*yaw)) * offset;
    }
}
