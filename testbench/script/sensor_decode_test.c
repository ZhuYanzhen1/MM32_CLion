//
// Created by 16625 on 2022-03-09.
//

#include "sensor_decode_test.h"
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

/*!
    \brief      Processing data to be sent through the serial port
    \param[out] packets: Processes packets as long as they are sent through the serial port
    \param[in]  buffer: Data to be sent,includes 5 short type variables and a check digit
*/
void precossing_send_data(unsigned char packets[12], short *buffer) {
    // 包头
    packets[0] = 0xff;

    // 对加速度的值进行处理，防止负值造成影响，然后在接收的时候再加2048
    buffer[2] += 2048;
    buffer[3] += 2048;
    buffer[4] += 2048;


    // 处理5个数据的低8位
    packets[1] = (unsigned char) (buffer[0] & 0x00ff);
    packets[2] = (unsigned char) (buffer[1] & 0x00ff);
    packets[3] = (unsigned char) (buffer[2] & 0x00ff);
    packets[4] = (unsigned char) (buffer[3] & 0x00ff);
    packets[5] = (unsigned char) (buffer[4] & 0x00ff);

    // 处理5个数据的高4位
    packets[6] = (unsigned char) (((buffer[0] & 0x0f00) >> 4) | ((buffer[1] & 0x0F00) >> 8));
    packets[7] = (unsigned char) (((buffer[2] & 0x0f00) >> 4) | ((buffer[3] & 0x0F00) >> 8));
    packets[8] = (unsigned char) ((buffer[4] & 0x0f00) >> 4);

    // 调整位，从高到低，每一位与一个字节的数据对应，如果为1，那就代表相应的数据为0xff
    for (unsigned char i = 1; i < 9; ++i) {
        if (packets[i] == 0xff) {
            packets[i] = 0;
            packets[9] = 0x80 >> (i - 1);
        }
    }
    packets[10] = buffer[5];

    // 包尾
    packets[11] = 0xff;
}

run_pack small_packets;

/*!
    \brief      Unwrap the small package
    \param[in]  packets: Packets to be solved
*/
void unpacking_send_data(const unsigned char packets[10]) {
    small_packets.pitch = (short) (packets[0] | (packets[5] & 0xf0 << 4));
    small_packets.yaw = (short) (packets[1] | (packets[5] & 0xf0 << 8));

    //先解出来并拿到东西，再去把传感器3轴信息-2048
}

void test_precossing_send_data() {
    short buffer[6] = {20, 352, -9, -132, 1543, 28};
    unsigned char packets[12] = {0};
    precossing_send_data(packets, buffer);
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
    CU_ASSERT_EQUAL(packets[10], 0x1c);
    CU_ASSERT_EQUAL(packets[11], 0xff);
}
