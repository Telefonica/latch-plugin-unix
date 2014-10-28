#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <CUnit/CUnit.h>

#include "util.h"


static const char *pAccountsFile = "test.accounts";
static const char *pConfigFile = "test.conf";



/*-------------------------------------------------------------------------*
 * GetAccount Test
 *-------------------------------------------------------------------------*/

void test_get_account() 
{
    const char* input = "user";
    const char* response = getAccountId(input, pAccountsFile);
    const char* expected = "ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8";

    CU_ASSERT_PTR_NOT_NULL_FATAL(response);
    CU_ASSERT_STRING_EQUAL(expected, response);
}

void test_get_account_equals() 
{
    const char* input1 = "user1";
    const char* input2 = "user2";
    const char* response1 = getAccountId(input1, pAccountsFile);
    const char* response2 = getAccountId(input2, pAccountsFile);
    const char* expected = "123xxxbrt8a782408a396197bv82a664b61eb7dd2f9b97236202e290489d9777";

    CU_ASSERT_PTR_NOT_NULL_FATAL(response1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(response2);
    CU_ASSERT_STRING_EQUAL(expected, response1);
    CU_ASSERT_STRING_EQUAL(expected, response2);
}

void test_get_account_longer_accountId()
{
    const char* input = "user3";
    const char* response = getAccountId(input, pAccountsFile);

    CU_ASSERT_PTR_NULL(response);
}

void test_get_account_shorter_accountId() 
{
    const char* input = "user4";
    const char* response = getAccountId(input, pAccountsFile);

    CU_ASSERT_PTR_NULL(response);
}

void test_get_account_no_value() 
{
    const char* input = "root";
    const char* response = getAccountId(input, pAccountsFile);

    CU_ASSERT_PTR_NULL(response);
}

void test_get_account_user_not_found() 
{
    const char* input = "bad_user";
    const char* response = getAccountId(input, pAccountsFile);

    CU_ASSERT_PTR_NULL(response);
}


/*-------------------------------------------------------------------------*
 * GetConfig Test
 *-------------------------------------------------------------------------*/

void test_get_config() 
{
    const char* input1 = "app_id";
    const char* input2 = "secret_key";

    char *response1;
    char *response2; 

    response1 = getConfig(APP_ID_LENGTH, input1, pConfigFile);
    response2 = getConfig(SECRET_KEY_LENGTH, input2, pConfigFile);

    const char* expected1 = "gh7a3yh82B5zjhgeP6y7";
    const char* expected2 = "t6yEdFivm3jDnt4DfghvbnQrb54ed509sTXvs6yu";

    CU_ASSERT_PTR_NOT_NULL_FATAL(response1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(response2);
    CU_ASSERT_STRING_EQUAL(expected1, response1);
    CU_ASSERT_STRING_EQUAL(expected2, response2);
}

void test_get_config_bad_config_file() 
{
    const char* input1 = "app_id";
    const char* input2 = "secret_key";

    char *response1;
    char *response2; 

    response1 = getConfig(APP_ID_LENGTH, input1, "bad_latch.conf");
    response2 = getConfig(SECRET_KEY_LENGTH, input2, "bad_latch.conf");

    CU_ASSERT_PTR_NULL(response1);
    CU_ASSERT_PTR_NULL(response2);
}

void test_get_config_longer_length() 
{
    const char* input1 = "app_id_longer";
    const char* input2 = "secret_key_longer";

    char *response1;
    char *response2; 

    response1 = getConfig(APP_ID_LENGTH, input1, pConfigFile);
    response2 = getConfig(SECRET_KEY_LENGTH, input2, pConfigFile);

    const char* expected1 = "gh7a3yh82B5zjvxcxcbx";
    const char* expected2 = "t6yEdFivm3jDnt4fgfgdgsdfnm86878zczvcxDfg";

    CU_ASSERT_PTR_NOT_NULL_FATAL(response1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(response2);
    CU_ASSERT_STRING_EQUAL(expected1, response1);
    CU_ASSERT_STRING_EQUAL(expected2, response2);
}

void test_get_config_shorter_length() 
{
    const char* input1 = "app_id_shorter";
    const char* input2 = "secret_key_shorter";

    char *response1;
    char *response2; 

    response1 = getConfig(APP_ID_LENGTH, input1, pConfigFile);
    response2 = getConfig(SECRET_KEY_LENGTH, input2, pConfigFile);

    const char* expected1 = "gh7a3yh82B";
    const char* expected2 = "t6yEdFivm3jDnt4Dfgh";

    CU_ASSERT_PTR_NOT_NULL_FATAL(response1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(response2);
    CU_ASSERT_STRING_EQUAL(expected1, response1);
    CU_ASSERT_STRING_EQUAL(expected2, response2);
}

void test_get_config_empty() 
{
    const char* input1 = "app_id";
    const char* input2 = "secret_key";

    char *response1;
    char *response2; 

    response1 = getConfig(APP_ID_LENGTH, input1, "empty.conf");
    response2 = getConfig(SECRET_KEY_LENGTH, input2, "empty.conf");

    CU_ASSERT_PTR_NULL(response1);
    CU_ASSERT_PTR_NULL(response2);
}

void test_get_config_no_value() 
{
    const char* input1 = "app_id_no_value";
    const char* input2 = "secret_key_no_value";

    char *response1 = NULL;
    char *response2 = NULL; 

    response1 = getConfig(APP_ID_LENGTH, input1, pConfigFile);
    response2 = getConfig(SECRET_KEY_LENGTH, input2, pConfigFile);

    CU_ASSERT_PTR_NULL(response1);
    CU_ASSERT_PTR_NULL(response2);
}

void test_get_config_gt_bufsiz() 
{
    FILE *fd = fopen("temp.conf", "w");
    fprintf(fd, "app_id_gt_bufsiz = ");
        
    int i;
    for (i = 0 ; i < BUFSIZ + 10 ; i++) {
        fprintf(fd, "X");
    }
    fprintf(fd, "\n\n");
    fprintf(fd, "secret_key_gt_bufsiz = ");

    for (i = 0 ; i < BUFSIZ + 10 ; i++) {
        fprintf(fd, "x");
    }
    fprintf(fd, "\n\n");

    fclose(fd);

    const char* input1 = "app_id_gt_bufsiz";
    const char* input2 = "secret_key_gt_bufsiz";

    char *response1;
    char *response2; 

    response1 = getConfig(APP_ID_LENGTH, input1, "temp.conf");
    response2 = getConfig(SECRET_KEY_LENGTH, input2, "temp.conf");

    remove("temp.conf");

    const char* expected1 = "XXXXXXXXXXXXXXXXXXXX";
    const char* expected2 = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

    CU_ASSERT_PTR_NOT_NULL_FATAL(response1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(response2);
    CU_ASSERT_STRING_EQUAL(expected1, response1);
    CU_ASSERT_STRING_EQUAL(expected2, response2);
}


/*-------------------------------------------------------------------------*
 * CountAccountId Test
 *-------------------------------------------------------------------------*/

void test_count_account() 
{
    const char* input1 = "ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8";
    const char* input2 = "123xxxbrt8a782408a396197bv82a664b61eb7dd2f9b97236202e290489d9777";
    const char* input3 = "023xxxbrt8a782408a396197bv82a664b61eb7dd2f9b97236202e290489d9777";
    const char* input4 = "ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b9";

    int response1 = countAccountId(input1, pAccountsFile);
    int response2 = countAccountId(input2, pAccountsFile);
    int response3 = countAccountId(input3, pAccountsFile);
    int response4 = countAccountId(input4, pAccountsFile);

    int expected1 = 1;
    int expected2 = 2;
    int expected3 = 0;
    int expected4 = 0;

    CU_ASSERT_TRUE(response1 == expected1);
    CU_ASSERT_TRUE(response2 == expected2);
    CU_ASSERT_TRUE(response3 == expected3);
    CU_ASSERT_TRUE(response4 == expected4);
}

void test_count_account_bad_length() 
{
    const char* input = "yuihdcvbn8a782408a397236202e2904rffmjh3";
    int response = countAccountId(input, pAccountsFile);
    int expected = -1;

    CU_ASSERT_TRUE(response == expected);
}

void test_count_account_null() 
{
    int response = countAccountId(NULL, pAccountsFile);
    int expected = -1;

    CU_ASSERT_TRUE(response == expected);
}

void test_count_account_null_acc_file() 
{
    const char* input = "ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8";
    int response = countAccountId(input, NULL);
    int expected = -1;

    CU_ASSERT_TRUE(response == expected);
}

void test_count_account_bad_acc_file() 
{
    const char* input = "ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8";
    int response = countAccountId(input, "bad_acc_file.accounts");
    int expected = -1;

    CU_ASSERT_TRUE(response == expected);
}



/*-------------------------------------------------------------------------*
 * AppendAccountId Test
 *-------------------------------------------------------------------------*/

void test_append_accountId() 
{
    const char* input = "ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8";
    int response1 = appendAccountId("username", input, pAccountsFile);

    const char* response2 = getAccountId("username", pAccountsFile);
    const char* expected2 = "ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8";

    CU_ASSERT_FALSE(response1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(response2);
    CU_ASSERT_STRING_EQUAL(expected2, response2);
}

void test_append_accountId_longer() 
{
    const char* input = "ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8t543tst45";
    int response1 = appendAccountId("username_longer", input, pAccountsFile);

    const char* response2 = getAccountId("username_longer", pAccountsFile);

    CU_ASSERT_FALSE(response1);
    CU_ASSERT_PTR_NULL(response2);
}

void test_append_accountId_shorter() 
{
    const char* input = "4461eb7dd2f9b97236202e290489d98b8t543tst45";
    int response1 = appendAccountId("username_shorter", input, pAccountsFile);

    const char* response2 = getAccountId("username_shorter", pAccountsFile);

    CU_ASSERT_FALSE(response1);
    CU_ASSERT_PTR_NULL(response2);
}

void test_append_accountId_no_value() 
{
    const char* input = "";
    int response1 = appendAccountId("username_no_value", input, pAccountsFile);

    const char* response2 = getAccountId("username_no_value", pAccountsFile);

    CU_ASSERT_FALSE(response1);
    CU_ASSERT_PTR_NULL(response2);
}

void test_append_accountId_no_file() 
{
    const char* input = "ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8";
    int response1 = appendAccountId("username_no_file", input, "no_file");

    const char* response2 = getAccountId("username", pAccountsFile);
    const char* expected2 = "ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8";

    remove("no_file");

    CU_ASSERT_FALSE(response1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(response2);
    CU_ASSERT_STRING_EQUAL(expected2, response2);
}



/*-------------------------------------------------------------------------*
 * DeleteAccountId Test
 *-------------------------------------------------------------------------*/

void test_delete_accountId() 
{
    int response1 = deleteAccountId("username", pAccountsFile);

    const char* response2 = getAccountId("username", pAccountsFile);

    CU_ASSERT_FALSE(response1);
    CU_ASSERT_PTR_NULL(response2);
}

void test_delete_accountId_longer() 
{
    int response1 = deleteAccountId("username_longer", pAccountsFile);

    const char* response2 = getAccountId("username_longer", pAccountsFile);

    CU_ASSERT_FALSE(response1);
    CU_ASSERT_PTR_NULL(response2);
}

void test_delete_accountId_shorter() 
{
    int response1 = deleteAccountId("username_shorter", pAccountsFile);

    const char* response2 = getAccountId("username_shorter", pAccountsFile);

    CU_ASSERT_FALSE(response1);
    CU_ASSERT_PTR_NULL(response2);
}

void test_delete_accountId_no_value() 
{
    int response1 = deleteAccountId("username_no_value", pAccountsFile);

    const char* response2 = getAccountId("username_no_value", pAccountsFile);

    CU_ASSERT_FALSE(response1);
    CU_ASSERT_PTR_NULL(response2);

}

void test_delete_accountId_no_file() 
{
    int response = deleteAccountId("username_no_file", "no_file");

    CU_ASSERT_TRUE(response);
}




/*-------------------------------------------------------------------------*
 * Util Suite
 *-------------------------------------------------------------------------*/

int init_suiteUtil(void)
{
    return 0;
}

int clean_suiteUtil(void)
{
    return 0;
}