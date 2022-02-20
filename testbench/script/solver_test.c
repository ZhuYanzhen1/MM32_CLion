#include "solver_test.h"
#include "CUnit/Basic.h"

#include "math.h"
#include "riccati_solver.h"

void test_riccati_solver(void) {
    float fai_r = 0.23f, delta_r = 0.086f, L = 0.4f, v_r = 5.1f, dt = 0.01f;
    float Matrix_A[3][3] = {{1, 0, -v_r * dt * sinf(fai_r)},
                            {0, 1, v_r * dt * cosf(fai_r)},
                            {0, 0, 1}};
    float Matrix_B[3][2] = {{cosf(fai_r) * dt,       0},
                            {sinf(fai_r) * dt,       0},
                            {tanf(delta_r) * dt / L, v_r * dt / (L * cosf(delta_r) * cosf(delta_r))}};
    float Matrix_Q[3][3] = {{1, 0, 0},
                            {0, 1, 0},
                            {0, 0, 1}};
    float Matrix_R[2][2] = {{1, 0},
                            {0, 1}};
    solveRiccatiIteration(Matrix_A, Matrix_B, Matrix_Q, Matrix_R);
    CU_PASS("Riccati solver solved.");
}
