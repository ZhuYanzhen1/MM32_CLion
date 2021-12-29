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
    add_test_scripts_to_suite(&gps_suite, "gps_nmea_comma_position", test_nmea_comma_position);
    add_test_scripts_to_suite(&gps_suite, "gps_nmea_pow", test_nmea_pow);
    add_test_scripts_to_suite(&gps_suite, "gps_nmea_str2num", test_nmea_str2num);
    add_test_scripts_to_suite(&gps_suite, "gps_nmea_gpgga_analysis", test_nmea_gpgga_analysis);
    add_test_scripts_to_suite(&gps_suite, "gps_nmea_gpant_analysis", test_nmea_gpant_analysis);

    CU_pSuite sort_suite = NULL;
    initialize_cunit_suite(&sort_suite, "sort_suit");
    add_test_scripts_to_suite(&sort_suite, "sort_heapsort", test_heap_sort);
//    add_test_scripts_to_suite(&sort_suite, "sort_mergesort", test_merge_sort);
    add_test_scripts_to_suite(&sort_suite, "sort_quicksort", test_quick_sort);

    return generate_statements_report();
}
