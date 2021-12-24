//
// Created by 16625 on 2021-12-23.
//

#include "gps_text.h"
#include "CUnit/Basic.h"
#include "gps_praser.h"

void test_nmea_comma_position() {
    char data[] = "$GPGGA,235316.000,3959.9925,S,12000.0090,E,1,06,1.21,62.77,M,0.00,M,,*7B";
    char *p = data;
    char n1 = nmea_comma_position(data, 2);
    CU_ASSERT_EQUAL(*(p + n1), '3'); //TURE
    CU_ASSERT_EQUAL(*(p + n1), 'J'); //FAULT
    CU_ASSERT_EQUAL(n1, 7); //TURE
    CU_ASSERT_EQUAL(n1, 6); //FAULT
    char n2 = nmea_comma_position(data, 5);
    CU_ASSERT_EQUAL(*(p + n2), 'E'); //TURE
    CU_ASSERT_EQUAL(*(p + n2), '0'); //FAULT
    CU_ASSERT_EQUAL(n2, 18); //TURE
    CU_ASSERT_EQUAL(n2, 19); //FAULT
    char n3 = nmea_comma_position(data, 20);
    CU_ASSERT_EQUAL(*(p + n3), '0'); //FAULT
    CU_ASSERT_EQUAL(n3, 0XFE); //TURE
    CU_ASSERT_EQUAL(n3, sizeof(data)); //TURE
}

void text_nmea_pow() {

}

void test_nmea_str2num() {

}

void text_nmea_gpgga_analysis() {

}
