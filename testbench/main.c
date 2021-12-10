//
// Created by Lao·Zhu on 2021/12/9.
//

#include "main.h"

int main() {
    CU_pSuite operate_suite = NULL;
    initialize_cunit_suite(&operate_suite, "operate_suit");

    add_test_scripts_to_suite(&operate_suite, "operation-add", test_operation_add);
    add_test_scripts_to_suite(&operate_suite, "operation-minus", test_operation_minus);
    add_test_scripts_to_suite(&operate_suite, "operation-multiply", test_operation_multiply);
    add_test_scripts_to_suite(&operate_suite, "operation-divide", test_operation_divide);

    return generate_statements_report();
}
