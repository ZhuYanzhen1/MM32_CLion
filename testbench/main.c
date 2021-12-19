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

    CU_pSuite mdtp_suite = NULL;
    initialize_cunit_suite(&mdtp_suite, "mdtp_suit");
    add_test_scripts_to_suite(&mdtp_suite, "mdtp_encrypt", test_mdtp_encrypt);

    return generate_statements_report();
}
