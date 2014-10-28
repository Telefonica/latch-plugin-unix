void test_get_account(void);
void test_get_account_equals(void);
void test_get_account_longer_accountId(void);
void test_get_account_shorter_accountId(void);
void test_get_account_no_value(void);
void test_get_account_user_not_found(void);

void test_get_config(void);
void test_get_config_bad_config_file(void);
void test_get_config_longer_length(void);
void test_get_config_shorter_length(void);
void test_get_config_empty(void);
void test_get_config_no_value(void);
void test_get_config_gt_bufsiz(void);

void test_count_account(void);
void test_count_account_bad_length(void);
void test_count_account_null(void);
void test_count_account_null_acc_file(void);
void test_count_account_bad_acc_file(void);

void test_append_accountId(void);
void test_append_accountId_longer(void);
void test_append_accountId_shorter(void);
void test_append_accountId_no_value(void);
void test_append_accountId_no_file(void);

void test_delete_accountId(void);
void test_delete_accountId_longer(void);
void test_delete_accountId_shorter(void);
void test_delete_accountId_no_value(void);
void test_delete_accountId_no_file(void);

int init_suiteUtil(void);
int clean_suiteUtil(void);