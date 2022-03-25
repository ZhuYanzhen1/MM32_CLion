#include "riccati_solver.h"
#include "qfplib.h"

#ifndef RUNNING_UNIT_TEST
#include "printf.h"
#else

#include "stdio.h"

#endif

//  P = 96.7181   16.2375   -2.0601
//      16.2375   31.0814    8.2303
//      -2.0601    8.2303   11.3719

#define OUTPUT_DEBUG_INFO   1
#define ITERATION_ACCURACY  0.05f

/*
------------Matrix P------------
94.043884   15.471626   -2.119365
15.471626   30.878653   8.220815
-2.119365   8.220815   11.373928
 */

float uabs(float value) {
    if (value < 0)
        return -1 * value;
    else
        return value;
}

void solveRiccatiIteration(float A[3][3], float B[3][2], float Q, float R, float P[3][3]) {
    float A02 = A[0][2], A12 = A[1][2], B00 = B[0][0], B10 = B[1][0], B20 = B[2][0], B21 = B[2][1];
    float p_next[3][3] = {0};
    P[0][0] = 90;
    P[0][1] = 10;
    P[0][2] = -2;
    P[1][0] = 10;
    P[1][1] = 31;
    P[1][2] = 8;
    P[2][0] = -2;
    P[2][1] = 8;
    P[2][2] = 11;

    for (unsigned int counter = 0; counter < 100000; ++counter) {
        float at_pn_a[3][3] = {{P[0][0], P[0][1], A02 * P[0][0] + A12 * P[0][1] + P[0][2]},
                               {P[1][0], P[1][1], A02 * P[1][0] + A12 * P[1][1] + P[1][2]},
                               {A02 * P[0][0] + A12 * P[1][0] + P[2][0], A02 * P[0][1] + A12 * P[1][1] + P[2][1],
                                A02 * P[0][2] + A12 * P[1][2] + A02 * (A02 * P[0][0] + A12 * P[1][0] + P[2][0]) +
                                    A12 * (A02 * P[0][1] + A12 * P[1][1] + P[2][1]) + P[2][2]}};
        float at_pn_b[3][2] = {{B00 * P[0][0] + B10 * P[0][1] + B20 * P[0][2], B21 * P[0][2]},
                               {B00 * P[1][0] + B10 * P[1][1] + B20 * P[1][2], B21 * P[1][2]},
                               {B00 * (A02 * P[0][0] + A12 * P[1][0] + P[2][0]) +
                                   B10 * (A02 * P[0][1] + A12 * P[1][1] + P[2][1]) +
                                   B20 * (A02 * P[0][2] + A12 * P[1][2] + P[2][2]),
                                B21 * (A02 * P[0][2] + A12 * P[1][2] + P[2][2])}};
        float r_bt_pn_b[2][2] = {{B00 * (B00 * P[0][0] + B10 * P[1][0] + B20 * P[2][0]) +
            B10 * (B00 * P[0][1] + B10 * P[1][1] + B20 * P[2][1])
                                      + B20 * (B00 * P[0][2] + B10 * P[1][2] + B20 * P[2][2]) + R,
                                  B21 * (B00 * P[0][2] + B10 * P[1][2] + B20 * P[2][2])},
                                 {B00 * B21 * P[2][0] + B10 * B21 * P[2][1] + B20 * B21 * P[2][2],
                                  B21 * B21 * P[2][2] + R}};
        float bt_pn_a[2][3] =
            {{B00 * P[0][0] + B10 * P[1][0] + B20 * P[2][0], B00 * P[0][1] + B10 * P[1][1] + B20 * P[2][1],
              B00 * P[0][2] + B10 * P[1][2] + A02 * (B00 * P[0][0] + B10 * P[1][0] + B20 * P[2][0]) +
                  A12 * (B00 * P[0][1] + B10 * P[1][1] + B20 * P[2][1]) + B20 * P[2][2]},
             {B21 * P[2][0], B21 * P[2][1], A02 * B21 * P[2][0] + A12 * B21 * P[2][1] + B21 * P[2][2]}};

        float r_bt_pn_b_diag = r_bt_pn_b[0][0] * r_bt_pn_b[1][1] - r_bt_pn_b[0][1] * r_bt_pn_b[1][0];

        float r_bt_pn_b_inv[2][2] = {{r_bt_pn_b[1][1] / r_bt_pn_b_diag, -(r_bt_pn_b[0][1] / r_bt_pn_b_diag)},
                                     {-(r_bt_pn_b[1][0] / (-r_bt_pn_b_diag)), r_bt_pn_b[0][0] / r_bt_pn_b_diag}};

        float big_matrix[3][3] =
            {{bt_pn_a[0][0] * (at_pn_b[0][0] * r_bt_pn_b_inv[0][0] + at_pn_b[0][1] * r_bt_pn_b_inv[1][0]) +
                bt_pn_a[1][0] * (at_pn_b[0][0] * r_bt_pn_b_inv[0][1] + at_pn_b[0][1] * r_bt_pn_b_inv[1][1]),
              bt_pn_a[0][1] * (at_pn_b[0][0] * r_bt_pn_b_inv[0][0] + at_pn_b[0][1] * r_bt_pn_b_inv[1][0]) +
                  bt_pn_a[1][1] * (at_pn_b[0][0] * r_bt_pn_b_inv[0][1] + at_pn_b[0][1] * r_bt_pn_b_inv[1][1]),
              bt_pn_a[0][2] * (at_pn_b[0][0] * r_bt_pn_b_inv[0][0] + at_pn_b[0][1] * r_bt_pn_b_inv[1][0]) +
                  bt_pn_a[1][2] * (at_pn_b[0][0] * r_bt_pn_b_inv[0][1] + at_pn_b[0][1] * r_bt_pn_b_inv[1][1])},
             {bt_pn_a[0][0] * (at_pn_b[1][0] * r_bt_pn_b_inv[0][0] + at_pn_b[1][1] * r_bt_pn_b_inv[1][0]) +
                 bt_pn_a[1][0] * (at_pn_b[1][0] * r_bt_pn_b_inv[0][1] + at_pn_b[1][1] * r_bt_pn_b_inv[1][1]),
              bt_pn_a[0][1] * (at_pn_b[1][0] * r_bt_pn_b_inv[0][0] + at_pn_b[1][1] * r_bt_pn_b_inv[1][0]) +
                  bt_pn_a[1][1] * (at_pn_b[1][0] * r_bt_pn_b_inv[0][1] + at_pn_b[1][1] * r_bt_pn_b_inv[1][1]),
              bt_pn_a[0][2] * (at_pn_b[1][0] * r_bt_pn_b_inv[0][0] + at_pn_b[1][1] * r_bt_pn_b_inv[1][0]) +
                  bt_pn_a[1][2] * (at_pn_b[1][0] * r_bt_pn_b_inv[0][1] + at_pn_b[1][1] * r_bt_pn_b_inv[1][1])},
             {bt_pn_a[0][0] * (at_pn_b[2][0] * r_bt_pn_b_inv[0][0] + at_pn_b[2][1] * r_bt_pn_b_inv[1][0]) +
                 bt_pn_a[1][0] * (at_pn_b[2][0] * r_bt_pn_b_inv[0][1] + at_pn_b[2][1] * r_bt_pn_b_inv[1][1]),
              bt_pn_a[0][1] * (at_pn_b[2][0] * r_bt_pn_b_inv[0][0] + at_pn_b[2][1] * r_bt_pn_b_inv[1][0]) +
                  bt_pn_a[1][1] * (at_pn_b[2][0] * r_bt_pn_b_inv[0][1] + at_pn_b[2][1] * r_bt_pn_b_inv[1][1]),
              bt_pn_a[0][2] * (at_pn_b[2][0] * r_bt_pn_b_inv[0][0] + at_pn_b[2][1] * r_bt_pn_b_inv[1][0]) +
                  bt_pn_a[1][2] * (at_pn_b[2][0] * r_bt_pn_b_inv[0][1] + at_pn_b[2][1] * r_bt_pn_b_inv[1][1])}};

        p_next[0][0] = at_pn_a[0][0] - big_matrix[0][0] + Q;
        p_next[0][1] = at_pn_a[0][1] - big_matrix[0][1];
        p_next[0][2] = at_pn_a[0][2] - big_matrix[0][2];
        p_next[1][0] = at_pn_a[1][0] - big_matrix[1][0];
        p_next[1][1] = at_pn_a[1][1] - big_matrix[1][1] + Q;
        p_next[1][2] = at_pn_a[1][2] - big_matrix[1][2];
        p_next[2][0] = at_pn_a[2][0] - big_matrix[2][0];
        p_next[2][1] = at_pn_a[2][1] - big_matrix[2][1];
        p_next[2][2] = at_pn_a[2][2] - big_matrix[2][2] + Q;

        float p_next_max_coefficient = uabs(p_next[0][0]);
        for (unsigned char counter1 = 0; counter1 < 3; ++counter1)
            for (unsigned char counter2 = 0; counter2 < 3; ++counter2)
                if (p_next_max_coefficient < uabs(p_next[counter1][counter2]))
                    p_next_max_coefficient = uabs(p_next[counter1][counter2]);

        float p_max_coefficient = uabs(P[0][0]);
        for (unsigned char counter1 = 0; counter1 < 3; ++counter1)
            for (unsigned char counter2 = 0; counter2 < 3; ++counter2)
                if (p_max_coefficient < uabs(P[counter1][counter2]))
                    p_max_coefficient = uabs(P[counter1][counter2]);

        if (uabs(p_max_coefficient - p_next_max_coefficient) < ITERATION_ACCURACY) {
#if OUTPUT_DEBUG_INFO == 1
            printf("\r\niteration counter:%d\r\n", counter);
#endif
            break;
        }

        for (unsigned char counter1 = 0; counter1 < 3; ++counter1)
            for (unsigned char counter2 = 0; counter2 < 3; ++counter2)
                P[counter1][counter2] = p_next[counter1][counter2];
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
