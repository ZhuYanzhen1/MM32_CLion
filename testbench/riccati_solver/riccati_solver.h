#ifndef RICCATI_SOLVER_H_
#define RICCATI_SOLVER_H_

#include "ulapack_type.h"

/* Itereation method for continuous model */
void solveRiccatiIterationC(Matrix_t *A, Matrix_t *B,
                            Matrix_t *Q, Matrix_t *R,
                            Matrix_t *P);

#endif
