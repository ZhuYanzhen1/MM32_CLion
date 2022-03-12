//
// Created by 16625 on 2022-03-09.
//

#include "sensor_decode_test.h"
#include "CUnit/Basic.h"
#include "gps_parser.h"
#include "sensor_decode.h"
#include "stdio.h"

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

extern decode_fixed small_packets;
extern decode_debug debug_data;

void test_precossing_send_data() {
    short buffer[5] = {20, 352, -9, -132, 1543};
    unsigned char packets[12] = {0};
    precossing_fixed_length_data(packets, buffer);
    CU_ASSERT_EQUAL(packets[0], 0xff);
    CU_ASSERT_EQUAL(packets[1], 0x14);
    CU_ASSERT_EQUAL(packets[2], 0x60);
    CU_ASSERT_EQUAL(packets[3], 0xf7);
    CU_ASSERT_EQUAL(packets[4], 0x7c);
    CU_ASSERT_EQUAL(packets[5], 0x07);
    CU_ASSERT_EQUAL(packets[6], 0x01);
    CU_ASSERT_EQUAL(packets[7], 0x77);
    CU_ASSERT_EQUAL(packets[8], 0xe0);
    CU_ASSERT_EQUAL(packets[9], 0x00);
//    CU_ASSERT_EQUAL(packets[10], 0x1c);

    unpacking_fixed_length_data(packets + 1);

    CU_ASSERT_EQUAL(small_packets.pitch, 20);
    CU_ASSERT_EQUAL(small_packets.yaw, 352);
    CU_ASSERT_EQUAL(small_packets.mx, -9);
    CU_ASSERT_EQUAL(small_packets.my, -132);
    CU_ASSERT_EQUAL(small_packets.mz, 1543);

}

void test_precossing_variable_length_data() {

    int mx = 0x7D4685, my = 0x852114, mz = 0x3305c4;
    int offset_x = (int) (63.2467f * 10000), offset_y = (int) (-3.3462f), offset_z = (int) (202.16880f * 100000);
    int bias_x = 0, bias_y = 0, bias_z = 0;
    int offbias_x = 0, offbias_y = 0, offbias_z = 0;
    int residual = 0, step_length = 0;
    int num = 0;
    int buffer[] = {mx, my, mz, offset_x, offset_y, offset_z, bias_x, bias_y, bias_z,
                    offbias_x, offbias_y, offbias_z, residual, step_length, num
    };
    unsigned char packets[59] = {0};
    precossing_variable_length_data(packets, 59, buffer);

    CU_ASSERT_EQUAL(packets[0], 0xa5);
    CU_ASSERT_EQUAL(packets[1], 0xa5);
    CU_ASSERT_EQUAL(packets[2], 0x3B);
    CU_ASSERT_EQUAL(packets[3], 0x7D);
    CU_ASSERT_EQUAL(packets[4], 0x46);
    CU_ASSERT_EQUAL(packets[5], 0x85);
    CU_ASSERT_EQUAL(packets[6], 0x85);
    CU_ASSERT_EQUAL(packets[7], 0x21);
    CU_ASSERT_EQUAL(packets[8], 0x14);
    CU_ASSERT_EQUAL(packets[9], 0x33);
    CU_ASSERT_EQUAL(packets[10], 0x05);
    CU_ASSERT_EQUAL(packets[11], 0xC4);
    CU_ASSERT_EQUAL(packets[12], 0x00);
    CU_ASSERT_EQUAL(packets[13], 0x09);
    CU_ASSERT_EQUAL(packets[14], 0xa6);
    CU_ASSERT_EQUAL(packets[15], 0x93);
    CU_ASSERT_EQUAL(packets[20], 0x01);
    CU_ASSERT_EQUAL(packets[21], 0x34);
    CU_ASSERT_EQUAL(packets[22], 0x7c);
    CU_ASSERT_EQUAL(packets[23], 0x30);

    unpacking_variable_length_data(packets + 3);

    CU_ASSERT_EQUAL(debug_data.mag_x, 0x7D4685);
    CU_ASSERT_EQUAL(debug_data.mag_y, 0x852114);
    CU_ASSERT_EQUAL(debug_data.mag_z, 0x3305c4);
    CU_ASSERT_EQUAL(debug_data.offset_x, 632467);
    CU_ASSERT_EQUAL(debug_data.offset_y, -3);
    CU_ASSERT_EQUAL(debug_data.offset_z, 20216880);
}
