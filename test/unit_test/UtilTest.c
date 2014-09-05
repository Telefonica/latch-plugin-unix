#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CuTest.h"
#include "../../lib/util.h"


static const char *pAccountsFile = "test.accounts";
static const char *pConfigFile = "test.conf";



/*-------------------------------------------------------------------------*
 * GetAccount Test
 *-------------------------------------------------------------------------*/

void TestGetAccount(CuTest *tc) {

    const char* input = strdup("user");
    const char* response = getAccountId(input, pAccountsFile);
    const char* expected = "ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8";

    CuAssertStrEquals(tc, expected, response);
}

void TestGetAccount_Equals(CuTest *tc) {

    const char* input1 = strdup("user1");
    const char* input2 = strdup("user2");
    const char* response1 = getAccountId(input1, pAccountsFile);
    const char* response2 = getAccountId(input2, pAccountsFile);
    const char* expected = "123xxxbrt8a782408a396197bv82a664b61eb7dd2f9b97236202e290489d9777";

    CuAssertStrEquals(tc, expected, response1);
    CuAssertStrEquals(tc, expected, response2);
}

void TestGetAccount_Longer_AccountId(CuTest *tc) {

    const char* input = strdup("user3");
    const char* response = getAccountId(input, pAccountsFile);
    const char* expected = NULL;

    CuAssertTrue(tc, response == expected);
}

void TestGetAccount_Shorter_AccountId(CuTest *tc) {

    const char* input = strdup("user4");
    const char* response = getAccountId(input, pAccountsFile);
    const char* expected = NULL;

    CuAssertTrue(tc, response == expected);
}

void TestGetAccount_No_Value(CuTest *tc) {

    const char* input = strdup("root");
    const char* response = getAccountId(input, pAccountsFile);
    const char* expected = NULL;

    CuAssertTrue(tc, response == expected);
}

void TestGetAccount_User_not_found(CuTest *tc) {

    const char* input = strdup("bad_user");
    const char* response = getAccountId(input, pAccountsFile);
    const char* expected = NULL;

    CuAssertTrue(tc, response == expected);
}

/*-------------------------------------------------------------------------*
 * GetConfig Test
 *-------------------------------------------------------------------------*/

void TestGetConfig(CuTest *tc) {

    const char* input1 = strdup("app_id");
    const char* input2 = strdup("secret_key");

    char *response1;
    char *response2; 

    response1 = getConfig(APP_ID_LENGTH, input1, pConfigFile);
    response2 = getConfig(SECRET_KEY_LENGTH, input2, pConfigFile);

    const char* expected1 = "gh7a3yh82B5zjhgeP6y7";
    const char* expected2 = "t6yEdFivm3jDnt4DfghvbnQrb54ed509sTXvs6yu";

    CuAssertStrEquals(tc, expected1, response1);
    CuAssertStrEquals(tc, expected2, response2);
}

void TestGetConfig_Bad_configFile(CuTest *tc) {

    const char* input1 = strdup("app_id");
    const char* input2 = strdup("secret_key");

    char *response1;
    char *response2; 

    response1 = getConfig(APP_ID_LENGTH, input1, "bad_latch.conf");
    response2 = getConfig(SECRET_KEY_LENGTH, input2, "bad_latch.conf");

    char expected1 = NULL;
    char expected2 = NULL; 

    CuAssertTrue(tc, response1 == expected1);
    CuAssertTrue(tc, response2 == expected2);
}

void TestGetConfig_Longer_Length(CuTest *tc) {

    const char* input1 = strdup("app_id_longer");
    const char* input2 = strdup("secret_key_longer");

    char *response1;
    char *response2; 

    response1 = getConfig(APP_ID_LENGTH, input1, pConfigFile);
    response2 = getConfig(SECRET_KEY_LENGTH, input2, pConfigFile);

    const char* expected1 = "gh7a3yh82B5zjvxcxcbx";
    const char* expected2 = "t6yEdFivm3jDnt4fgfgdgsdfnm86878zczvcxDfg";

    CuAssertStrEquals(tc, expected1, response1);
    CuAssertStrEquals(tc, expected2, response2);
}

