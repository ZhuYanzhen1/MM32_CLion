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
    add_test_scripts_to_suite(&mdtp_suite, "mdtp-encrypt", test_mdtp_encrypt);
    add_test_scripts_to_suite(&mdtp_suite, "mdtp-decrypt", test_mdtp_decrypt);

    CU_pSuite gps_suite = NULL;
    initialize_cunit_suite(&gps_suite, "gps_suit");
//    add_test_scripts_to_suite(&gps_suite, "gps-encrypt", test_gps_encrypt);
//    add_test_scripts_to_suite(&gps_suite, "gps-decrypt", test_gps_decrypt);

    return generate_statements_report();
}
