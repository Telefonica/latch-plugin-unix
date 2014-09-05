#!/usr/bin/env bash
#run as root

echo '###############'
echo '  Latch tests  '
echo '###############'
echo


# pair tests
./test_pair_bad_char_token.sh
echo
./test_pair_bad_length_token.sh
echo
sudo ./test_pair_invalid_app_and_secret.sh
echo
./test_pair_invalid_token.sh
echo

# status tests
sudo ./test_status_without_acc_file.sh
echo
./test_status_not_paired.sh
echo
sudo ./test_status_invalid_app_and_secret.sh
echo
sudo ./test_status_invalid_account_id.sh
echo

# operation status tests
sudo ./test_op_status_without_acc_file.sh
echo
./test_op_status_not_paired.sh
echo
./test_op_status_not_operation.sh
echo
sudo ./test_op_status_invalid_app_and_secret.sh
echo
sudo ./test_op_status_invalid_operation_id.sh
echo
sudo ./test_op_status_invalid_account_id.sh
echo

# unpair tests
sudo ./test_unpair_without_acc_file.sh
echo
./test_unpair_not_paired.sh
echo
sudo ./test_unpair_success.sh
echo
sudo ./test_unpair_succ_with_other_same_acc_id_user.sh
echo

# interactive tests
sudo ./test_generating_token.sh
echo

# network tests
sudo ./test_without_network.sh
echo