void TestGetConfig_Shorter_Length(CuTest *tc) {

    const char* input1 = strdup("app_id_shorter");
    const char* input2 = strdup("secret_key_shorter");

    char *response1;
    char *response2; 

    response1 = getConfig(APP_ID_LENGTH, input1, pConfigFile);
    response2 = getConfig(SECRET_KEY_LENGTH, input2, pConfigFile);

    const char* expected1 = "gh7a3yh82B";
    const char* expected2 = "t6yEdFivm3jDnt4Dfgh";

    CuAssertStrEquals(tc, expected1, response1);
    CuAssertStrEquals(tc, expected2, response2);
}

void TestGetConfig_Empty(CuTest *tc) {

    const char* input1 = strdup("app_id");
    const char* input2 = strdup("secret_key");

    char *response1;
    char *response2; 

    response1 = getConfig(APP_ID_LENGTH, input1, "empty.conf");
    response2 = getConfig(SECRET_KEY_LENGTH, input2, "empty.conf");

    char *expected1 = NULL;
    char *expected2 = NULL; 

    CuAssertTrue(tc, response1 == expected1);
    CuAssertTrue(tc, response2 == expected2);
}

void TestGetConfig_No_Value(CuTest *tc) {

    const char* input1 = strdup("app_id_no_value");
    const char* input2 = strdup("secret_key_no_value");

    char *response1 = NULL;
    char *response2 = NULL; 

    response1 = getConfig(APP_ID_LENGTH, input1, pConfigFile);
    response2 = getConfig(SECRET_KEY_LENGTH, input2, pConfigFile);

    char *expected1 = NULL;
    char *expected2 = NULL; 

    CuAssertTrue(tc, response1 == expected1);
    CuAssertTrue(tc, response2 == expected2);
}

void TestGetConfig_Gt_Bufsiz(CuTest *tc) {

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

    const char* input1 = strdup("app_id_gt_bufsiz");
    const char* input2 = strdup("secret_key_gt_bufsiz");

    char *response1;
    char *response2; 

    response1 = getConfig(APP_ID_LENGTH, input1, "temp.conf");
    response2 = getConfig(SECRET_KEY_LENGTH, input2, "temp.conf");

    remove("temp.conf");

    const char* expected1 = "XXXXXXXXXXXXXXXXXXXX";
    const char* expected2 = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

    CuAssertStrEquals(tc, expected1, response1);
    CuAssertStrEquals(tc, expected2, response2);
}


/*-------------------------------------------------------------------------*
 * CountAccountId Test
 *-------------------------------------------------------------------------*/

void TestCountAccount(CuTest *tc) {

    const char* input1 = strdup("ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8");
    const char* input2 = strdup("123xxxbrt8a782408a396197bv82a664b61eb7dd2f9b97236202e290489d9777");
    const char* input3 = strdup("023xxxbrt8a782408a396197bv82a664b61eb7dd2f9b97236202e290489d9777");
    const char* input4 = strdup("ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b9");

    int response1 = countAccountId(input1, pAccountsFile);
    int response2 = countAccountId(input2, pAccountsFile);
    int response3 = countAccountId(input3, pAccountsFile);
    int response4 = countAccountId(input4, pAccountsFile);

    int expected1 = 1;
    int expected2 = 2;
    int expected3 = 0;
    int expected4 = 0;

    CuAssertTrue(tc, response1 == expected1);
    CuAssertTrue(tc, response2 == expected2);
    CuAssertTrue(tc, response3 == expected3);
    CuAssertTrue(tc, response4 == expected4);
}

