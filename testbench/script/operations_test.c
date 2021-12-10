//
// Created by Lao·Zhu on 2021/12/9.
//

#include "operations_test.h"
#include "CUnit/Basic.h"

unsigned char operation_add_uint8(unsigned char a, unsigned char b) {
    unsigned char c = (a + b) % 0xff;
    return c;
}

unsigned char operation_minus_uint8(unsigned char a, unsigned char b) {
    int c = (int) ((int) a - (int) b);
    c = c < 0 ? 0 : c;
    return (unsigned char) c;
}

unsigned char operation_multiply_uint8(unsigned char a, unsigned char b) {
    unsigned char c = ((int) ((int) a * (int) b)) % 0xff;
    return c;
}

unsigned char operation_divide_uint8(unsigned char a, unsigned char b) {
    unsigned char c = (b != 0 ? (a / b) : 0xff);
    return c;
}

void test_operation_add(void) {
    for (unsigned char counter = 0; counter < 0xff; ++counter) {
        CU_ASSERT_EQUAL(operation_add_uint8(counter, 1), (unsigned short) (counter + 1))
    }
}

void test_operation_minus(void) {
    for (unsigned char counter = 0; counter < 0xff; ++counter) {
        CU_ASSERT_EQUAL(operation_minus_uint8(counter, 1), (short) (counter - 1))
    }
}

void test_operation_multiply(void) {
    CU_ASSERT_EQUAL(operation_multiply_uint8(20, 5), 100)
    CU_ASSERT_EQUAL(operation_multiply_uint8(200, 1), 200)
    CU_ASSERT_EQUAL(operation_multiply_uint8(10, 200), 215)
    CU_ASSERT_EQUAL(operation_multiply_uint8(5, 5), 25)
}

void test_operation_divide(void) {
    CU_ASSERT_EQUAL(operation_divide_uint8(20, 5), 4)
    CU_ASSERT_EQUAL(operation_divide_uint8(200, 1), 200)
    CU_ASSERT_EQUAL(operation_divide_uint8(10, 0), 255)
    CU_ASSERT_EQUAL(operation_divide_uint8(5, 3), 1)
}
