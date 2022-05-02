#include "solver_test.h"
#include "CUnit/Basic.h"

#include "math.h"
#include "riccati_solver.h"
#include "mag_calibrate.h"
#include "fast_math.h"
#include "stdio.h"
#include "string.h"
#include "test_data.h"

#define TOLERANCE_PRECISION     0.2f
#define YAW_TO_ANGLE            (-60.3111346f)     // 180/pi * (-50/47.5)

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

float distance_north;
float distance_east;

float calculate_distance(int ind) {
    float distance = sqrtf(
        ((test_point_unit[ind][0] - distance_north) * (test_point_unit[ind][0] - distance_north)
            + (test_point_unit[ind][1] - distance_east) * (test_point_unit[ind][1] - distance_east)));
    return distance;
}

void lqr_control_test(unsigned short index, int angle_ctrl, float angle_yaw, float distance_n, float distance_e) {

    float v_r = 2, dt = 0.02f, L = 0.3f;

    // 求位置、航向角的误差
    float yaw_temp = (angle_yaw < 180) ? angle_yaw : (180 - angle_yaw);
    yaw_temp *= (0.0174533f);
    float x_error = distance_n - test_point_unit[index][0];
    float y_error = distance_e - test_point_unit[index][1];
    float yaw_error = yaw_temp - test_point_unit[index][2];

    // 计算横向误差
    float lateral_error = y_error * (test_point_unit[index][2]) - x_error * my_asin(test_point_unit[index][2]);

    // 由状态方程矩阵系数，计算K
    float a[3][3] = {{1, 0, -v_r * dt * sinf(test_point_unit[index][2])},
                     {0, 1, v_r * dt * cosf(test_point_unit[index][2])},
                     {0, 0, 1}};
    float b[3][2] = {{cosf(test_point_unit[index][2]) * dt, 0},
                     {sinf(test_point_unit[index][2]) * dt, 0},
                     {tanf(yaw_error) * dt / L, v_r * dt /
                         (L * cosf(yaw_error) * cosf(yaw_error))}};

    // 获得速度误差量、前轮转角误差量两个控制量
    float x[3][1] = {{x_error},
                     {y_error},
                     {yaw_error}};
    float p[3][3] = {0};
    float control_val[2][1] = {0};
    float r = 1;
    float q = 1;

    solve_riccati_equation(a, b, q, r, p);
    solve_feedback_value(p, a, b, x, r, control_val);
//    speed = speed +control_val[0][0];
    short delta_angle = (short) (control_val[1][0] * YAW_TO_ANGLE);
    angle_ctrl = angle_ctrl + delta_angle;
    if (angle_ctrl > 200)
        angle_ctrl = 200;
    else if (angle_ctrl < 100)
        angle_ctrl = 100;
}

void test_riccati_solver(void) {
    float fai_r = 0.23f, delta_r = 0.086f, L = 0.4f, v_r = 5.1f, dt = 0.01f;
    float error_x = 1.2f, error_y = 0.4f, error_fai = 0.17f;
    float a[3][3] = {{1, 0, -v_r * dt * sinf(fai_r)},
                     {0, 1, v_r * dt * cosf(fai_r)},
                     {0, 0, 1}};

    float b[3][2] = {{cosf(fai_r) * dt, 0},
                     {sinf(fai_r) * dt, 0},
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

void test_lqr_contrl(void) {
    short angle_ctrl = 150;
    unsigned short index;
    float distance_n, distance_e, angle_yaw;

    index = 16, distance_n = 337764.781f, distance_e = 347232.688f, angle_yaw = 242.7f;
    // 337766.6207556f, 347232.7255131f
    lqr_control_test(index, angle_ctrl, angle_yaw, distance_n, distance_e);

    index = 15, distance_n = 337765.781f, distance_e = 347231.938f, angle_yaw = 147.5f;
    // 337766.6029507f, 347232.4832368f
    lqr_control_test(index, angle_ctrl, angle_yaw, distance_n, distance_e);
}
