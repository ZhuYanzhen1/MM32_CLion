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
    short pitch;
    short yaw;
    short mx;
    short my;
    short mz;

} run_pack;

typedef struct {

} run_debug;

void precossing_send_data(unsigned char packets[12], short *buffer);

void test_precossing_send_data();

void test_nmea_pow();

void test_nmea_str2num();

void test_nmea_gnrmc_analysis();

void test_lat_and_lon_rotation_shift();

#endif //TESTBENCH_SENSOR_DECODE_TEST_H
