#ifndef RICCATI_SOLVER_H_
#define RICCATI_SOLVER_H_

void solve_feedback_value(float p[3][3], float a[3][3], float b[3][2], float x[3][1], float r, float fb[2][1]);
void solve_riccati_equation(float a[3][3], float b[3][2], float q, float r, float p[3][3]);

#endif