void TestCountAccount_Bad_Length(CuTest *tc) {

    const char* input = strdup("yuihdcvbn8a782408a397236202e2904rffmjh3");
    int response = countAccountId(input, pAccountsFile);
    int expected = -1;
    CuAssertTrue(tc, response == expected);
}

void TestCountAccount_Null(CuTest *tc) {

    int response = countAccountId(NULL, pAccountsFile);
    int expected = -1;
    CuAssertTrue(tc, response == expected);
}

void TestCountAccount_Null_AccFile(CuTest *tc) {

    const char* input = strdup("ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8");
    int response = countAccountId(input, NULL);
    int expected = -1;
    CuAssertTrue(tc, response == expected);
}

void TestCountAccount_Bad_AccFile(CuTest *tc) {

    const char* input = strdup("ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8");
    int response = countAccountId(input, "bad_acc_file.accounts");
    int expected = -1;
    CuAssertTrue(tc, response == expected);
}



/*-------------------------------------------------------------------------*
 * AppendAccountId Test
 *-------------------------------------------------------------------------*/

void TestAppendAccountId(CuTest *tc) {

    const char* input = strdup("ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8");
    const char* response = appendAccountId("username", input, pAccountsFile);

    const char* response2 = getAccountId("username", pAccountsFile);
    const char* expected2 = "ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8";

    CuAssertStrEquals(tc, expected2, response2);
    CuAssertTrue(tc, !response);
}

void TestAppendAccountId_Longer(CuTest *tc) {

    const char* input = strdup("ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8t543tst45");
    const char* response = appendAccountId("username_longer", input, pAccountsFile);

    const char* response2 = getAccountId("username_longer", pAccountsFile);
    const char* expected2 = NULL;

    CuAssertTrue(tc, expected2 == response2);
    CuAssertTrue(tc, !response);
}

void TestAppendAccountId_Shorter(CuTest *tc) {

    const char* input = strdup("4461eb7dd2f9b97236202e290489d98b8t543tst45");
    const char* response = appendAccountId("username_shorter", input, pAccountsFile);

    const char* response2 = getAccountId("username_shorter", pAccountsFile);
    const char* expected2 = NULL;

    CuAssertTrue(tc, expected2 == response2);
    CuAssertTrue(tc, !response);
}

void TestAppendAccountId_No_Value(CuTest *tc) {

    const char* input = strdup("");
    const char* response = appendAccountId("username_no_value", input, pAccountsFile);

    const char* response2 = getAccountId("username_no_value", pAccountsFile);
    const char* expected2 = NULL;

    CuAssertTrue(tc, expected2 == response2);
    CuAssertTrue(tc, !response);
}

void TestAppendAccountId_No_File(CuTest *tc) {

    const char* input = strdup("ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8");
    const char* response = appendAccountId("username_no_file", input, "no_file");

    const char* response2 = getAccountId("username", pAccountsFile);
    const char* expected2 = "ddd917b408a782408a3961978a82a664461eb7dd2f9b97236202e290489d98b8";

    remove("no_file");

    CuAssertStrEquals(tc, expected2, response2);
    CuAssertTrue(tc, !response);
}



/*-------------------------------------------------------------------------*
 * DeleteAccountId Test
 *-------------------------------------------------------------------------*/

void TestDeleteAccountId(CuTest *tc) {

    const char* response = deleteAccountId("username", pAccountsFile);

    const char* response2 = getAccountId("username", pAccountsFile);
    const char* expected2 = NULL;

    CuAssertTrue(tc, expected2 == response2);
    CuAssertTrue(tc, !response);
}

void TestDeleteAccountId_Longer(CuTest *tc) {

    const char* response = deleteAccountId("username_longer", pAccountsFile);

    const char* response2 = getAccountId("username_longer", pAccountsFile);
    const char* expected2 = NULL;

    CuAssertTrue(tc, expected2 == response2);
    CuAssertTrue(tc, !response);
}

