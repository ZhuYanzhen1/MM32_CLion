//
// Created by Lao·Zhu on 2021/12/9.
//

#ifndef TESTBENCH__MAIN_H_
#define TESTBENCH__MAIN_H_

/////////////// System ///////////////
#include <string.h>
#include <stdio.h>

/////////////// CUnit ///////////////
#include "CUnit/Basic.h"
#include "CUnit/Automated.h"

/////////////// Script ///////////////
#include "operations_test.h"
#include "mdtp_test.h"
#include "gps_test.h"

int initialize_cunit_suite(CU_pSuite *psuite_ptr, const char *strName);

int generate_statements_report(void);

int add_test_scripts_to_suite(CU_pSuite *psuite_ptr, const char *strName, CU_TestFunc pTestFunc);

#endif //TESTBENCH__MAIN_H_
