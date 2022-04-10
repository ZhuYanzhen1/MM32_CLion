//
// Created by 16625 on 2022-03-09.
//

#include "sensor_decode_test.h"
#include "CUnit/Basic.h"
#include "gps_parser.h"
#include "sensor_decode.h"
#include "fast_math.h"
#include "sdtp_pack.h"
#include "sdtp_unpack.h"

void test_nmea_gnrmc_analysis() {
    float i = 0.732f;
    float j = my_acos(i);
    (void) j;

    char data[] = "GNRMC,235316.000,A,2959.9925,S,12000.0090,E,0.009,75.020,020711,,,A,*77";
//    char data1[] = "$GNRMC,,V,,,,,,,,,,M,V*34";
//    nmea_rmc gps_rmc = {0};
    nmea_gnrmc_analysis(data);

    CU_ASSERT_EQUAL(gps_rmc.status, 'A')
    CU_ASSERT_EQUAL(gps_rmc.latitude, 29599925)
    CU_ASSERT_EQUAL(gps_rmc.latitude_direction, 'S')
    CU_ASSERT_EQUAL(gps_rmc.longitude, 120000090)
    CU_ASSERT_EQUAL(gps_rmc.longitude_direction, 'E')
    CU_ASSERT_EQUAL(gps_rmc.speed_to_ground_section, 9)
    CU_ASSERT_EQUAL(gps_rmc.direction_of_ground_truth, 75020)
    CU_ASSERT_EQUAL(gps_rmc.date, 20711)
    CU_ASSERT_EQUAL(gps_rmc.mode, 'A')
    CU_ASSERT_EQUAL(gps_rmc.checksum, 0X77)
//    nmea_gnrmc_analysis(&gps_rmc, data1);

}

extern decode_proc proc_distance;

void test_precossing_proc_to_control() {
    float n_dis = 333.453f;
    float e_dis = 444.342f;
    float num = 0xFFFF;
    unsigned int buffer[] = {*((unsigned int *) (&n_dis)), *((unsigned int *) (&e_dis))};
    unsigned int packets[PROC_MCU_SEND_AMOUNT] = {0};

    packets[1] = ((buffer[0] & 0xff000000) >> 24);
    packets[(1) + 1] = ((buffer[0] & 0x00ff0000) >> 16);
    packets[(1) + 2] = ((buffer[0] & 0x0000ff00) >> 8);
    packets[(1) + 3] = (buffer[0] & 0x000000ff);

    unsigned int temp;
    (temp) = (unsigned int) (((int) packets[1] << 24) |
                             ((int) packets[1 + 1] << 16) |
                             ((int) packets[1 + 2] << 8) |
                             ((int) packets[1 + 3]));

    float a = *((float *) (&temp));
    (void) a;
//    float a = 234.543f;
//    unsigned int tmp_float_int = *((unsigned int *) (&a));
//    a = *((float *) (&tmp_float_int));
//    (void) a;

    precossing_proc_to_control((unsigned int *) packets, (const unsigned int *) buffer);
    unpacking_proc_to_control(&packets[1]);
}
