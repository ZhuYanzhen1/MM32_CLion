//
// Created by 16625 on 2022-03-09.
//

#ifndef TESTBENCH_SENSOR_DECODE_TEST_H
#define TESTBENCH_SENSOR_DECODE_TEST_H

/* MSB
 * high:4bit
 * low:8bit
 * xy：x占高四位；y占低四位
 * */
typedef struct {
    char pitch_low;
    char yaw_low;
    char mag_x_low;
    char mag_y_low;
    char mag_z_low;
    char angle_py_high;
    char mag_xy_high;
    char mag_z_high;
} run_pack;

typedef struct {

} run_debug;

void precossing_send_data(unsigned char packets[12], short *buffer);

void test_precossing_send_data();

#endif //TESTBENCH_SENSOR_DECODE_TEST_H
