//
// Created by Lao·Zhu on 2021/12/9.
//

#include "main.h"
#include "fast_math.h"

int main() {
    CU_pSuite fast_math_suite = NULL;
    initialize_cunit_suite(&fast_math_suite, "fast_math_suit");
    add_test_scripts_to_suite(&fast_math_suite, "fast_math-atan", test_operation_arctan);
    add_test_scripts_to_suite(&fast_math_suite, "fast_math-sqrt", test_operation_sqrt);

    CU_pSuite mdtp_suite = NULL;
    initialize_cunit_suite(&mdtp_suite, "mdtp_suit");
    add_test_scripts_to_suite(&mdtp_suite, "mdtp-encrypt", test_mdtp_encrypt);
    add_test_scripts_to_suite(&mdtp_suite, "mdtp-decrypt", test_mdtp_decrypt);

    CU_pSuite gps_suite = NULL;
    initialize_cunit_suite(&gps_suite, "gps_suit");
    add_test_scripts_to_suite(&gps_suite, "gps_nmea_gnrmc_analysis", test_nmea_gnrmc_analysis);
    add_test_scripts_to_suite(&gps_suite, "lat_and_lon_rotation_shift", test_lat_and_lon_rotation_shift);

    CU_pSuite sort_suite = NULL;
    initialize_cunit_suite(&sort_suite, "sort_suit");
    add_test_scripts_to_suite(&sort_suite, "sort_heapsort", test_heap_sort);
    add_test_scripts_to_suite(&sort_suite, "sort_mergesort", test_merge_sort);
    add_test_scripts_to_suite(&sort_suite, "sort_quicksort", test_quick_sort);

    CU_pSuite calibrate_suite = NULL;
    initialize_cunit_suite(&calibrate_suite, "calibrate_suite");
    add_test_scripts_to_suite(&calibrate_suite, "mag_calibrate", test_mag_calibration);

    CU_pSuite solver_suite = NULL;
    initialize_cunit_suite(&solver_suite, "solver_suite");
    add_test_scripts_to_suite(&solver_suite, "riccati_solver", test_riccati_solver);

    return generate_statements_report();
}
