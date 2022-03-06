#include "main.h"

int generate_statements_report(void) {
#ifdef CMAKE_GITHUB_BUILD
    /* Run all tests using the CUnit Automatic interface */
    CU_set_output_filename("Report");
    CU_list_tests_to_file();
    CU_automated_run_tests();
    printf("Run all tests using the CUnit Automatic interface.\r\n");
#else
    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("Run all tests using the CUnit Basic interface.\r\n");
#endif
    CU_cleanup_registry();
    printf("Clean up all registry.\r\n");
    return CU_get_error();
}

int initialize_cunit_suite(CU_pSuite *psuite_ptr, const char *strName) {
    static int initialize_registry_flag = 0;
    if (initialize_registry_flag == 0) {
        printf("Initialize redistry.\r\n");
        /* initialize the CUnit test registry */
        if (CUE_SUCCESS != CU_initialize_registry())
            return CU_get_error();
        initialize_registry_flag = 1;
    }

    /* add a suite to the registry */
    *psuite_ptr = CU_add_suite(strName, NULL, NULL);
    if (NULL == *psuite_ptr) {
        CU_cleanup_registry();
        printf("suite add failed.\r\n");
        return CU_get_error();
    }
    return 0;
}

int add_test_scripts_to_suite(CU_pSuite *psuite_ptr, const char *strName, CU_TestFunc pTestFunc) {
    /* add the tests to the suite */
    if (NULL == CU_add_test(*psuite_ptr, strName, pTestFunc)) {
        CU_cleanup_registry();
        printf("test scripts add failed.\r\n");
        return CU_get_error();
    }
    return 0;
}