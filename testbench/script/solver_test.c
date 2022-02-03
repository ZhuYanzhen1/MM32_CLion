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

    /* == iteration based Riccati solution (continuous) == */
    printf("-- Iteration based method (continuous) --\r\n");
    solveRiccatiIterationC(&A, &B, &Q, &R, &P);
    ulapack_print(&P);
}
