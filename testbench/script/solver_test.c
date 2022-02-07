#include "solver_test.h"
#include "CUnit/Basic.h"

#include "math.h"
#include "riccati_solver.h"

void test_riccati_solver(void) {
    double fai_r = 0.23, delta_r = 0.086, L = 0.4, v_r = 5.1, dt = 0.1;
    double Matrix_A[3][3] = {{1, 0, -v_r * sin(fai_r)},
                             {0, 1, v_r * dt * cos(fai_r)},
                             {0, 0, 1}};
    double Matrix_B[3][2] = {{cos(fai_r), 0},
                             {sin(fai_r), 0},
                             {tan(delta_r) / L, v_r / (L * cos(delta_r) * cos(delta_r))}};
    double Matrix_Q[3][3] = {{1, 0, 0},
                             {0, 1, 0},
                             {0, 0, 1}};
    double Matrix_R[2][2] = {{1, 0},
                             {0, 1}};
    solveRiccatiIteration(Matrix_A, Matrix_B, Matrix_Q, Matrix_R);
//    CU_PASS("Riccati Solver")
}
