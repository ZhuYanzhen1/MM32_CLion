#include "solver_test.h"
#include "CUnit/Basic.h"

#include "math.h"
#include "riccati_solver.h"

#include "mag_calibrate.h"
#include "stdio.h"
#include "string.h"

#include "test_data.h"

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

#define TOLERANCE_PRECISION     0.1f

void test_riccati_solver(void) {
    float fai_r = 0.23f, delta_r = 0.086f, L = 0.4f, v_r = 5.1f, dt = 0.01f;
    float error_x = 1.2f, error_y = 0.4f, error_fai = 0.17f;
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

    CU_ASSERT(fabsf(control_val[0][0]) < fabsf(-1.2583f - TOLERANCE_PRECISION))
    CU_ASSERT(fabsf(control_val[0][0]) > fabsf(-1.2583f + TOLERANCE_PRECISION))
    CU_ASSERT(fabsf(control_val[1][0]) < fabsf(-0.3026f - TOLERANCE_PRECISION))
    CU_ASSERT(fabsf(control_val[1][0]) > fabsf(-0.3026f + TOLERANCE_PRECISION))
}

#define MAX_ITERATION   5
#define MAX_ERROR       0.01f
#define INDEX_OFFSET    500
#define DISTANCE_OFFSET 3.6f

float distance_north = 25.5f;
float distance_east = 5.7f;

float calculate_distance(int ind) {
    float distance = sqrtf(
            ((reference_point[ind][0] - distance_north) * (reference_point[ind][0] - distance_north)
             + (reference_point[ind][1] - distance_east) * (reference_point[ind][1] - distance_east)));
    return distance;
}

int Newton_Iteration(int ind_last) {
    int ind_next = 0;
    float f_derivative;
    for (unsigned char i = 0; i < MAX_ITERATION; i++) {
        f_derivative = calculate_distance(ind_last) - calculate_distance(ind_last - 1);
        ind_next = ind_last - (int) ((calculate_distance(ind_last) - DISTANCE_OFFSET) / f_derivative);
        ind_last = ind_next;
        if (fabsf(calculate_distance(ind_last) - DISTANCE_OFFSET) < MAX_ERROR)
            break;
    }
    return ind_next;
}

void test_calc_target_index(void) {
    int ind = 1000;
    int ind_last_ahead = (int) Newton_Iteration(ind + INDEX_OFFSET);
    int ind_next_rear = (int) Newton_Iteration(ind - INDEX_OFFSET);
    ind = (ind_last_ahead + ind_next_rear) / 2;
    (void) ind;
}
