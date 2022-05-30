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
    basic_status_t status = {4354.01f, //370
                             39633,
                             0}; // -2.792516059106141
    float yaw_temp = (status.angle < 180) ? status.angle : (status.angle - 360);
    yaw_temp *= ANGLE_TO_RADIAN;

    yaw_temp = -2.7925f;

    double x_error =
            status.pos_n - 4352.9990;    // 4352.99900000000	39632.9530000000	-2.78425578615249	0.00167711554858187
    double y_error = status.pos_e - 39632.953;
    double yaw_error = yaw_temp - (-2.784255);
    if (yaw_error > 3.14)
        yaw_error -= _2PI_;
    else if (yaw_error < -3.14)
        yaw_error += _2PI_;

    double v_r = 4, dt = 0.04f, fai_r = (-2.784255), delta_r = 0.00167711, L = 0.28;

    double a[3][3] = {{1, 0, -v_r * dt * sin(fai_r)},
                      {0, 1, v_r * dt * cos(fai_r)},
                      {0, 0, 1}};

    double b[3][2] = {{cos(fai_r) * dt,       0},
                      {sin(fai_r) * dt,       0},
                      {tan(delta_r) * dt / L, v_r * dt / (L * cos(delta_r) * cos(delta_r))}};
    double x[3][1] = {{x_error},
                      {y_error},
                      {yaw_error}};
    float p[3][3] = {0};
    float control_val[2][1] = {0};
    float q = 1;
    float r = 3;

    solve_riccati_equation(a, b, q, r, p);
    solve_feedback_value(p, a, b, x, r, control_val);

    volatile float angle = control_val[1][0] + (float) delta_r;

    CU_ASSERT(fabsf(control_val[0][0] + (float) v_r) > fabsf(5.4989f - TOLERANCE_PRECISION))
    CU_ASSERT(fabsf(control_val[0][0] + (float) v_r) < fabsf(5.4989f + TOLERANCE_PRECISION))
    CU_ASSERT(fabsf(control_val[1][0] + (float) delta_r) > fabsf(1.8205f - TOLERANCE_PRECISION))
    CU_ASSERT(fabsf(control_val[1][0] + (float) delta_r) < fabsf(1.8205f + TOLERANCE_PRECISION))
}

void test_project_solver(void) {

//    basic_status_t current_status = {4354.01f, 39633, -2.792516f}, project_status = {0};
//    float v = 4, t = 0.1f, servo_angle = 0.0016771f;
    basic_status_t current_status = {0, 0, 10 * 3.1315926f / 180}, project_status = {0};
    float v = 4, t = 0.1f, servo_angle = -20 * ANGLE_TO_RADIAN;
    project(current_status, &project_status, v, t, servo_angle);

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
