//
// Created by Lao·Zhu on 2021/12/25.
//

#include "sort_test.h"
#include "sort.h"
#include "CUnit/Basic.h"

void test_heap_sort(void) {
    int test_buffer[] = {10, 48376, -1928, 0, 28, 138};
    sort_heapsort(test_buffer, (sizeof(test_buffer) / sizeof(test_buffer[0])));
    CU_ASSERT_EQUAL(test_buffer[0], -1928)
    CU_ASSERT_EQUAL(test_buffer[1], 0)
    CU_ASSERT_EQUAL(test_buffer[2], 10)
    CU_ASSERT_EQUAL(test_buffer[3], 28)
    CU_ASSERT_EQUAL(test_buffer[4], 138)
    CU_ASSERT_EQUAL(test_buffer[5], 48376)
}

void test_quick_sort(void) {
    int test_buffer[] = {10, 48376, -1928, 0, 28, 138};
    sort_quicksort(test_buffer, (sizeof(test_buffer) / sizeof(test_buffer[0])));
    CU_ASSERT_EQUAL(test_buffer[0], -1928)
    CU_ASSERT_EQUAL(test_buffer[1], 0)
    CU_ASSERT_EQUAL(test_buffer[2], 10)
    CU_ASSERT_EQUAL(test_buffer[3], 28)
    CU_ASSERT_EQUAL(test_buffer[4], 138)
    CU_ASSERT_EQUAL(test_buffer[5], 48376)
}

void test_merge_sort(void) {
    int test_buffer[] = {10, 48376, -1928, 0, 28, 138};
    sort_mergesort(test_buffer, (sizeof(test_buffer) / sizeof(test_buffer[0])));
    CU_ASSERT_EQUAL(test_buffer[0], -1928)
    CU_ASSERT_EQUAL(test_buffer[1], 0)
    CU_ASSERT_EQUAL(test_buffer[2], 10)
    CU_ASSERT_EQUAL(test_buffer[3], 28)
    CU_ASSERT_EQUAL(test_buffer[4], 138)
    CU_ASSERT_EQUAL(test_buffer[5], 48376)
}
