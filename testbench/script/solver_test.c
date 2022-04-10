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

#define TOLERANCE_PRECISION     0.2f

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

extern float distance_north;
extern float distance_east;

void test_calc_target_index(void) {
    int ind;
    int ind_ahead;
    int ind_rear;
//    ind = 1000;
//    ind_ahead = (int) newton_iteration(ind + INDEX_OFFSET);
//    ind_rear = (int) newton_iteration(ind - INDEX_OFFSET);
//    ind = dichotomy(ind_rear, ind_ahead);
//    CU_ASSERT_EQUAL(ind_ahead, 1454)
//    CU_ASSERT_EQUAL(ind_rear, 1104)
//    CU_ASSERT_EQUAL(ind, 1257)
//
//    ind = 1063;
//    distance_north = 21.3f;
//    distance_east = 4.9f;
//    ind_ahead = (int) newton_iteration(ind + INDEX_OFFSET);
//    ind_rear = (int) newton_iteration(ind - INDEX_OFFSET);
//    ind = dichotomy(ind_rear, ind_ahead);
//    CU_ASSERT_EQUAL(ind_ahead, 1212)
//    CU_ASSERT_EQUAL(ind_rear, 887)
//    CU_ASSERT_EQUAL(ind, 1028)
//
//    ind = 685;
//    distance_north = 13.7f;
//    distance_east = 8.6f;
//    ind_ahead = (int) newton_iteration(ind + INDEX_OFFSET);
//    ind_rear = (int) newton_iteration(ind - INDEX_OFFSET);
//    ind = dichotomy(ind_rear, ind_ahead);
//    CU_ASSERT_EQUAL(ind_ahead, 843)
//    CU_ASSERT_EQUAL(ind_rear, 509)
//    CU_ASSERT_EQUAL(ind, 655)
//
//    ind = 1522;
//    distance_north = 30.45f;
//    distance_east = 6.48f;
//    ind_ahead = (int) newton_iteration((ind + INDEX_OFFSET > 1999) ? 1999 : (ind + INDEX_OFFSET));
//    ind_rear = (int) newton_iteration((ind - INDEX_OFFSET < 0) ? 0 : (ind - INDEX_OFFSET));
//    ind = dichotomy(ind_rear, ind_ahead);
//    CU_ASSERT_EQUAL(ind_ahead, 1641)
//    CU_ASSERT_EQUAL(ind_rear, 1344)
//    CU_ASSERT_EQUAL(ind, 1547) //30.9


//    {23.164415f, 140.739357f},
//    25.032100f, 140.478674f
    ind = 137;
    distance_north = 23.463922f;
    distance_east = 140.948782f; // 注：修改了窗口 和 计算距离的函数内存放位置点的数据
    ind_ahead = (int) newton_iteration((ind + INDEX_OFFSET > 412) ? 412 : (ind + INDEX_OFFSET));
    ind_rear = (int) newton_iteration((ind - INDEX_OFFSET < 0) ? 0 : (ind - INDEX_OFFSET));
    ind = dichotomy(ind, ind_ahead);
    CU_ASSERT_EQUAL(ind, 139)

}
