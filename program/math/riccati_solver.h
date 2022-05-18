#ifndef RICCATI_SOLVER_H_
#define RICCATI_SOLVER_H_

#define INDEX_OFFSET    20

typedef struct {
    float pos_n;
    float pos_e;
    float angle;
} basic_status_t;

#if RUNNING_UNIT_TEST
float calculate_distance(int ind, float x, float y);
int dichotomy(int ind_start, int ind_end, float x, float y);
#else
float calculate_distance(int ind);
int dichotomy(int ind_start, int ind_end);
#endif  // RUNNING_UNIT_TEST

void project(basic_status_t current, basic_status_t *project, float v, float t, float servo_angle);

void lqr_control_test(unsigned short index, int angle_ctrl, float angle_yaw, float distance_n, float distance_e);
void lqr_control(unsigned short index);

void solve_feedback_value(float p[3][3], float a[3][3], float b[3][2], float x[3][1], float r, float fb[2][1]);
void solve_riccati_equation(float a[3][3], float b[3][2], float q, float r, float p[3][3]);

#endif
