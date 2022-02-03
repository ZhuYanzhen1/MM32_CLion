/*
// solvers for Algebraic Riccati equation
// - Iteration (continuous)
// - Iteration (discrete)
// - Arimoto-Potter
//
// author: Horibe Takamasa
*/

#include "riccati_solver.h"
#include "ulapack.h"
#include "math.h"

void solveRiccatiIterationC(Matrix_t *A, Matrix_t *B,
                            Matrix_t *Q, Matrix_t *R,
                            Matrix_t *P) {
    double diff = 0;

    Matrix_t AT, BT, Rinv, P_next, P_A, AT_P, P_B_Rinv_BT_P, Temp_Matrix1, Temp_Matrix2;

    ulapack_transpose(A, &AT);
    ulapack_transpose(B, &BT);
    ulapack_inverse(R, &Rinv);

//    ulapack_print(&AT);
//    ulapack_print(&BT);
//    ulapack_print(&Rinv);

    ulapack_copy(Q, P);

    for (unsigned int counter = 0; counter < 100000; ++counter) {
        ulapack_product(P, A, &P_A);
//        ulapack_print(&P_A);

        ulapack_product(P, &AT, &AT_P);
//        ulapack_print(&AT_P);

        ulapack_product(P, B, &Temp_Matrix1);
        ulapack_product(&Temp_Matrix1, &Rinv, &P_B_Rinv_BT_P);
        ulapack_product(&P_B_Rinv_BT_P, &BT, &Temp_Matrix1);
        ulapack_product(&Temp_Matrix1, P, &P_B_Rinv_BT_P);
//        ulapack_print(&P_B_Rinv_BT_P);

        ulapack_add(&P_A, &AT_P, &Temp_Matrix1);
        ulapack_add(&Temp_Matrix1, Q, &Temp_Matrix2);
        ulapack_subtract(&Temp_Matrix2, &P_B_Rinv_BT_P, &Temp_Matrix1);
        ulapack_scale(&Temp_Matrix1, 0.001, &Temp_Matrix2);
        ulapack_add(P, &Temp_Matrix2, &P_next);
//        P_next = P + (P * A + AT * P + Q - P * B * Rinv * BT * P) * dt;

        ulapack_subtract(&P_next, P, &Temp_Matrix2);
        ulapack_max_coeff(&Temp_Matrix2, &diff);
        diff = fabs(diff);
//        printf("diff:%lf\r\n", diff);
        ulapack_copy(&P_next, P);
//        ulapack_print(&P_next);
        if (diff < 1.E-5 || diff > 1.0) {
            printf("iteration mumber = %d\r\n", counter);
            break;
        }
    }
}
