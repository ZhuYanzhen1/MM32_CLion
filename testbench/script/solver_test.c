#include "solver_test.h"
#include "CUnit/Basic.h"

#include "math.h"
#include "riccati_solver.h"

#include "mag_calibrate.h"
#include "stdio.h"
#include "string.h"

extern calpara_t params;

void test_calibration_solver(void) {
    float sample[3];
    FILE *infile;
    char calflg = 0;
    int return_char;
    char line_counter;
    char StrLine[1024] = {0};

    infile = fopen("../magfile.txt", "r");
    if (infile == NULL) {
        fclose(infile);
        CU_FAIL("Mag Measurement data file does not exit!")
    } else {
        while (1) {
            return_char = fgetc(infile);
            if (calflg != 0 || feof(infile))
                break;
            if (return_char != '\n') {
                StrLine[line_counter] = (char) return_char;
                line_counter++;
            } else {
                sscanf(StrLine, "%f %f %f", &sample[0], &sample[1], &sample[2]);
                calflg = CompassCal(sample);
                memset(StrLine, 0x00, sizeof(StrLine));
                line_counter = 0;
            }
        }
        fclose(infile);

        if (calflg == 1) CU_PASS("Calibrate success.\r\n")
        else CU_FAIL("Calibrate failed, please retry.\r\n")

//        printf("diag_x = %lf  diag_y = %lf  diag_z = %lf\r\n", params.diag[0], params.diag[1],
//               params.diag[2]);
//        printf("offdiag_x = %lf  offdiag_y = %lf  offdiag_z = %lf\r\n", params.offdiag[0],
//               params.offdiag[1], params.offdiag[2]);
//        printf("offset_x = %lf  offset_y = %lf  offset_z = %lf\r\n", params.offset[0], params.offset[1],
//               params.offset[2]);
//        printf("radius = %lf\r\n", params.radius);
    }
}

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
