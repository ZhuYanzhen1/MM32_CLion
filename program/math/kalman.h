//
// Created by 16625 on 2022-02-25.
//

#ifndef MAIN_C_MATH_KALMAN_H_
#define MAIN_C_MATH_KALMAN_H_

void kalman_init(void);
float get_kalman_angle(float new_angle, float new_gyro, float dt);

#endif //MAIN_C_MATH_KALMAN_H_
