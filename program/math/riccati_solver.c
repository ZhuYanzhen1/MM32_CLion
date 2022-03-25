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

#define OUTPUT_DEBUG_INFO   0
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
    static unsigned int counter = 0;
    static float div_temp = 0;
    static float matrix_bt_pn_a[2][3] = {0};
    static float matrix_at_pn_a[3][3] = {0};
    static float matrix_at_pn_b[3][2] = {0};
    static float matrix_r_bt_pn_b[2][2] = {0};
    static float matrix_inv_r_bt_pn_b[2][2] = {0};
    float A02 = A[0][2], A12 = A[1][2], B00 = B[0][0], B10 = B[1][0], B20 = B[2][0], B21 = B[2][1];
    static float matrix_p_next[3][3] = {0};

    P[0][0] = 90;
    P[0][1] = 10;
    P[0][2] = -2;
    P[1][0] = 10;
    P[1][1] = 31;
    P[1][2] = 8;
    P[2][0] = -2;
    P[2][1] = 8;
    P[2][2] = 11;
//    for (int i = 0; i < 3; ++i)
//        for (int j = 0; j < 3; ++j)
//            P[i][j] = Q;

    for (counter = 0; counter < 100000; ++counter) {
        matrix_at_pn_a[0][0] = P[0][0];
        matrix_at_pn_a[0][1] = P[0][1];
        matrix_at_pn_a[0][2] = qfp_fadd(qfp_fadd(qfp_fmul(A02, P[0][0]),
                                                 qfp_fmul(A12, P[0][1])), P[0][2]);
        matrix_at_pn_a[1][0] = P[1][0];
        matrix_at_pn_a[1][1] = P[1][1];
        matrix_at_pn_a[1][2] = qfp_fadd(qfp_fadd(qfp_fmul(A02, P[1][0]),
                                                 qfp_fmul(A12, P[1][1])), P[1][2]);
        matrix_at_pn_a[2][0] = qfp_fadd(qfp_fadd(qfp_fmul(A02, P[0][0]),
                                                 qfp_fmul(A12, P[1][0])), P[2][0]);
        matrix_at_pn_a[2][1] = qfp_fadd(qfp_fadd(qfp_fmul(A02, P[0][1]),
                                                 qfp_fmul(A12, P[1][1])), P[2][1]);
        matrix_at_pn_a[2][2] =
            qfp_fadd(
                qfp_fadd(qfp_fadd(qfp_fmul(qfp_fmul(A02, A02), P[0][0]),
                                  qfp_fmul(qfp_fmul(A12, A12), P[1][1])),
                         qfp_fadd(qfp_fmul(A02,
                                           qfp_fadd(P[0][2],
                                                    qfp_fadd(qfp_fmul(A12, qfp_fadd(P[0][1], P[1][0])),
                                                             P[2][0]))),
                                  qfp_fmul(A12, qfp_fadd(P[1][2], P[2][1])))), P[2][2]);

        matrix_at_pn_b[0][0] = qfp_fadd(
            qfp_fadd(qfp_fmul(B00, P[0][0]), qfp_fmul(B10, P[0][1])), qfp_fmul(B20, P[0][2]));
        matrix_at_pn_b[0][1] = qfp_fmul(B21, P[0][2]);
        matrix_at_pn_b[1][0] = qfp_fadd(
            qfp_fadd(qfp_fmul(B00, P[1][0]), qfp_fmul(B10, P[1][1])), qfp_fmul(B20, P[1][2]));
        matrix_at_pn_b[1][1] = qfp_fmul(B21, P[1][2]);
        matrix_at_pn_b[2][0] = qfp_fadd(qfp_fadd(qfp_fmul(B00, qfp_fadd(qfp_fadd(qfp_fmul(A02, P[0][0]),
                                                                                 qfp_fmul(A12, P[1][0])), P[2][0])),
                                                 qfp_fmul(B10, qfp_fadd(qfp_fadd(qfp_fmul(A02, P[0][1]),
                                                                                 qfp_fmul(A12, P[1][1])), P[2][1]))),
                                        qfp_fmul(B20, qfp_fadd(qfp_fadd(qfp_fmul(A02, P[0][2]),
                                                                        qfp_fmul(A12, P[1][2])), P[2][2])));
        matrix_at_pn_b[2][1] = qfp_fmul(B21, qfp_fadd(qfp_fadd(qfp_fmul(A02, P[0][2]),
                                                               qfp_fmul(A12, P[1][2])), P[2][2]));

        matrix_r_bt_pn_b[0][0] = qfp_fadd(
            qfp_fadd(qfp_fmul(qfp_fmul(B00, B00), P[0][0]),
                     qfp_fmul(qfp_fmul(B00, B10), qfp_fadd(P[0][1], P[1][0]))),
            qfp_fadd(qfp_fadd(qfp_fmul(qfp_fmul(B10, B10), P[1][1]),
                              qfp_fmul(qfp_fmul(B00, B20), qfp_fadd(P[0][2], P[2][0]))),
                     qfp_fadd(qfp_fmul(qfp_fmul(B10, B20), qfp_fadd(P[1][2], P[2][1])),
                              qfp_fadd(qfp_fmul(qfp_fmul(B20, B20), P[2][2]), R))));
        matrix_r_bt_pn_b[0][1] = qfp_fmul(B21, qfp_fadd(qfp_fadd(qfp_fmul(B00, P[0][2]),
                                                                 qfp_fmul(B10, P[1][2])),
                                                        qfp_fmul(B20, P[2][2])));
        matrix_r_bt_pn_b[1][0] = qfp_fmul(B21, qfp_fadd(qfp_fadd(qfp_fmul(B00, P[2][0]),
                                                                 qfp_fmul(B10, P[2][1])),
                                                        qfp_fmul(B20, P[2][2])));
        matrix_r_bt_pn_b[1][1] = qfp_fadd(qfp_fmul(qfp_fmul(B21, B21), P[2][2]), R);
        matrix_bt_pn_a[0][0] = qfp_fadd(qfp_fadd(qfp_fmul(B00, P[0][0]),
                                                 qfp_fmul(B10, P[1][0])), qfp_fmul(B20, P[2][0]));
        matrix_bt_pn_a[0][1] = qfp_fadd(qfp_fadd(qfp_fmul(B00, P[0][1]),
                                                 qfp_fmul(B10, P[1][1])), qfp_fmul(B20, P[2][1]));
        matrix_bt_pn_a[0][2] = qfp_fadd(qfp_fadd(qfp_fadd(qfp_fmul(B00, P[0][2]), qfp_fmul(B10, P[1][2])),
                                                 qfp_fmul(A02,
                                                          qfp_fadd(qfp_fadd(qfp_fmul(B00, P[0][0]),
                                                                            qfp_fmul(B10, P[1][0])),
                                                                   qfp_fmul(B20, P[2][0])))),
                                        qfp_fadd(qfp_fmul(A12,
                                                          qfp_fadd(qfp_fadd(qfp_fmul(B00, P[0][1]),
                                                                            qfp_fmul(B10, P[1][1])),
                                                                   qfp_fmul(B20, P[2][1]))),
                                                 qfp_fmul(B20, P[2][2])));
        matrix_bt_pn_a[1][0] = qfp_fmul(B21, P[2][0]);
        matrix_bt_pn_a[1][1] = qfp_fmul(B21, P[2][1]);
        matrix_bt_pn_a[1][2] =
            qfp_fmul(B21, qfp_fadd(qfp_fadd(qfp_fmul(A02, P[2][0]), qfp_fmul(A12, P[2][1])),
                                   P[2][2]));

        div_temp = qfp_fadd(qfp_fmul(-matrix_r_bt_pn_b[0][1], matrix_r_bt_pn_b[1][0]),
                            qfp_fmul(matrix_r_bt_pn_b[0][0], matrix_r_bt_pn_b[1][1]));
        matrix_inv_r_bt_pn_b[0][0] = qfp_fdiv(matrix_r_bt_pn_b[1][1], div_temp);
        matrix_inv_r_bt_pn_b[0][1] = -qfp_fdiv(matrix_r_bt_pn_b[0][1], div_temp);
        matrix_inv_r_bt_pn_b[1][0] = -qfp_fdiv(matrix_r_bt_pn_b[1][0], div_temp);
        matrix_inv_r_bt_pn_b[1][1] = qfp_fdiv(matrix_r_bt_pn_b[0][0], div_temp);


//94.043884   15.471626   -2.119365
//15.471626   30.878653   8.220815
//-2.119365   8.220815   11.373928

        float temp_01 = qfp_fadd(
            qfp_fmul(matrix_at_pn_b[0][0], matrix_inv_r_bt_pn_b[0][0]),
            qfp_fmul(matrix_at_pn_b[0][1], matrix_inv_r_bt_pn_b[1][0]));
        float temp_02 = qfp_fadd(
            qfp_fmul(matrix_at_pn_b[0][0], matrix_inv_r_bt_pn_b[0][1]),
            qfp_fmul(matrix_at_pn_b[0][1], matrix_inv_r_bt_pn_b[1][1]));
        float temp_11 = qfp_fadd(
            qfp_fmul(matrix_at_pn_b[1][0], matrix_inv_r_bt_pn_b[0][0]),
            qfp_fmul(matrix_at_pn_b[1][1], matrix_inv_r_bt_pn_b[1][0]));
        float temp_12 = qfp_fadd(
            qfp_fmul(matrix_at_pn_b[1][0], matrix_inv_r_bt_pn_b[0][1]),
            qfp_fmul(matrix_at_pn_b[1][1], matrix_inv_r_bt_pn_b[1][1]));
        float temp_21 = qfp_fadd(
            qfp_fmul(matrix_at_pn_b[2][0], matrix_inv_r_bt_pn_b[0][0]),
            qfp_fmul(matrix_at_pn_b[2][1], matrix_inv_r_bt_pn_b[1][0]));
        float temp_22 = qfp_fadd(
            qfp_fmul(matrix_at_pn_b[2][0], matrix_inv_r_bt_pn_b[0][1]),
            qfp_fmul(matrix_at_pn_b[2][1], matrix_inv_r_bt_pn_b[1][1]));

        float another_matrix[3][3];
        another_matrix[0][0] = qfp_fadd(qfp_fmul(temp_01, matrix_bt_pn_a[0][0]),
                                        qfp_fmul(temp_02, matrix_bt_pn_a[1][0]));
        another_matrix[0][1] = qfp_fadd(qfp_fmul(temp_01, matrix_bt_pn_a[0][1]),
                                        qfp_fmul(temp_02, matrix_bt_pn_a[1][1]));
        another_matrix[0][2] = qfp_fadd(qfp_fmul(temp_01, matrix_bt_pn_a[0][2]),
                                        qfp_fmul(temp_02, matrix_bt_pn_a[1][2]));
        another_matrix[1][0] = qfp_fadd(qfp_fmul(temp_11, matrix_bt_pn_a[0][0]),
                                        qfp_fmul(temp_12, matrix_bt_pn_a[1][0]));
        another_matrix[1][1] = qfp_fadd(qfp_fmul(temp_11, matrix_bt_pn_a[0][1]),
                                        qfp_fmul(temp_12, matrix_bt_pn_a[1][1]));
        another_matrix[1][2] = qfp_fadd(qfp_fmul(temp_11, matrix_bt_pn_a[0][2]),
                                        qfp_fmul(temp_12, matrix_bt_pn_a[1][2]));
        another_matrix[2][0] = qfp_fadd(qfp_fmul(temp_21, matrix_bt_pn_a[0][0]),
                                        qfp_fmul(temp_22, matrix_bt_pn_a[1][0]));
        another_matrix[2][1] = qfp_fadd(qfp_fmul(temp_21, matrix_bt_pn_a[0][1]),
                                        qfp_fmul(temp_22, matrix_bt_pn_a[1][1]));
        another_matrix[2][2] = qfp_fadd(qfp_fmul(temp_21, matrix_bt_pn_a[0][2]),
                                        qfp_fmul(temp_22, matrix_bt_pn_a[1][2]));

        matrix_p_next[0][0] = qfp_fadd(qfp_fsub(matrix_at_pn_a[0][0], another_matrix[0][0]), Q);
        matrix_p_next[0][1] = qfp_fsub(matrix_at_pn_a[0][1], another_matrix[0][1]);
        matrix_p_next[0][2] = qfp_fsub(matrix_at_pn_a[0][2], another_matrix[0][2]);
        matrix_p_next[1][0] = qfp_fsub(matrix_at_pn_a[1][0], another_matrix[1][0]);
        matrix_p_next[1][1] = qfp_fadd(qfp_fsub(matrix_at_pn_a[1][1], another_matrix[1][1]), Q);
        matrix_p_next[1][2] = qfp_fsub(matrix_at_pn_a[1][2], another_matrix[1][2]);
        matrix_p_next[2][0] = qfp_fsub(matrix_at_pn_a[2][0], another_matrix[2][0]);
        matrix_p_next[2][1] = qfp_fsub(matrix_at_pn_a[2][1], another_matrix[2][1]);
        matrix_p_next[2][2] = qfp_fadd(qfp_fsub(matrix_at_pn_a[2][2], another_matrix[2][2]), Q);

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
    printf("\r\niteration counter:%d\r\n", counter);
#if OUTPUT_DEBUG_INFO == 1
    printf("\r\niteration counter:%d\r\n", counter);
    printf("------------Matrix P------------\r\n");
    for (int counter1 = 0; counter1 < 3; ++counter1) {
        for (int counter2 = 0; counter2 < 3; ++counter2)
            printf("%f   ", P[counter1][counter2]);
        printf("\r\n");
    }
#endif
}
