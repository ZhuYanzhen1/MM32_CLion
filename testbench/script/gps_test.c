//
// Created by 16625 on 2021-12-23.
//

#include "gps_test.h"
#include "CUnit/Basic.h"
#include "gps_parser.h"


void test_nmea_gnrmc_analysis() {
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

#define GEO_ANGLE(x)    ((x) * PI / 180.0)
#define PI              3.1415926f
#define EARTH_RADIUS    6371000 //m:6371000  cm:63710

float get_distance_m_lat(float lat) {
    float distance = GEO_ANGLE(lat);
    return EARTH_RADIUS * distance;
}

float get_distance_m_lon(float lon) {
    float distance = GEO_ANGLE(lon);
    return EARTH_RADIUS * distance;
}

void lat_and_lon_rotation_shift(unsigned int latitude, unsigned int longitude) {
    float temp_latitude = unit_to_degree(latitude, 4);
    float temp_lonitude = unit_to_degree(longitude, 4);
    float north_distance = get_distance_m_lat(temp_latitude - 20);
    float east_distance = get_distance_m_lat(temp_lonitude - 100);

    (void) north_distance;
    (void) east_distance;
}

void test_lat_and_lon_rotation_shift() {


//    lat_and_lon_rotation_shift(23027480, 120000090);//338677.406 2223915.25
    lat_and_lon_rotation_shift(23027481, 120000091);//338677.656 2223915.25
    lat_and_lon_rotation_shift(23027490, 120000100);//338679.312 2223917
}
