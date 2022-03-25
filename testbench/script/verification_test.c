//
// Created by LaoZhu on 2022/3/9.
//

#include "verification_test.h"
#include "verification.h"
#include "CUnit/Basic.h"

void test_verification_checksum(void) {
    const char veriy_string1[21] = "this is a test string";
    const char veriy_string2[24] = "this is next test string";
    const char veriy_string3[16] = "this is a string";
    const char veriy_string4[13] = "a test string";
    unsigned char verify_checksum1 = verification_checksum_u8((unsigned char *) veriy_string1, 21);
    unsigned char verify_checksum2 = verification_checksum_u8((unsigned char *) veriy_string2, 24);
    unsigned char verify_checksum3 = verification_checksum_u8((unsigned char *) veriy_string3, 16);
    unsigned char verify_checksum4 = verification_checksum_u8((unsigned char *) veriy_string4, 13);
    CU_ASSERT_EQUAL(verify_checksum1, 0xCC)
    CU_ASSERT_EQUAL(verify_checksum2, 0x2A)
    CU_ASSERT_EQUAL(verify_checksum3, 0xEC)
    CU_ASSERT_EQUAL(verify_checksum4, 0xF8)
}

void test_verification_checkxor(void) {
    const char veriy_string1[21] = "this is a test string";
    const char veriy_string2[24] = "this is next test string";
    const char veriy_string3[16] = "this is a string";
    const char veriy_string4[13] = "a test string";
    unsigned char verify_checkxor1 = verification_checkxor_u8((unsigned char *) veriy_string1, 21);
    unsigned char verify_checkxor2 = verification_checkxor_u8((unsigned char *) veriy_string2, 24);
    unsigned char verify_checkxor3 = verification_checkxor_u8((unsigned char *) veriy_string3, 16);
    unsigned char verify_checkxor4 = verification_checkxor_u8((unsigned char *) veriy_string4, 13);
    CU_ASSERT_EQUAL(verify_checkxor1, 0x7E)
    CU_ASSERT_EQUAL(verify_checkxor2, 0x18)
    CU_ASSERT_EQUAL(verify_checkxor3, 0x48)
    CU_ASSERT_EQUAL(verify_checkxor4, 0x62)
}

void test_verification_crc8(void) {
    const char veriy_string1[21] = "this is a test string";
    const char veriy_string2[24] = "this is next test string";
    const char veriy_string3[16] = "this is a string";
    const char veriy_string4[13] = "a test string";
    unsigned int verify_data[24] = {0};

    for (int i = 0; i < sizeof(veriy_string1); ++i)
        verify_data[i] = (unsigned int) veriy_string1[i];
    unsigned char verify_crc1 = verification_crc8(verify_data, 21);

    for (int i = 0; i < sizeof(veriy_string2); ++i)
        verify_data[i] = (unsigned int) veriy_string2[i];
    unsigned char verify_crc2 = verification_crc8(verify_data, 24);

    for (int i = 0; i < sizeof(veriy_string3); ++i)
        verify_data[i] = (unsigned int) veriy_string3[i];
    unsigned char verify_crc3 = verification_crc8(verify_data, 16);

    for (int i = 0; i < sizeof(veriy_string4); ++i)
        verify_data[i] = (unsigned int) veriy_string4[i];
    unsigned char verify_crc4 = verification_crc8(verify_data, 13);

    CU_ASSERT_EQUAL(verify_crc1, 0x6F)
    CU_ASSERT_EQUAL(verify_crc2, 0x24)
    CU_ASSERT_EQUAL(verify_crc3, 0xC9)
    CU_ASSERT_EQUAL(verify_crc4, 0x7F)
}

void test_verification_crc16(void) {
    const char veriy_string1[21] = "this is a test string";
    const char veriy_string2[24] = "this is next test string";
    const char veriy_string3[16] = "this is a string";
    const char veriy_string4[13] = "a test string";
    unsigned int verify_data[24] = {0};

    for (int i = 0; i < sizeof(veriy_string1); ++i)
        verify_data[i] = (unsigned int) veriy_string1[i];
    unsigned short verify_crc1 = verification_crc16(verify_data, 21);

    for (int i = 0; i < sizeof(veriy_string2); ++i)
        verify_data[i] = (unsigned int) veriy_string2[i];
    unsigned short verify_crc2 = verification_crc16(verify_data, 24);

    for (int i = 0; i < sizeof(veriy_string3); ++i)
        verify_data[i] = (unsigned int) veriy_string3[i];
    unsigned short verify_crc3 = verification_crc16(verify_data, 16);

    for (int i = 0; i < sizeof(veriy_string4); ++i)
        verify_data[i] = (unsigned int) veriy_string4[i];
    unsigned short verify_crc4 = verification_crc16(verify_data, 13);

    CU_ASSERT_EQUAL(verify_crc1, 0x32E6)
    CU_ASSERT_EQUAL(verify_crc2, 0xEA43)
    CU_ASSERT_EQUAL(verify_crc3, 0x64E3)
    CU_ASSERT_EQUAL(verify_crc4, 0xEBE6)
}
