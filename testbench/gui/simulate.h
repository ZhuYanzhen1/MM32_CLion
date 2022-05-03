//
// Created by 16625 on 2022-05-03.
//

#ifndef TESTBENCH_SIMULATE_H
#define TESTBENCH_SIMULATE_H

void
lqr_update(float refDelta, float Delta_delta, float v_delta, float *x, float *y, float *yaw, float *v, float *Delta);

#endif //TESTBENCH_SIMULATE_H
