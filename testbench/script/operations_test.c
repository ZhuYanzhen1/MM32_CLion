//
// Created by Lao·Zhu on 2021/12/9.
//

#include "operations_test.h"
#include "fast_math.h"
#include "CUnit/Basic.h"

void test_operation_arctan(void) {
    float atan1 = atan_fast(1.0f);
    float atan10 = atan_fast(10.0f);
    float atanm1 = atan_fast(-1.0f);
    float atanm10 = atan_fast(-10.0f);
    CU_ASSERT((atan1 > 0.7853f && atan1 < 0.7854f))
    CU_ASSERT((atanm1 < -0.7853f && atanm1 > -0.7854f))
    CU_ASSERT((atan10 > 1.4711f && atan10 < 1.4712f))
    CU_ASSERT((atanm10 < -1.4711f && atanm10 > -1.4712f))

}

void test_operation_sqrt(void) {
    float sqrt2 = fast_sqrt(2);
    float sqrt7 = fast_sqrt(7);
    float sqrt10 = fast_sqrt(10);
    float sqrt131 = fast_sqrt(131);
    CU_ASSERT((sqrt2 > 1.4142f && sqrt2 < 1.4143f))
    CU_ASSERT((sqrt7 > 2.6457f && sqrt7 < 2.6458f))
    CU_ASSERT((sqrt10 > 3.1622f && sqrt10 < 3.1623f))
    CU_ASSERT((sqrt131 > 11.4455f && sqrt131 < 11.4456f))
}
