#include "riccati_solver.h"
#include "ulapack.h"
#include "math.h"

void solveRiccatiIteration(Matrix_t *A, Matrix_t *B,
                           Matrix_t *Q, Matrix_t *R,
                           Matrix_t *P) {
    double diff = 0;

    Matrix_t AT, BT, P_next, P_Differ, AT_Pn, AT_Pn_A, Q_AT_Pn_A;
    Matrix_t AT_Pn_B, BT_Pn, BT_Pn_A, B_Pn_B, R_BT_Pn_B, R_BT_Pn_B_inv;
    Matrix_t AT_Pn_B_R_BT_Pn_B_inv, AT_Pn_B_R_BT_Pn_B_inv_BT_Pn_A;

    ulapack_init(&P_Differ, 4, 4);
    ulapack_transpose(A, &AT);
    ulapack_transpose(B, &BT);
    for (unsigned int counter = 0; counter < 100000; ++counter) {
        ulapack_product(&AT, P, &AT_Pn);
        ulapack_product(&AT_Pn, A, &AT_Pn_A);
        ulapack_add(Q, &AT_Pn_A, &Q_AT_Pn_A);

        ulapack_product(&AT_Pn, B, &AT_Pn_B);

        ulapack_product(&BT, P, &BT_Pn);
        ulapack_product(&BT_Pn, A, &BT_Pn_A);

        ulapack_product(&BT_Pn, B, &B_Pn_B);
        ulapack_add(R, &B_Pn_B, &R_BT_Pn_B);
        ulapack_inverse(&R_BT_Pn_B, &R_BT_Pn_B_inv);

        ulapack_product(&AT_Pn_B, &R_BT_Pn_B_inv, &AT_Pn_B_R_BT_Pn_B_inv);
        ulapack_product(&AT_Pn_B_R_BT_Pn_B_inv, &BT_Pn_A, &AT_Pn_B_R_BT_Pn_B_inv_BT_Pn_A);
        ulapack_subtract(&Q_AT_Pn_A, &AT_Pn_B_R_BT_Pn_B_inv_BT_Pn_A, &P_next);
//        P_next = AT * P * A - AT * P * B * (R + BT * P * B).inverse() * BT * P * A + Q;
//        ulapack_print(&BT_Pn);

        ulapack_subtract(&P_next, P, &P_Differ);
        ulapack_max_coeff(&P_Differ, &diff);
        diff = fabs(diff);
        ulapack_copy(&P_next, P);
        if (diff < 1.E-5 || diff > 1000000.0) {
//            printf("iteration mumber = %d\r\n", counter);
//            printf("diff: %f\r\n", diff);
            break;
        }
    }
}
