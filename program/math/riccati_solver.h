#ifndef RICCATI_SOLVER_H_
#define RICCATI_SOLVER_H_

#define INDEX_NUMBER        2000
#define MAX_ITERATION       5
#define MAX_ERROR           0.01f
#define INDEX_OFFSET        500
#define DISTANCE_OFFSET     3.6f

int dichotomy(int ind_start, int ind_end);
int newton_iteration(int ind_last);
float calculate_distance(int ind);
void solve_feedback_value(float p[3][3], float a[3][3], float b[3][2], float x[3][1], float r, float fb[2][1]);
void solve_riccati_equation(float a[3][3], float b[3][2], float q, float r, float p[3][3]);

#endif
