//
// Created by 16625 on 2021-12-20.
//

#include "mdtp_test.h"
#include "encrypt.h"
#include "decrypt.h"
#include "CUnit/Basic.h"

void uart1_sendbyte(unsigned char data) {
    static unsigned char rcv_counter = 0;
    switch (rcv_counter) {
        case 0:
        case 11: CU_ASSERT_EQUAL(data, 0xff)
            break;
        case 1: CU_ASSERT_EQUAL(data, 0x1e)
            break;
        case 2:
        case 3:
        case 4:
        case 6:
        case 9: CU_ASSERT_EQUAL(data, 0x00)
            break;
        case 5: CU_ASSERT_EQUAL(data, 0x01)
            break;
        case 7: CU_ASSERT_EQUAL(data, 0x51)
            break;
        case 8: CU_ASSERT_EQUAL(data, 0x88)
            break;
        case 10: CU_ASSERT_EQUAL(data, 0x11)
            break;
        default: CU_FAIL("Unexpected Value Appear")
            break;
    }
    rcv_counter++;
}

void mdtp_callback_handler(unsigned char pid, const unsigned char *data) {
    CU_ASSERT_EQUAL(pid, 0x02)

    CU_ASSERT_EQUAL(data[0], 0xff)
    CU_ASSERT_EQUAL(data[1], 0x00)
    CU_ASSERT_EQUAL(data[2], 0x00)
    CU_ASSERT_EQUAL(data[3], 0x01)
    CU_ASSERT_EQUAL(data[4], 0xff)
    CU_ASSERT_EQUAL(data[5], 0x51)
    CU_ASSERT_EQUAL(data[6], 0x88)
    CU_ASSERT_EQUAL(data[7], 0x00)
}

void test_mdtp_encrypt(void) {
    unsigned char data[8] = {0xff, 0x00, 0x00, 0x01, 0xff, 0x51, 0x88, 0x00};
    mdtp_data_transmit(0x01, data);
}

void test_mdtp_decrypt(void) {
    unsigned char data[14] = {0xff, 0xff, 0xff, 0x2d, 0x00, 0x00, 0x00, 0x01,
                              0x00, 0x51, 0x88, 0x00, 0x11, 0xff};
    for (int counter = 0; counter < 14; ++counter)
        mdtp_receive_handler(data[counter]);
}
