//
// Created by Lao·Zhu on 2021/12/9.
//

#include "main.h"

int main(int argc, char **argv) {
    CU_pSuite fast_math_suite = NULL;
    initialize_cunit_suite(&fast_math_suite, "fast_math_suit");
    add_test_scripts_to_suite(&fast_math_suite, "fast_math-atan", test_operation_arctan);
    add_test_scripts_to_suite(&fast_math_suite, "fast_math-sqrt", test_operation_sqrt);

    CU_pSuite mdtp_suite = NULL;
    initialize_cunit_suite(&mdtp_suite, "mdtp_suit");
    add_test_scripts_to_suite(&mdtp_suite, "mdtp-encrypt", test_mdtp_encrypt);
    add_test_scripts_to_suite(&mdtp_suite, "mdtp-decrypt", test_mdtp_decrypt);

    CU_pSuite sort_suite = NULL;
    initialize_cunit_suite(&sort_suite, "sort_suit");
    add_test_scripts_to_suite(&sort_suite, "sort_heapsort", test_heap_sort);
    add_test_scripts_to_suite(&sort_suite, "sort_mergesort", test_merge_sort);
    add_test_scripts_to_suite(&sort_suite, "sort_quicksort", test_quick_sort);

    CU_pSuite verification_suite = NULL;
    initialize_cunit_suite(&verification_suite, "verification_suit");
    add_test_scripts_to_suite(&verification_suite, "verification_checksum", test_verification_checksum);
    add_test_scripts_to_suite(&verification_suite, "verification_checkxor", test_verification_checkxor);
    add_test_scripts_to_suite(&verification_suite, "verification_crc8", test_verification_crc8);
    add_test_scripts_to_suite(&verification_suite, "verification_crc16", test_verification_crc16);

    CU_pSuite solver_suite = NULL;
    initialize_cunit_suite(&solver_suite, "solver_suite");
    add_test_scripts_to_suite(&solver_suite, "riccati_solver", test_riccati_solver);
    add_test_scripts_to_suite(&solver_suite, "calibrate_solver", test_calibration_solver);
    add_test_scripts_to_suite(&solver_suite, "test_lqr_contrl", test_lqr_contrl);

    CU_pSuite sensor_decode_suite = NULL;
    initialize_cunit_suite(&sensor_decode_suite, "sensor_decode_suite");
    add_test_scripts_to_suite(&sensor_decode_suite, "gps_nmea_gnrmc_analysis",
                              test_nmea_gnrmc_analysis);
    add_test_scripts_to_suite(&sensor_decode_suite, "test_precossing_proc_to_control",
                              test_precossing_proc_to_control);
//    add_test_scripts_to_suite(&sensor_decode_suite, "test_precossing_variable_length_data",
//                              test_precossing_variable_length_data);

#ifdef USING_GTK_GUI_MACRO
    return mainform_config(argc, argv) | generate_statements_report();
#else
    return generate_statements_report();
#endif
}
