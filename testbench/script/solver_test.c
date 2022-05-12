#include "solver_test.h"
#include "CUnit/Basic.h"

#include "math.h"
#include "riccati_solver.h"
#include "mag_calibrate.h"
#include "stdio.h"
#include "string.h"

#define TOLERANCE_PRECISION     (0.2f)

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
    float fai_r = 1.700996560758021f, delta_r = 0.0015f, L = 0.28f, v_r = 3.0f, dt = 0.03f;
//    float error_x = 1.2f, error_y = 0.4f, error_fai = 0.17f;
    float error_x = 0, error_y = 0, error_fai = -0.5f;
    float a[3][3] = {{1, 0, -v_r * dt * sinf(fai_r)},
                     {0, 1, v_r * dt * cosf(fai_r)},
                     {0, 0, 1}};

    float b[3][2] = {{cosf(fai_r) * dt,       0},
                     {sinf(fai_r) * dt,       0},
                     {tanf(delta_r) * dt / L, v_r * dt / (L * cosf(delta_r) * cosf(delta_r))}};
    float x[3][1] = {{error_x},
                     {error_y},
                     {error_fai}};
    float p[3][3] = {0};
    float control_val[2][1] = {0};
    float r = 1;
    float q = 1;

    solve_riccati_equation(a, b, q, r, p);
    solve_feedback_value(p, a, b, x, r, control_val);

    printf("\r\n\r\nangle: %f\r\n\r\n", control_val[1][0] + delta_r);
//    CU_ASSERT(fabsf(control_val[0][0]) < fabsf(-1.2583f - TOLERANCE_PRECISION))
//    CU_ASSERT(fabsf(control_val[0][0]) > fabsf(-1.2583f + TOLERANCE_PRECISION))
//    CU_ASSERT(fabsf(control_val[1][0]) < fabsf(-0.3026f - TOLERANCE_PRECISION))
//    CU_ASSERT(fabsf(control_val[1][0]) > fabsf(-0.3026f + TOLERANCE_PRECISION))
}

#ifndef USING_GTK_GUI_MACRO
#include "test_data.h"
float calculate_distance(int ind, float x, float y) {
    float distance = (sqrtf
        ((test_point_1[ind][0] - x)
             * (test_point_1[ind][0] - x)
             + (test_point_1[ind][1] - y) *
                 (test_point_1[ind][1] - y)));
    return distance;
}
#endif
