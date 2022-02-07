#include "riccati_solver.h"
#include "stdio.h"

//  P = 1.5011    1.0602   -0.0839
//      1.0602    9.3531    0.2382
//      -0.0839    0.2382    0.0994

void solveRiccatiIteration(double A[3][3], double B[3][2], double Q[3][3], double R[2][2]) {
    printf("\r\n------------Matrix A------------\r\n");
    for (int counter1 = 0; counter1 < 3; ++counter1) {
        for (int counter2 = 0; counter2 < 3; ++counter2)
            printf("%f   ", A[counter1][counter2]);
        printf("\r\n");
    }

    printf("\r\n------------Matrix B------------\r\n");
    for (int counter1 = 0; counter1 < 3; ++counter1) {
        for (int counter2 = 0; counter2 < 2; ++counter2)
            printf("%f   ", B[counter1][counter2]);
        printf("\r\n");
    }

    printf("\r\n------------Matrix Q------------\r\n");
    for (int counter1 = 0; counter1 < 3; ++counter1) {
        for (int counter2 = 0; counter2 < 3; ++counter2)
            printf("%f   ", Q[counter1][counter2]);
        printf("\r\n");
    }

    printf("\r\n------------Matrix R------------\r\n");
    for (int counter1 = 0; counter1 < 2; ++counter1) {
        for (int counter2 = 0; counter2 < 2; ++counter2)
            printf("%f   ", R[counter1][counter2]);
        printf("\r\n");
    }
}
