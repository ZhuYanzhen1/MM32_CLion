//
// Created by 16625 on 2021-12-20.
//

#include "mdtp_test.h"
#include "mdtp_unpack.h"
#include "mdtp_pack.h"
#include "CUnit/Basic.h"

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
    unsigned int uart_buffer[12];
    unsigned char data[8] = {0xff, 0x00, 0x00, 0x01, 0xff, 0x51, 0x88, 0x00};
    mdtp_data_transmit(0x01, data, uart_buffer);
    CU_ASSERT_EQUAL(uart_buffer[0], 0xff)
    CU_ASSERT_EQUAL(uart_buffer[11], 0xff)
    CU_ASSERT_EQUAL(uart_buffer[1], 0x1e)
    CU_ASSERT_EQUAL(uart_buffer[2], 0x00)
    CU_ASSERT_EQUAL(uart_buffer[3], 0x00)
    CU_ASSERT_EQUAL(uart_buffer[4], 0x00)
    CU_ASSERT_EQUAL(uart_buffer[6], 0x00)
    CU_ASSERT_EQUAL(uart_buffer[9], 0x00)
    CU_ASSERT_EQUAL(uart_buffer[5], 0x01)
    CU_ASSERT_EQUAL(uart_buffer[7], 0x51)
    CU_ASSERT_EQUAL(uart_buffer[8], 0x88)
    CU_ASSERT_EQUAL(uart_buffer[10], 0x11)
}

void test_mdtp_decrypt(void) {
    unsigned char data[14] = {0xff, 0xff, 0xff, 0x2d, 0x00, 0x00, 0x00, 0x01,
                              0x00, 0x51, 0x88, 0x00, 0x11, 0xff};
    for (int counter = 0; counter < 14; ++counter)
        mdtp_receive_handler(data[counter]);
}
