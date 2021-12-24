//
// Created by 16625 on 2021-12-23.
//

#include "gps_text.h"
#include "CUnit/Basic.h"
#include "gps_praser.h"

void test_nmea_comma_position() {
    char data[] = "$GPGGA,235316.000,3959.9925,S,12000.0090,E,1,06,1.21,62.77,M,0.00,M,,*7B";
    char *p = data;
    int n1 = nmea_comma_position(data, 2);
    CU_ASSERT_EQUAL(*(p + n1), '3');
    CU_ASSERT_NOT_EQUAL(*(p + n1), 'J');
    CU_ASSERT_EQUAL(n1, 18);
    CU_ASSERT_NOT_EQUAL(n1, 17);
    int n2 = nmea_comma_position(data, 3);
    CU_ASSERT_EQUAL(*(p + n2), 'S');
    CU_ASSERT_NOT_EQUAL(*(p + n2), '0');
    CU_ASSERT_EQUAL(n2, 28);
    CU_ASSERT_EQUAL(n2 - n1, 10);
    CU_ASSERT_NOT_EQUAL(n2, 29);
    int n3 = nmea_comma_position(data, 20);
    CU_ASSERT_NOT_EQUAL(*(p + n3), '0');
    CU_ASSERT_EQUAL(n3, 0XFF); //FAULT
    CU_ASSERT_NOT_EQUAL(n3, sizeof(data));
}

void text_nmea_pow() {

}

void test_nmea_str2num() {

}

void text_nmea_gpgga_analysis() {

}
