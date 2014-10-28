#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>

#include "CUnitCharsetSuite.h"
#include "CUnitUtilSuite.h"
#include "CUnitDropPrivsSuite.h"




/*-------------------------------------------------------------------------*
 * Main
 *-------------------------------------------------------------------------*/

int main(int argc, char *argv[]){

    CU_pSuite pSuiteCharset = NULL;
    CU_pSuite pSuiteUtil = NULL;
    CU_pSuite pSuiteDropPrivs = NULL;


    const char *output_file = "cunit-results";


    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuiteCharset = CU_add_suite("Suite_Charset", init_suiteCharset, clean_suiteCharset);
    if (NULL == pSuiteCharset) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    pSuiteUtil = CU_add_suite("Suite_Util", init_suiteUtil, clean_suiteUtil);
    if (NULL == pSuiteUtil) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    pSuiteDropPrivs = CU_add_suite("Suite_DropPrivs", init_suiteDropPrivs, clean_suiteDropPrivs);
    if (NULL == pSuiteDropPrivs) {
        CU_cleanup_registry();
        return CU_get_error();
    }


    /* add the tests to the suite */
    if (
        (NULL == CU_add_test(pSuiteCharset, "test of test_valid_code()", test_valid_code)) ||
        (NULL == CU_add_test(pSuiteCharset, "test of test_valid_code_exceptions()", test_valid_code_exceptions)) ||
        (NULL == CU_add_test(pSuiteCharset, "test of test_valid_code_invalid_chars()", test_valid_code_invalid_chars)) ||
        (NULL == CU_add_test(pSuiteCharset, "test of test_valid_code_short()", test_valid_code_short)) ||
        (NULL == CU_add_test(pSuiteCharset, "test of test_valid_code_long()", test_valid_code_long)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
     /* add the tests to the suite */
    if (
        (NULL == CU_add_test(pSuiteUtil, "test of test_get_account()", test_get_account)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_get_account_equals()", test_get_account_equals)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_get_account_longer_accountId()", test_get_account_longer_accountId)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_get_account_shorter_accountId()", test_get_account_shorter_accountId)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_get_account_no_value()", test_get_account_no_value)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_get_account_user_not_found()", test_get_account_user_not_found)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_get_config()", test_get_config)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_get_config_bad_config_file()", test_get_config_bad_config_file)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_get_config_longer_length()", test_get_config_longer_length)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_get_config_shorter_length()", test_get_config_shorter_length)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_get_config_empty()", test_get_config_empty)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_get_config_no_value()", test_get_config_no_value)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_get_config_gt_bufsiz()", test_get_config_gt_bufsiz)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_count_account()", test_count_account)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_count_account_bad_length()", test_count_account_bad_length)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_count_account_null()", test_count_account_null)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_count_account_null_acc_file()", test_count_account_null_acc_file)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_count_account_bad_acc_file()", test_count_account_bad_acc_file)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_append_accountId()", test_append_accountId)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_append_accountId_longer()", test_append_accountId_longer)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_append_accountId_shorter()", test_append_accountId_shorter)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_append_accountId_no_value()", test_append_accountId_no_value)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_append_accountId_no_file()", test_append_accountId_no_file)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_delete_accountId()", test_delete_accountId)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_delete_accountId_longer()", test_delete_accountId_longer)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_delete_accountId_shorter()", test_delete_accountId_shorter)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_delete_accountId_no_value()", test_delete_accountId_no_value)) ||
        (NULL == CU_add_test(pSuiteUtil, "test of test_delete_accountId_no_file()", test_delete_accountId_no_file))
        )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (
        (NULL == CU_add_test(pSuiteDropPrivs, "test of test_drop_privileges()", test_drop_privileges)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*
    CU_list_tests_to_file();
    CU_set_output_filename(output_file);
    CU_automated_run_tests();        
    */
    /* Run all tests using the CUnit Basic interface */   
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    

    CU_cleanup_registry();
    return CU_get_error();
}