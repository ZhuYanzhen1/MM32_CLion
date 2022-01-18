//
// Created by 16625 on 2021-12-23.
//

#include "gps_test.h"
#include "CUnit/Basic.h"
#include "gps_parser.h"

void test_nmea_pow() {
    CU_ASSERT_EQUAL(nmea_pow(10, 2), 100)
    CU_ASSERT_NOT_EQUAL(nmea_pow(10, 8), 100)
    CU_ASSERT_EQUAL(nmea_pow(10, 0), 1)
    CU_ASSERT_NOT_EQUAL(nmea_pow(10, 0), 10)
    CU_ASSERT_NOT_EQUAL(nmea_pow(10, 8), 1)
}

void test_nmea_str2num() {
    char *data[] = {"3424,", "-234,", "34.32,", "-23.43,", "0.661,"}, d = 1;
    CU_ASSERT_EQUAL(nmea_str2num(data[0], &d), 3424)
    CU_ASSERT_EQUAL(nmea_str2num(data[1], &d), -234)
//    CU_ASSERT_EQUAL(nmea_str2num(p[2]), 34.32);   //TRUE
//    CU_ASSERT_EQUAL(nmea_str2num(p[3]), -23.43);  //TRUE
//    CU_ASSERT_EQUAL(nmea_str2num(p[4]), 0.661);   //TRUE
    CU_ASSERT_NOT_EQUAL(nmea_str2num(data[0], &d), 13424)
    CU_ASSERT_NOT_EQUAL(nmea_str2num(data[1], &d), -13424)
    CU_ASSERT_NOT_EQUAL(nmea_str2num(data[2], &d), 13.424)
    CU_ASSERT_NOT_EQUAL(nmea_str2num(data[3], &d), -13.424)
    CU_ASSERT_NOT_EQUAL(nmea_str2num(data[4], &d), 0.134)

}

void test_nmea_gnrmc_analysis() {
    char data[] = "GNRMC,235316.000,A,2959.9925,S,12000.0090,E,0.009,75.020,020711,,,A,*77";
//    char data1[] = "$GNRMC,,V,,,,,,,,,,M,V*34";
    nmea_rmc gps_rmc = {0};
    nmea_gnrmc_analysis(&gps_rmc, data);

    CU_ASSERT_EQUAL(gps_rmc.status, 'A')
    CU_ASSERT_EQUAL(gps_rmc.latitude, 295959)
    CU_ASSERT_EQUAL(gps_rmc.latitude_direction, 'S')
    CU_ASSERT_EQUAL(gps_rmc.longitude, 1200000)
    CU_ASSERT_EQUAL(gps_rmc.longitude_direction, 'E')
    CU_ASSERT_EQUAL(gps_rmc.speed_to_ground_section, 9)
    CU_ASSERT_EQUAL(gps_rmc.direction_of_ground_truth, 75020)
    CU_ASSERT_EQUAL(gps_rmc.date, 20711)
    CU_ASSERT_EQUAL(gps_rmc.mode, 'A')
    CU_ASSERT_EQUAL(gps_rmc.checksum, 0X77)
//    nmea_gnrmc_analysis(&gps_rmc, data1);
}


