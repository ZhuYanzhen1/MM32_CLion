//
// Created by 16625 on 2021-12-20.
//

#include "mdtp_test.h"
#include "encrypt.h"
#include "CUnit/Basic.h"

static unsigned char rcv_counter = 0;

void uart1_sendbyte(unsigned char data) {
    rcv_counter++;
    CU_ASSERT_EQUAL(data, 0xff);
}

void test_mdtp_encrypt(void) {
    unsigned char data[8] = {0xff, 0x00, 0x00, 0x01, 0xff, 0x51, 0x88, 0x00};
    mdtp_data_transmit(0x01, data);
}
