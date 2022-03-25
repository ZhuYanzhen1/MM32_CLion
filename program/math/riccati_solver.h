#ifndef RICCATI_SOLVER_H_
#define RICCATI_SOLVER_H_

void solveRiccatiIteration(float A[3][3], float B[3][2], float Q, float R, float P[3][3]);

#endif
