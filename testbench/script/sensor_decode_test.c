//
// Created by 16625 on 2022-03-09.
//

#include "sensor_decode_test.h"
#include "CUnit/Basic.h"

/*!
    \brief      Processing data to be sent through the serial port
    \param[out] send_data: Processes packets as long as they are sent through the serial port
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