void TestDeleteAccountId_Shorter(CuTest *tc) {

    const char* response = deleteAccountId("username_shorter", pAccountsFile);

    const char* response2 = getAccountId("username_shorter", pAccountsFile);
    const char* expected2 = NULL;

    CuAssertTrue(tc, expected2 == response2);
    CuAssertTrue(tc, !response);
}

void TestDeleteAccountId_No_Value(CuTest *tc) {

    const char* response = deleteAccountId("username_no_value", pAccountsFile);

    const char* response2 = getAccountId("username_no_value", pAccountsFile);
    const char* expected2 = NULL;

    CuAssertTrue(tc, expected2 == response2);
    CuAssertTrue(tc, !response);
}

void TestDeleteAccountId_No_File(CuTest *tc) {

    const char* response = deleteAccountId("username_no_file", "no_file");

    CuAssertTrue(tc, response);
}



/*-------------------------------------------------------------------------*
 * Drop_privileges Test
 *-------------------------------------------------------------------------*/

void TestDropPrivileges(CuTest *tc) {

    const char* response1 = get_effective_user_name();
    CuAssertStrEquals(tc, "root", response1);
    
    const char* expected3 = get_user_name();

    int error = drop_privileges();

    const char* response2 = get_effective_user_name();
    const char* response3 = get_user_name();


    CuAssertTrue(tc, strcmp("root", response2) != 0);
    CuAssertStrEquals(tc, expected3, response3);
    CuAssertTrue(tc, ! error);
}



/*-------------------------------------------------------------------------*
 * UtilSuite Test
 *-------------------------------------------------------------------------*/


CuSuite* UtilGetSuite() {
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestGetAccount);
    SUITE_ADD_TEST(suite, TestGetAccount_Equals);
    SUITE_ADD_TEST(suite, TestGetAccount_Longer_AccountId);
    SUITE_ADD_TEST(suite, TestGetAccount_Shorter_AccountId);
    SUITE_ADD_TEST(suite, TestGetAccount_No_Value);
    SUITE_ADD_TEST(suite, TestGetAccount_User_not_found);

    SUITE_ADD_TEST(suite, TestGetConfig);
    SUITE_ADD_TEST(suite, TestGetConfig_Bad_configFile);
    SUITE_ADD_TEST(suite, TestGetConfig_Longer_Length);
    SUITE_ADD_TEST(suite, TestGetConfig_Shorter_Length);
    SUITE_ADD_TEST(suite, TestGetConfig_Empty);
    SUITE_ADD_TEST(suite, TestGetConfig_No_Value);
    SUITE_ADD_TEST(suite, TestGetConfig_Gt_Bufsiz);

    SUITE_ADD_TEST(suite, TestCountAccount);
    SUITE_ADD_TEST(suite, TestCountAccount_Bad_Length);
    SUITE_ADD_TEST(suite, TestCountAccount_Null);
    SUITE_ADD_TEST(suite, TestCountAccount_Null_AccFile);
    SUITE_ADD_TEST(suite, TestCountAccount_Bad_AccFile);

    SUITE_ADD_TEST(suite, TestAppendAccountId);
    SUITE_ADD_TEST(suite, TestAppendAccountId_Longer);
    SUITE_ADD_TEST(suite, TestAppendAccountId_Shorter);
    SUITE_ADD_TEST(suite, TestAppendAccountId_No_Value);
    SUITE_ADD_TEST(suite, TestAppendAccountId_No_File);

    SUITE_ADD_TEST(suite, TestDeleteAccountId);
    SUITE_ADD_TEST(suite, TestDeleteAccountId_Longer);
    SUITE_ADD_TEST(suite, TestDeleteAccountId_Shorter);
    SUITE_ADD_TEST(suite, TestDeleteAccountId_No_Value);
    SUITE_ADD_TEST(suite, TestDeleteAccountId_No_File);

    SUITE_ADD_TEST(suite, TestDropPrivileges);

    return suite;
}
