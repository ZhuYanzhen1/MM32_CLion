#include "solver_test.h"

#include "ulapack.h"
#include "stdio.h"
#include "riccati_solver.h"

#define DIM_X 4
#define DIM_U 1

void test_riccati_solver(void) {
    Matrix_t A, B, Q, R, P;
    ulapack_init(&A, DIM_X, DIM_X);
    ulapack_init(&B, DIM_X, DIM_U);
    ulapack_init(&Q, DIM_X, DIM_X);
    ulapack_init(&R, DIM_U, DIM_U);
    ulapack_init(&P, DIM_X, DIM_X);

    ulapack_edit_entry(&A, 0, 1, 1.0f);
    ulapack_edit_entry(&A, 0, 1, 1.0f);
    ulapack_edit_entry(&A, 1, 1, -15.0f);
    ulapack_edit_entry(&A, 1, 2, 10.0f);
    ulapack_edit_entry(&A, 2, 3, 1.0f);
    ulapack_edit_entry(&A, 3, 3, -15.0f);

    ulapack_edit_entry(&B, 1, 0, 10.0f);
    ulapack_edit_entry(&B, 3, 0, 1.0f);

    ulapack_edit_entry(&Q, 0, 0, 1.0f);
    ulapack_edit_entry(&Q, 2, 2, 1.0f);
    ulapack_edit_entry(&Q, 3, 3, 2.0f);

    ulapack_edit_entry(&R, 0, 0, 1.0f);

    Matrix_t I, Ad, Bd, A_Scale, I_Add_A_Scale, I_Sub_A_Scale, I_A_Scale_Product;
    ulapack_init(&I, DIM_X, DIM_X);
    ulapack_init(&Ad, DIM_X, DIM_X);
    ulapack_init(&Bd, DIM_X, DIM_X);
    ulapack_eye(&I);

    ulapack_scale(&A, 0.0005, &A_Scale);
    ulapack_add(&I, &A_Scale, &I_Add_A_Scale);
    ulapack_subtract(&I, &A_Scale, &I_Sub_A_Scale);
    ulapack_product(&I_Add_A_Scale, &I_Sub_A_Scale, &I_A_Scale_Product);
    ulapack_inverse(&I_A_Scale_Product, &Ad);
//    Ad = (I + 0.5 * dt * A) * (I - 0.5 * dt * A).inverse();
    ulapack_scale(&B, 0.001, &Bd);

    /* == iteration based Riccati solution (continuous) == */
    printf("-- Iteration based method (continuous) --\r\n");
    solveRiccatiIteration(&Ad, &Bd, &Q, &R, &P);
    ulapack_print(&P);
}
