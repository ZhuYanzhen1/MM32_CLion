#include "riccati_solver.h"

#ifndef RUNNING_UNIT_TEST
#include "printf.h"
#else

#include "stdio.h"

#endif

//  P = 96.7181   16.2375   -2.0601
//      16.2375   31.0814    8.2303
//      -2.0601    8.2303   11.3719

#define OUTPUT_DEBUG_INFO   1
#define ITERATION_ACCURACY  0.01f

float uabs(float value) {
    if (value < 0)
        return -1 * value;
    else
        return value;
}

void solveRiccatiIteration(float A[3][3], float B[3][2], float Q[3][3], float R[2][2], float P[3][3]) {
    float matrix_p_next[3][3] = {0};
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            P[i][j] = Q[i][j];

    for (unsigned int counter = 0; counter < 100000; ++counter) {
        float matrix_at_pn[3][3] = {{A[0][0] * P[0][0],                                         A[0][0] * P[0][1],
                                                                                                         A[0][0] *
                                                                                                         P[0][2]},
                                    {A[1][1] * P[1][0],                                         A[1][1] *
                                                                                                P[1][1], A[1][1] *
                                                                                                         P[1][2]},
                                    {A[0][2] * P[0][0] + A[1][2] * P[1][0] + A[2][2] * P[2][0], A[0][2] * P[0][1] +
                                                                                                A[1][2] * P[1][1] +
                                                                                                A[2][2] *
                                                                                                P[2][1], A[0][2] *
                                                                                                         P[0][2] +
                                                                                                         A[1][2] *
                                                                                                         P[1][2] +
                                                                                                         A[2][2] *
                                                                                                         P[2][2]}};
        float matrix_bt_pn[2][3] = {{B[0][0] * P[0][0] + B[1][0] * P[1][0] + B[2][0] * P[2][0],
                                                        B[0][0] * P[0][1] + B[1][0] * P[1][1] + B[2][0] * P[2][1],
                                                                           B[0][0] * P[0][2] + B[1][0] * P[1][2] +
                                                                           B[2][0] * P[2][2]},
                                    {B[2][1] * P[2][0], B[2][1] * P[2][1], B[2][1] * P[2][2]}};
        float matrix_at_pn_a[3][3] = {{A[0][0] * matrix_at_pn[0][0], A[1][1] * matrix_at_pn[0][1],
                                                                                                   A[0][2] *
                                                                                                   matrix_at_pn[0][0] +
                                                                                                   A[1][2] *
                                                                                                   matrix_at_pn[0][1] +
                                                                                                   A[2][2] *
                                                                                                   matrix_at_pn[0][2]},
                                      {A[0][0] * matrix_at_pn[1][0], A[1][1] * matrix_at_pn[1][1], A[0][2] *
                                                                                                   matrix_at_pn[1][0] +
                                                                                                   A[1][2] *
                                                                                                   matrix_at_pn[1][1] +
                                                                                                   A[2][2] *
                                                                                                   matrix_at_pn[1][2]},
                                      {A[0][0] * matrix_at_pn[2][0], A[1][1] * matrix_at_pn[2][1], A[0][2] *
                                                                                                   matrix_at_pn[2][0] +
                                                                                                   A[1][2] *
                                                                                                   matrix_at_pn[2][1] +
                                                                                                   A[2][2] *
                                                                                                   matrix_at_pn[2][2]}};
        float matrix_at_pn_b[3][2] = {{matrix_at_pn[0][0] * B[0][0] + matrix_at_pn[0][1] * B[1][0] +
                                       matrix_at_pn[0][2] * B[2][0], matrix_at_pn[0][2] * B[2][1]},
                                      {matrix_at_pn[1][0] * B[0][0] + matrix_at_pn[1][1] * B[1][0] +
                                       matrix_at_pn[1][2] * B[2][0], matrix_at_pn[1][2] * B[2][1]},
                                      {matrix_at_pn[2][0] * B[0][0] + matrix_at_pn[2][1] * B[1][0] +
                                       matrix_at_pn[2][2] * B[2][0], matrix_at_pn[2][2] * B[2][1]}};
        float matrix_r_bt_pn_b[2][2] = {{B[0][0] * matrix_bt_pn[0][0] + B[1][0] * matrix_bt_pn[0][1] +
                                         B[2][0] * matrix_bt_pn[0][2] + R[0][0],
                                                                       B[2][1] *
                                                                       matrix_bt_pn[0][2]},
                                        {B[0][0] * matrix_bt_pn[1][0] + B[1][0] * matrix_bt_pn[1][1] +
                                         B[2][0] * matrix_bt_pn[1][2], B[2][1] *
                                                                       matrix_bt_pn[1][2] +
                                                                       R[0][0]}};
        float matrix_bt_pn_a[2][3] = {{A[0][0] * matrix_bt_pn[0][0], A[1][1] * matrix_bt_pn[0][1],
                                                                                                   A[0][2] *
                                                                                                   matrix_bt_pn[0][0] +
                                                                                                   A[1][2] *
                                                                                                   matrix_bt_pn[0][1] +
                                                                                                   A[2][2] *
                                                                                                   matrix_bt_pn[0][2]},
                                      {A[0][0] * matrix_bt_pn[1][0], A[1][1] * matrix_bt_pn[1][1], A[0][2] *
                                                                                                   matrix_bt_pn[1][0] +
                                                                                                   A[1][2] *
                                                                                                   matrix_bt_pn[1][1] +
                                                                                                   A[2][2] *
                                                                                                   matrix_bt_pn[1][2]}};
        float matrix_inv_r_bt_pn_b[2][2] =
            {{matrix_r_bt_pn_b[1][1]
              / (-matrix_r_bt_pn_b[0][1] * matrix_r_bt_pn_b[1][0]
                 + matrix_r_bt_pn_b[0][0] * matrix_r_bt_pn_b[1][1]), -(matrix_r_bt_pn_b[0][1]
                                                                       / (-matrix_r_bt_pn_b[0][1] *
                                                                          matrix_r_bt_pn_b[1][0]
                                                                          + matrix_r_bt_pn_b[0][0] *
                                                                            matrix_r_bt_pn_b[1][1]))},
             {-(matrix_r_bt_pn_b[1][0]
                / (-matrix_r_bt_pn_b[0][1] * matrix_r_bt_pn_b[1][0]
                   + matrix_r_bt_pn_b[0][0] * matrix_r_bt_pn_b[1][1])),
                                                                     matrix_r_bt_pn_b[0][0] /
                                                                     (-matrix_r_bt_pn_b[0][1] *
                                                                      matrix_r_bt_pn_b[1][0]
                                                                      + matrix_r_bt_pn_b[0][0] *
                                                                        matrix_r_bt_pn_b[1][1])}};
        float another_matrix[3][3] =
            {{(matrix_at_pn_b[0][0] * matrix_inv_r_bt_pn_b[0][0]
               + matrix_at_pn_b[0][1] * matrix_inv_r_bt_pn_b[1][0]) * matrix_bt_pn_a[0][0]
              + (matrix_at_pn_b[0][0] * matrix_inv_r_bt_pn_b[0][1]
                 + matrix_at_pn_b[0][1] * matrix_inv_r_bt_pn_b[1][1]) * matrix_bt_pn_a[1][0],
                 (matrix_at_pn_b[0][0] * matrix_inv_r_bt_pn_b[0][0] +
                  matrix_at_pn_b[0][1] * matrix_inv_r_bt_pn_b[1][0]) * matrix_bt_pn_a[0][1]
                 + (matrix_at_pn_b[0][0] * matrix_inv_r_bt_pn_b[0][1]
                    + matrix_at_pn_b[0][1] * matrix_inv_r_bt_pn_b[1][1]) * matrix_bt_pn_a[1][1],
                 (matrix_at_pn_b[0][0] * matrix_inv_r_bt_pn_b[0][0] +
                  matrix_at_pn_b[0][1] * matrix_inv_r_bt_pn_b[1][0]) * matrix_bt_pn_a[0][2]
                 + (matrix_at_pn_b[0][0] * matrix_inv_r_bt_pn_b[0][1]
                    + matrix_at_pn_b[0][1] * matrix_inv_r_bt_pn_b[1][1]) * matrix_bt_pn_a[1][2]},
             {(matrix_at_pn_b[1][0] * matrix_inv_r_bt_pn_b[0][0] +
               matrix_at_pn_b[1][1] * matrix_inv_r_bt_pn_b[1][0]) * matrix_bt_pn_a[0][0]
              + (matrix_at_pn_b[1][0] * matrix_inv_r_bt_pn_b[0][1]
                 + matrix_at_pn_b[1][1] * matrix_inv_r_bt_pn_b[1][1]) * matrix_bt_pn_a[1][0],
                 (matrix_at_pn_b[1][0] * matrix_inv_r_bt_pn_b[0][0] +
                  matrix_at_pn_b[1][1] * matrix_inv_r_bt_pn_b[1][0]) * matrix_bt_pn_a[0][1]
                 + (matrix_at_pn_b[1][0] * matrix_inv_r_bt_pn_b[0][1]
                    + matrix_at_pn_b[1][1] * matrix_inv_r_bt_pn_b[1][1]) * matrix_bt_pn_a[1][1],
                 (matrix_at_pn_b[1][0] * matrix_inv_r_bt_pn_b[0][0] +
                  matrix_at_pn_b[1][1] * matrix_inv_r_bt_pn_b[1][0]) * matrix_bt_pn_a[0][2]
                 + (matrix_at_pn_b[1][0] * matrix_inv_r_bt_pn_b[0][1]
                    + matrix_at_pn_b[1][1] * matrix_inv_r_bt_pn_b[1][1]) * matrix_bt_pn_a[1][2]},
             {(matrix_at_pn_b[2][0] * matrix_inv_r_bt_pn_b[0][0] +
               matrix_at_pn_b[2][1] * matrix_inv_r_bt_pn_b[1][0]) * matrix_bt_pn_a[0][0]
              + (matrix_at_pn_b[2][0] * matrix_inv_r_bt_pn_b[0][1]
                 + matrix_at_pn_b[2][1] * matrix_inv_r_bt_pn_b[1][1]) * matrix_bt_pn_a[1][0],
                 (matrix_at_pn_b[2][0] * matrix_inv_r_bt_pn_b[0][0] +
                  matrix_at_pn_b[2][1] * matrix_inv_r_bt_pn_b[1][0]) * matrix_bt_pn_a[0][1]
                 + (matrix_at_pn_b[2][0] * matrix_inv_r_bt_pn_b[0][1]
                    + matrix_at_pn_b[2][1] * matrix_inv_r_bt_pn_b[1][1]) * matrix_bt_pn_a[1][1],
                 (matrix_at_pn_b[2][0] * matrix_inv_r_bt_pn_b[0][0] +
                  matrix_at_pn_b[2][1] * matrix_inv_r_bt_pn_b[1][0]) * matrix_bt_pn_a[0][2]
                 + (matrix_at_pn_b[2][0] * matrix_inv_r_bt_pn_b[0][1]
                    + matrix_at_pn_b[2][1] * matrix_inv_r_bt_pn_b[1][1]) * matrix_bt_pn_a[1][2]}};
        matrix_p_next[0][0] = matrix_at_pn_a[0][0] - another_matrix[0][0] + Q[0][0];
        matrix_p_next[0][1] = matrix_at_pn_a[0][1] - another_matrix[0][1];
        matrix_p_next[0][2] = matrix_at_pn_a[0][2] - another_matrix[0][2];
        matrix_p_next[1][0] = matrix_at_pn_a[1][0] - another_matrix[1][0];
        matrix_p_next[1][1] = matrix_at_pn_a[1][1] - another_matrix[1][1] + Q[0][0];
        matrix_p_next[1][2] = matrix_at_pn_a[1][2] - another_matrix[1][2];
        matrix_p_next[2][0] = matrix_at_pn_a[2][0] - another_matrix[2][0];
        matrix_p_next[2][1] = matrix_at_pn_a[2][1] - another_matrix[2][1];
        matrix_p_next[2][2] = matrix_at_pn_a[2][2] - another_matrix[2][2] + Q[0][0];

        float p_next_max_coefficient = uabs(matrix_p_next[0][0]);
        for (unsigned char counter1 = 0; counter1 < 3; ++counter1)
            for (unsigned char counter2 = 0; counter2 < 3; ++counter2)
                if (p_next_max_coefficient < uabs(matrix_p_next[counter1][counter2]))
                    p_next_max_coefficient = uabs(matrix_p_next[counter1][counter2]);

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
                P[counter1][counter2] = matrix_p_next[counter1][counter2];
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
