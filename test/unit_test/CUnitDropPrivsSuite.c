#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <CUnit/CUnit.h>

#include "drop_privs.h"




/*-------------------------------------------------------------------------*
 * Drop_privileges Test
 *-------------------------------------------------------------------------*/

void test_drop_privileges() 
{
    char *e_user_name_initial = get_effective_user_name();
    char *r_user_name_initial = get_user_name();

    CU_ASSERT_PTR_NOT_NULL_FATAL(e_user_name_initial);
    CU_ASSERT_PTR_NOT_NULL_FATAL(r_user_name_initial);

    // drop_privs temp
    int response1 = drop_privileges(0); 
    char *e_user_name1 = get_effective_user_name();
    char *r_user_name1 = get_user_name();

    CU_ASSERT_FALSE(response1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(e_user_name1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(r_user_name1);
    CU_ASSERT_STRING_EQUAL(r_user_name1, e_user_name1);

    // drop_privs temp again
    int response2 = drop_privileges(0); 
    char *e_user_name2 = get_effective_user_name();
    char *r_user_name2 = get_user_name();

    CU_ASSERT_FALSE(response2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(e_user_name2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(r_user_name2);
    CU_ASSERT_STRING_EQUAL(r_user_name2, e_user_name2);

    // restore privs
    int response3 = restore_privileges();
    char *e_user_name3 = get_effective_user_name();
    char *r_user_name3 = get_user_name();

    CU_ASSERT_FALSE(response3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(e_user_name3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(r_user_name3);
    CU_ASSERT_STRING_EQUAL(e_user_name_initial, e_user_name3);

    // drop_privs definitely
    int response4 = drop_privileges(1); 
    char *e_user_name4 = get_effective_user_name();
    char *r_user_name4 = get_user_name();

    CU_ASSERT_FALSE(response4);
    CU_ASSERT_PTR_NOT_NULL_FATAL(e_user_name4);
    CU_ASSERT_PTR_NOT_NULL_FATAL(r_user_name4);
    CU_ASSERT_STRING_EQUAL(r_user_name4, e_user_name4);

    // restore privs
    int response5 = restore_privileges();
    char *e_user_name5 = get_effective_user_name();
    char *r_user_name5 = get_user_name();

    CU_ASSERT_PTR_NOT_NULL_FATAL(e_user_name5);
    CU_ASSERT_PTR_NOT_NULL_FATAL(r_user_name5);
    CU_ASSERT_STRING_EQUAL(r_user_name5, e_user_name5);

    // drop_privs temp (being dropped definitely)
    int response6 = drop_privileges(0); 
    char *e_user_name6 = get_effective_user_name();
    char *r_user_name6 = get_user_name();

    CU_ASSERT_FALSE(response6);
    CU_ASSERT_PTR_NOT_NULL_FATAL(e_user_name6);
    CU_ASSERT_PTR_NOT_NULL_FATAL(r_user_name6);
    CU_ASSERT_STRING_EQUAL(r_user_name6, e_user_name6);

    // drop_privs definitely again
    int response7 = drop_privileges(1); 
    char *e_user_name7 = get_effective_user_name();
    char *r_user_name7 = get_user_name();

    CU_ASSERT_FALSE(response7);
    CU_ASSERT_PTR_NOT_NULL_FATAL(e_user_name7);
    CU_ASSERT_PTR_NOT_NULL_FATAL(r_user_name7);
    CU_ASSERT_STRING_EQUAL(r_user_name7, e_user_name7);
}




/*-------------------------------------------------------------------------*
 * Drop privs Suite
 *-------------------------------------------------------------------------*/

int init_suiteDropPrivs(void)
{
    return 0;
}

int clean_suiteDropPrivs(void)
{
    return 0;
}